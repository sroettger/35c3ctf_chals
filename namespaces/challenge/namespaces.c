#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/mount.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <linux/memfd.h>
#include <linux/limits.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>

const char BANNER[] = "A new namespace challenge by popular demand!\n";
const char MENU[] = "\
What would you like to do?\n\
1) Start sandbox\n\
2) Run ELF\n\
3) Exit\n\
> \
";

long check(long res, const char *msg) {
  if (res == -1) {
    err(1, "%s", msg);
  }
  return res;
}

void mk_chroot_dir(const char *path) {
  check(mkdir(path, 0000), "mkdir");
  check(chmod(path, 0777), "chmod");
}

void init() {
  mk_chroot_dir("/tmp/chroots");
}

void read_line(char *buf, size_t buf_sz) {
  while (1) {
    if (!buf_sz) {
      errx(1, "newline expected");
    }
    if (read(0, buf, 1) != 1) {
      err(1, "read");
    }
    if (*buf == '\n') {
      *buf = 0;
      return;
    }
    buf++;
    buf_sz--;
  }
}

int read_int() {
  char buf[16] = {0};
  read_line(buf, sizeof(buf));
  return atoi(buf);
}

int execveat(int dirfd, const char *pathname,
                    char *const argv[], char *const envp[],
                    int flags) {
  return syscall(SYS_execveat, dirfd, pathname, argv, envp, flags);
}

void wait_for(int fd, const char *s) {
  char buf[128];
  if (strlen(s)+1 > sizeof(buf)) {
    _exit(1);
  }
  if (check(read(fd, buf, strlen(s)+1), "read") != strlen(s)+1) {
    _exit(1);
  }
  if (strcmp(s, buf)) {
    _exit(1);
  }
}

int memfd_create(const char *name, unsigned int flags) {
  return syscall(SYS_memfd_create, name, flags);
}

#define MAX_BIN (1024*1024*10)

#define SANDBOX_CNT 10
pid_t sandboxes[SANDBOX_CNT] = {0};

#define MIN(a, b) (a < b ? a : b)
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*x))

int load_elf() {
  printf("elf len? ");
  fflush(stdout);
  unsigned bin_len = read_int();
  if (bin_len > MAX_BIN) {
    errx(1, "too long");
  }

  printf("data? ");
  fflush(stdout);

  char buf[4096] = {0};
  int exe = check(memfd_create("x", MFD_CLOEXEC), "memfd_create");
  while (bin_len) {
    size_t read_sz = check(read(0, buf, MIN(sizeof(buf), bin_len)), "read(ELF)");
    if (read_sz == 0) {
      errx(1, "short read");
    }
    if (write(exe, buf, read_sz) != read_sz) {
      errx(1, "short write");
    }
    bin_len -= read_sz;
  }
  return exe;
}

void write_proc(pid_t pid, const char *fname, const char *data) {
  char path[PATH_MAX] = "";
  snprintf(path, sizeof(path), "/proc/%d/%s", pid, fname);
  int fd = check(open(path, O_WRONLY), "open xidmap");
  if (write(fd, data, strlen(data)) != strlen(data)) {
    err(1, "write");
  }
  close(fd);
}

pid_t new_proc() {
  long ret = syscall(SYS_clone, CLONE_NEWUSER|CLONE_NEWPID|CLONE_NEWNS|CLONE_NEWIPC|CLONE_NEWUTS|CLONE_NEWNET|CLONE_NEWCGROUP, 0, 0, 0, 0);
  if (ret == -1) {
    errx(1, "clone");
  }
  return (pid_t) ret;
}

