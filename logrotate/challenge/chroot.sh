#!/bin/bash

set -Eeuxo pipefail

echo 'Setting up chroot.'
cp -R --preserve=mode /skel /tmp/
mount -o bind /proc /tmp/skel/proc
touch /tmp/skel/dev/null
touch /tmp/skel/dev/zero
touch /tmp/skel/dev/random
touch /tmp/skel/dev/urandom
mount -o bind /dev/null /tmp/skel/dev/null
mount -o bind /dev/zero /tmp/skel/dev/zero
mount -o bind /dev/random /tmp/skel/dev/random
mount -o bind /dev/urandom /tmp/skel/dev/urandom

cp -R --preserve=mode /home/user /tmp/skel/home/
chmod u+s /tmp/skel/home/user/run_cron

cp --preserve=mode /tmp/skel/etc/cron.daily/logrotate /tmp/skel/etc/cron.d/
cp --preserve=mode /home/user/pwnme /tmp/skel/etc/logrotate.d/

cp /flag /tmp/skel/
chmod 400 /tmp/skel/flag

echo 'Setup done.'
exec /usr/sbin/chroot /tmp/skel /home/user/unpriv
