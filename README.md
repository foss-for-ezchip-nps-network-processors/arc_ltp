# Linux Test Project for ARC Linux

For details about LTP see README.ltp and http://ltp.sourceforge.net/
For details about ARC processors see http://www.synopsys.com/dw/ipdir.php?ds=arc_770d

Not all tests from LTP suite are applicable to ARC Linux which is an embedded
system based on uClibc and BusyBox. So some of the tests need to be disabled in
order for LTP to build and run succesfully.


## Build instructions

```
git clone git://github.com/anthony-kolesov/arc_ltp.git
cd arc_ltp
make autotools
./configure --host=arc-linux-uclibc CC=arc-linux-uclibc-gcc --enable-arc-support CFLAGS="-Dlinux -DUCLINUX"
make
sudo make install
```

If you want to make out-of-tree then consult `INSTALL` for details and possible issues.


## Run on target

```
[ARCLinux] $ cd arc_ltp
[ARCLinux] $ ./runltp
```

Only `runltp` configuration is supported. `runalltests.sh` and `runltplite.sh` will probably
run some tests that may crash or hang target system or take too much time to finish.


## Modifications to original LTP

### Tests that cannot be built

These tests cannot be build by ARC Linux uClibc Toolchain (as of version 4.4). To disable all of
them at once provide --enable-arc-support flag to ./configure command.

* controllers/cpuset
* security/tomoyo
* kernel/syscalls/clock_nanosleep (also must be disable in ./runtest/syscalls)
* kernel/syscalls/getdents - obscolete syscall, not available on ARC
* kernel/syscalls/sysctl - obscolete syscall, not available on ARC
* kernel/syscalls/ustat - obscolete syscall, not available on ARC.


### Tests with changed parameters

These tests can be run on ARC Linux but require are different from default
parameters because they either take too much time to execute or use values that
are too big for an embedded system.

* math/float_bessel - with default parameters take too much time to finish
* math/float_exp_log - with default parameters take too much time to finish
* math/float_iperb - with default parameters take too much time to finish
* math/float_power - with default parameters take too much time to finish
* math/float_trigo - with default parameters take too much time to finish
* syscalls/fork13 - change value of -i from 1000000 to 10000 so it will take
  bearable amount of time to complete	
* sched/hackbench01 - reduce number of iterations to reduce run time. Reduce
  number of processes because ARC doesn't support more than 4096 processes at
  one time.
* sched/hackbench02 - ARC Linux has problems if amount of threads in one
  process is more that 380.


### Fatal tests

These tests cause system to crash or hung so they must be disabled.

* syscalls/sendfile04 - it is required to find why this tests causes system to hang
* syscalls/sendfile04_64 - it is required to find why this tests causes system to hang.


### Not applicable tests

These tests are not applicable to ARC Linux but are reported as failed, so they
should be disabled.

* admin_tools/su01 - requires `useradd/userdel` commands
* admin_tools/cron02 - requires `useradd/userdel` commands
* admin_tools/cron_deny01 - requires `useradd/userdel` commands
* admin_tools/cron_allow01 - requires `useradd/userdel` commands
* admin_tools/cron_dirs_checks - cron isn't available
* admin_tools/acl_test01 - requires `mkfs` command. `mke2fs` can be used
  instead but then it fails due to lack of `useradd`
* commands/ar - command is unsupported on ARC Linux
* commands/ld - command is unsupported on ARC Linux
* commands/ldd - command is unsupported on ARC Linux
* commands/nm - command is unsupported on ARC Linux
* commands/objdump - command is unsupported on ARC Linux
* commands/file - command is unsupported on ARC Linux
* commands/tar - test uses `file` command which is unsupported on ARC Linux
* commands/cron - command is unsupported on ARC Linux
* commands/logrotate - command is unsupported on ARC Linux
* commands/cpio - test uses `file` command which is unsupported on ARC Linux
* commands/gzip01 - gzip in BusyBox doesn't support option `-r`
* controllers/cgroup
* controllers/controllers
* fs/rwtest01 - this test requires BASH which isn't available
* fs/rwtest02 - this test requires BASH which isn't available
* fs/rwtest03 - this test requires BASH which isn't available
* fs/rwtest04 - this test requires BASH which isn't available
* fs/rwtest05 - this test requires BASH which isn't available
* fs/iogen01 - this test requires BASH which isn't available
* fs_bind/BindMounts - this test requires BASH which isn't available
* io/aio01 - asynchronous IO isn't supported by uClibc
* io/aio02 - asynchronous IO isn't supported by uClibc
* mm/mtest06_2 - test requires 1 GiB of RAM but board has only 512 MiB
* numa/Numa-testcases - test requires command `numactl`


## Notes

* commands/unzip01 - tests expect different output from `unzip` however it is
  just different words with same meaning so they cn be safely replaces in test
* fs/fs_racer* - these tests are written for bash which isn't available however
  they doesn't use syntax unsupported by BusyBox so they run nicely if `bash`
  is replaced with `sh`.
* mm/mmapstress06 - to pass this test requires 512 MiB RAM on board. So either
  ARC Linux kernel image must be compiled with 512 MiB RAM or
  ANON_GRAN_PAGES_MAX in test source code must be changed to 16U instead of 32U.
* sched/pth_str02 - this test will fail with current paramter "-n 1000" until
  STAR 9000579074 will be fixed
* syscalls/acct01 - will fail if BSD process accounting isn't enabled. Test
  source has been changes so test won't run if it isn't applicable.
* syscalls/epoll_create1_01 - epoll is disabled in ARC Linux by default but can
  be switched on.
* syscalls/epoll01 - epoll is disabled in ARC Linux by default but can be
  switched on.
* syscalls/ioctl03 - requires /dev/net/tun device.
* several shell scripts in Open POSIX test suite start with empty line. These
  need to be removed so bash bang will be on the first line.

