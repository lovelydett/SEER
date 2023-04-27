//
// Created by Yuting Xie
// 23-4-27

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <pthread.h>
#include <random>
#include <string>
#include <sys/mman.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>

std::default_random_engine random_engine;

static const int kShmSize = 2048;
static const int kNumProcesses = 3;

struct Shm {
  pthread_mutex_t mutex;
  int fd_;
  void *addr_;
};

int main() {
  std::vector<std::string> shm_names = {"/test", "/test2", "/test3"};
  for (auto &shm_name : shm_names) {
    shm_unlink(shm_name.c_str());
  }
  std::vector<Shm *> shms;
  shms.resize(shm_names.size());
  for (int i = 0; i < shms.size(); ++i) {
    int fd =
        shm_open(shm_names[i].c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    assert(fd != -1);
    assert(ftruncate(fd, kShmSize) != -1);

    // mmap and get the attr
    void *addr =
        mmap(NULL, kShmSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    assert(addr != MAP_FAILED);

    // interpret the addr as a Shm and set up df and attr
    shms[i] = reinterpret_cast<Shm *>(addr);
    shms[i]->fd_ = fd;
    shms[i]->addr_ = addr;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(shms[i]->mutex), &attr);
  }

  // Check the shm
  for (int i = 0; i < shms.size(); ++i) {
    auto *shm = shms[i];
    printf("shm %d: fd = %d, addr = %p\n", i, shm->fd_, shm->addr_);
  }

  for (int i = 0; i < kNumProcesses; ++i) {
    pid_t pid = fork();
    assert(pid != -1);
    if (pid == 0) {
      while (true) {
        printf("In pid = %d\n", getpid());
        int idx = random_engine() % shms.size();
        auto *shm = shms[idx];
        pthread_mutex_lock(&(shm->mutex)); // Acquire the lock
        printf("Got lock the lock from %s by pid = %d\n",
               shm_names[idx].c_str(), getpid());
        // Do something
        sleep(1);
        // Release the lock
        pthread_mutex_unlock(&(shm->mutex));
        printf("Release lock from %s by pid = %d\n", shm_names[idx].c_str(),
               getpid());
      }
      printf("Out pid = %d\n", pid);
    } else {
      printf("Spawned a child process with pid = %d\n", pid);
    }
  }

  // Wait until all children exit
  // wait(nullptr);

  // Get the exit status of all children
  int status;
  while (wait(&status) != -1) {
    if (WIFEXITED(status)) {
      printf("Child process exited with status %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
      printf("Child process exited due to signal %d\n", WTERMSIG(status));
    } else if (WIFSTOPPED(status)) {
      printf("Child process stopped due to signal %d\n", WSTOPSIG(status));
    } else if (WIFCONTINUED(status)) {
      printf("Child process continued\n");
    }
  }

  // Clean up
  for (auto &shm : shms) {
    munmap(shm->addr_, kShmSize);
    close(shm->fd_);
  }

  exit(0);
}
