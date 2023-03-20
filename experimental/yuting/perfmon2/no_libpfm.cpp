#define _GNU_SOURCE
#include <asm/unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <linux/hw_breakpoint.h>
#include <linux/perf_event.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <unistd.h>

struct read_format {
  uint64_t nr;
  struct {
    uint64_t value;
    uint64_t id;
  } values[];
};

static const int M = 1000;
static double A[M][M];
static double B[M][M];
static double C[M][M];
void matmul1() {
  printf("matmul1 begins\n");
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      for (int k = 0; k < M; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  printf("matmul1 finishes\n");
}

int main(int argc, char *argv[]) {
  struct perf_event_attr pea;
  int fd1, fd2, fd3;
  uint64_t id1, id2, id3;
  uint64_t val1, val2, val3;
  char buf[4096];
  struct read_format *rf = (struct read_format *)buf;
  int i;

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_HARDWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_HW_CPU_CYCLES;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd1 = syscall(__NR_perf_event_open, &pea, 0, -1, -1, 0);
  ioctl(fd1, PERF_EVENT_IOC_ID, &id1);

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_SOFTWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_SW_PAGE_FAULTS;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd2 = syscall(__NR_perf_event_open, &pea, 0, -1, fd1 /*!!!*/, 0);
  ioctl(fd2, PERF_EVENT_IOC_ID, &id2);

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_SOFTWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_HW_INSTRUCTIONS;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd3 = syscall(__NR_perf_event_open, &pea, 0, -1, fd1 /*!!!*/, 0);
  ioctl(fd3, PERF_EVENT_IOC_ID, &id3);

  ioctl(fd1, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
  ioctl(fd1, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
  matmul1();
  ioctl(fd1, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

  read(fd1, buf, sizeof(buf));
  for (i = 0; i < rf->nr; i++) {
    if (rf->values[i].id == id1) {
      val1 = rf->values[i].value;
    } else if (rf->values[i].id == id2) {
      val2 = rf->values[i].value;
    } else if (rf->values[i].id == id3) {
      val3 = rf->values[i].value;
    }
  }

  printf("cpu cycles: %" PRIu64 "\n", val1);
  printf("page faults: %" PRIu64 "\n", val2);
  printf("instructions: %" PRIu64 "\n", val3);
  printf("utilization: %.2f%%\n", (double)val3 / val1 * 100);

  return 0;
}