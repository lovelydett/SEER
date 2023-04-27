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
#include <random>

static std::default_random_engine random_engine;
int random_int() {
    return random_engine();
}

enum class State { kNone = 0, kIdle };

const int N = 1000;
double A[N][N], B[N][N], C[N][N];

bool check_job_ready() {
    return random_int() % 100 < 50;
}

bool check_preempt_ready() {
    return random_int() % 100 < 80;
}

void workload() {
  for (int i = 0; i < N; ++i) {
    for (int k = 0; k < N; ++k) {
      for (int j = 0; j < N; ++j) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

bool receive_preempt_cmd() {
    return random_int() % 100 < 20;
}

void job() {
    puts("Doing original job.");
    workload();
    puts("Finish original job.");
}

void preemptable() {
    puts("Doing preempt job.");
    workload();
    puts("Finish preempt job.");
}

void do_schedule() {
    return;
}

static ucontext_t main_loop_context;
timer_t timer;
sigevent_t sigevent;
struct itimerspec its;

// This callback should be very clean and tiny.
void timer_callback(int sig) {
    puts("Checking whether to preempt");
    if (receive_preempt_cmd()) {
        puts("Preempting the current running preemptable job");
        // If ready to preempt, turn off timer.
        its.it_value.tv_sec = 100000000;
        timer_settime(timer, 0, &its, NULL);
        setcontext(&main_loop_context);
    }
}

void worker() {
    memset(&sigevent, 0, sizeof(sigevent));

    // Prepare signal event
    sigevent.sigev_notify = SIGEV_SIGNAL;
    sigevent.sigev_signo = SIGALRM;
    sigevent.sigev_value.sival_ptr = &timer;

    timer_create(CLOCK_REALTIME, &sigevent, &timer);
    its.it_value.tv_sec = 10000000; // This means turning off
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 10 * 1000 * 1000; // Period set to 10ms
    ; // Interval is 10ms
    timer_settime(timer, 0, &its, NULL);
    // Set up the timer callback
    signal(SIGALRM, timer_callback);

    bool is_init = false;

    // Loop in the following states:
    while (true) {
        if (!is_init) {
            getcontext(&main_loop_context);
            is_init = true;
        }
        // 1. Check readiness of job()
        if (check_job_ready()) {
            // 2. If ready, turn off timer and do job()
            its.it_value.tv_sec = 100000000;
            timer_settime(timer, 0, &its, NULL);
            job();
        } else if (check_preempt_ready()) {
            // 3. Else if job is not ready and preemptable is ready, turn on timer and do it.
            its.it_value.tv_sec = 0;
            its.it_value.tv_nsec = 10 * 1000 * 1000;
            timer_settime(timer, 0, &its, NULL);
            preemptable();
            its.it_value.tv_sec = 100000000;
        } else {
            // 4. No job no preemtable, do schedule
            // Note that the lock for the scheduler should be non-block
            do_schedule();
        }
    }
}

int main() {
  worker();
  return 0;
}