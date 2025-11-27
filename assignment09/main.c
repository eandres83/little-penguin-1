// SPDX-License-Identifier: GPL-2.0

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static int my_show(struct seq_file *m, void *v)
{
	
}

static int __init proc_init(void)
{
	struct proc_dir_entry *pde;

	pde = proc_create_single("mymounts", 0, NULL, my_show);
	
}

fs_initcall(proc_init);