void start_sandbox() {
  pid_t *sandbox = 0;
  for (int i = 0; i < SANDBOX_CNT; i++) {
    if (!sandboxes[i]) {
      sandbox = &sandboxes[i];
      break;
    }
  }
  if (!sandbox) {
    errx(1, "too many sandboxes");
  }

  int socks[2] = {0};
  check(socketpair(AF_LOCAL, SOCK_STREAM, 0, socks), "socketpair");

  pid_t pid = new_proc();
  if (!pid) {
    close(socks[0]);

    write(socks[1], "1", 2);
    wait_for(socks[1], "2");

    puts("Please send me an init ELF.");
    int init = load_elf();

    char chroot_dir[PATH_MAX] = "";
    snprintf(chroot_dir, sizeof(chroot_dir), "/tmp/chroots/%ld", sandbox - sandboxes);

    printf("[*] Creating chroot dir \"%s\"\n", chroot_dir);
    mk_chroot_dir(chroot_dir);

    printf("[*] Chrooting to \"%s\"\n", chroot_dir);
    check(chroot(chroot_dir), "chroot");
    check(chdir("/"), "chdir");

    puts("[*] changing group ids");
    check(setresgid(1, 1, 1), "setresgid");

    puts("[*] changing user ids");
    check(setresuid(1, 1, 1), "setresuid");

    write(socks[1], "3", 3);
    close(socks[1]);

    puts("[*] starting init");
    char * const argv[] = {"init", 0};
    execveat(init, "", argv, 0, AT_EMPTY_PATH);

    _exit(1);
  }

  close(socks[1]);
  wait_for(socks[0], "1");

  puts("[*] setgroups deny");
  write_proc(pid, "setgroups", "deny");

  puts("[*] writing uid_map");
  write_proc(pid, "uid_map", "1 1 1");

  puts("[*] writing gid_map");
  write_proc(pid, "gid_map", "1 1 1");

  write(socks[0], "2", 2);
  wait_for(socks[0], "3");
  close(socks[0]);
  *sandbox = pid;
}

pid_t *get_sandbox() {
  printf("which sandbox? ");
  fflush(stdout);
  int idx = read_int();
  if (idx < 0 || idx >= SANDBOX_CNT) {
    errx(1, "out of bounds");
  }
  if (!sandboxes[idx]) {
    errx(1, "no such sandbox");
  }
  return &sandboxes[idx];
}

void stop_sandbox() {
  pid_t *sandbox = get_sandbox();
  check(kill(*sandbox, SIGKILL), "kill");
  *sandbox = 0;
}

const char *NSS[] = {
  "user",
  "mnt",
  "pid",
  "uts",
  "ipc",
  "cgroup",
};

void change_ns(pid_t sandbox, int sandbox_idx) {
  printf("[*] entering namespaces of pid %d\n", sandbox);

  for (int i = 0; i < ARRAY_SIZE(NSS); i++) {
    char fname[PATH_MAX];
    snprintf(fname, sizeof(fname), "/proc/%d/ns/%s", sandbox, NSS[i]);
    int ns_fd = check(open(fname, O_RDONLY), "open(ns)");

    check(setns(ns_fd, 0), "setns");

    if (!strcmp(NSS[i], "pid")) {
      if (check(fork(), "fork")) {
        _exit(0);
      }
    }

    close(ns_fd);
  }

  char chroot_dir[PATH_MAX] = "";
  snprintf(chroot_dir, sizeof(chroot_dir), "/tmp/chroots/%d", sandbox_idx);
  check(chroot(chroot_dir), "chroot");
  check(chdir("/"), "chdir");

  check(setresgid(1, 1, 1), "setresgid");
  check(setresuid(1, 1, 1), "setresuid");
}

void run_elf() {
  pid_t *sandbox_ptr = get_sandbox();
  pid_t sandbox = *sandbox_ptr;
  int sandbox_idx = sandbox_ptr - sandboxes;
  int fd = load_elf();
  pid_t pid = check(fork(), "fork");
  if (!pid) {
    change_ns(sandbox, sandbox_idx);
    char * const argv[] = {"bin", 0};
    execveat(fd, "", argv, 0, AT_EMPTY_PATH);
    _exit(1);
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  if (getuid()) {
    fputs("run me as root", stderr);
    return 1;
  }

  init();
  puts(BANNER);
  while (1) {
    printf(MENU);
    fflush(stdout);
    switch (read_int()) {
      case 1:
        start_sandbox();
        break;
      case 2:
        run_elf();
        break;
      default:
        puts("Bye");
        _exit(0);
    }
  }
  return 0;
}
