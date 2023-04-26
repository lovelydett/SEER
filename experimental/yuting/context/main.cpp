//
// Created by tt on 4/26/23.
//

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <memory>
#include <thread>
#include <ucontext.h>

enum class State { kNone = 0, kIdle };

void timer_callback(int sig) {
  puts("Timer expired\n");
  // setcontext(&context);
}

const int N = 1000;
double A[N][N], B[N][N], C[N][N];

static std::function<void(void)> job;
static bool job_valid = false;

static std::function<void(void)> preempt_job;
static bool preempt_valid = false;

static ucontext_t main_context;
static ucontext_t preempt_context;

void main_loop() {
  getcontext(&main_context);
  if (job_valid) {
    // Register a timer
    timer_t timer;
    sigevent_t sigevent;
    memset(&sigevent, 0, sizeof(sigevent));

    // Prepare signal event
    sigevent.sigev_notify = SIGEV_SIGNAL;
    sigevent.sigev_signo = SIGALRM;
    sigevent.sigev_value.sival_ptr = &timer;

    timer_create(CLOCK_REALTIME, &sigevent, &timer);
    struct itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 10 * 1000 * 1000; // Starts after 10ms
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 10 * 1000 * 1000;
    ; // Interval is 10ms
    timer_settime(timer, 0, &its, NULL);
    // Set up the timer callback
    signal(SIGALRM, timer_callback);

    job();
    job_valid = false;
  }
}

void func() {
  puts("Doing job...\n");
  for (int i = 0; i < N; ++i) {
    for (int k = 0; k < N; ++k) {
      for (int j = 0; j < N; ++j) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  puts("Finish job\n");
}

ucontext_t preempt(const std::function<void(void)> job) {
  static bool is_init = false;
  static const std::function<void(void)> inner_job = job;
  ucontext_t context;
  getcontext(&context);
  if (is_init == false) {
    is_init = true;
    return context;
  }
  job();
  return context;
}

int main() {
  // Make the context of func
  ucontext_t func_context;
  getcontext(&func_context);
  func_context.uc_stack.ss_sp = malloc(1024 * 128);
  func_context.uc_stack.ss_size = 1024 * 128;
  func_context.uc_stack.ss_flags = 0;
  func_context.uc_link = &main_context;
  makecontext(&func_context, func, 0);

  return 0;
}