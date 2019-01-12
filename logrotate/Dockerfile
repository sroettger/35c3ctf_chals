FROM tsuro/nsjail

COPY skel /skel
COPY challenge/chroot.sh /home/user/
COPY challenge/unpriv /home/user/
COPY challenge/pwnme /home/user/
COPY challenge/run_cron /home/user/

CMD /bin/sh -c "/usr/bin/setup_cgroups.sh && su user -c '/usr/bin/nsjail -Ml --port 1337 --chroot / -U 0:1000:1 -U 1000:100000:1 -G 0:1000:1 -G 1000:100000:1 --keep_caps --disable_no_new_privs -m none:/tmp:tmpfs:size=314572800 --cgroup_mem_max 209715200 --cgroup_pids_max 100 --cgroup_cpu_ms_per_sec 0 --rlimit_as max --rlimit_cpu max --rlimit_nofile max --rlimit_nproc max --rlimit_fsize max -- /usr/bin/stdbuf -i0 -o0 -e0 /usr/bin/maybe_pow.sh /home/user/chroot.sh'"
