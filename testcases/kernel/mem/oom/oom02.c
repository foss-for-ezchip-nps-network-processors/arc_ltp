/*
 * Out Of Memory (OOM) for NUMA
 *
 * The program is designed to cope with unpredictable like amount and
 * system physical memory, swap size and other VMM technology like KSM,
 * memcg, memory hotplug and so on which may affect the OOM
 * behaviours. It simply increase the memory consumption 3G each time
 * until all the available memory is consumed and OOM is triggered.
 *
 * Copyright (C) 2010  Red Hat, Inc.
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Further, this software is distributed without any warranty that it
 * is free of the rightful claim of any third person regarding
 * infringement or the like.  Any license provided herein, whether
 * implied or otherwise, applies only to this software file.  Patent
 * licenses, if any, provided herein do not apply to combinations of
 * this program with other software, or any other product whatsoever.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "config.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include "test.h"
#include "usctest.h"
#include "mem.h"

char *TCID = "oom02";
int TST_TOTAL = 1;

#if HAVE_NUMA_H && HAVE_LINUX_MEMPOLICY_H && HAVE_NUMAIF_H \
	&& HAVE_MPOL_CONSTANTS
int main(int argc, char *argv[])
{
	char *msg;
	int lc;

	msg = parse_opts(argc, argv, NULL, NULL);
	if (msg != NULL)
		tst_brkm(TBROK, NULL, "OPTION PARSING ERROR - %s", msg);

#if __WORDSIZE == 32
	tst_brkm(TCONF, NULL, "test is not designed for 32-bit system.");
#endif

	setup();

	for (lc = 0; TEST_LOOPING(lc); lc++) {
		Tst_count = 0;

		tst_resm(TINFO, "process mempolicy.");
		testoom(1, 0, 1);

		tst_resm(TINFO, "process cpuset.");
		testoom(0, 0, 1);
	}
	cleanup();
	tst_exit();
}

void setup(void)
{
	tst_require_root(NULL);
	tst_sig(FORK, DEF_HANDLER, cleanup);
	TEST_PAUSE;

	overcommit = get_sys_tune("overcommit_memory");
	set_sys_tune("overcommit_memory", 1, 1);
	mount_mem("cpuset", "cpuset", NULL, CPATH, CPATH_NEW);
}

void cleanup(void)
{
	set_sys_tune("overcommit_memory", overcommit, 0);
	umount_mem(CPATH, CPATH_NEW);

	TEST_CLEANUP;
}

#else /* no NUMA */
int main(void)
{
	tst_brkm(TCONF, NULL, "no NUMA development packages installed.");
}
#endif
