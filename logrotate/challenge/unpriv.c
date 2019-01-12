#define _GNU_SOURCE
#include <unistd.h>
#include <err.h>
#include <sys/capability.h>

int main(int argc, char *argv[]) {
  if (setresgid(1000, 1000, 1000)) {
    err(1, "setresgid");
  }
  if (setresuid(1000, 1000, 1000)) {
    err(1, "setresgid");
  }
  cap_t caps = cap_init();
  if (cap_clear(caps)) {
    err(1, "cap_clear");
  }
  if(cap_set_proc(caps)) {
    err(1, "cap_set_proc");
  }
  execl("/bin/bash", "bash", NULL);
  return 1;
}
