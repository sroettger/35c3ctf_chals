FROM tsuro/nsjail
COPY files /home/user
#COPY tmpflag /flag
CMD /bin/sh -c "/usr/bin/setup_cgroups.sh && su user -c '/usr/bin/nsjail --config /nsjail.cfg'"
