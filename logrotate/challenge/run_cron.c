#define _GNU_SOURCE
#include <unistd.h>
#include <err.h>

int main(int argc, char *argv[]) {
  if (setresuid(0, 0, 0)) {
    err(1, "setresuid");
  }
  execl("/bin/run-parts", "run-parts", "--regex", ".*", "/etc/cron.d", NULL);
  return 1;
}

