// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/nsproxy.h>
#include <linux/ns_common.h>
#include <linux/path.h>
#include <linux/sched.h>
#include <linux/list.h>
#include "/usr/src/kernel-6.16.1-eandres/fs/mount.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("eandres");
MODULE_DESCRIPTION("Module creates a file in procfs and show");

static int my_show(struct seq_file *m, void *v)
{
	struct mount *mnt;
	char buf[256];
	char *p;

	list_for_each_entry(mnt, &current->nsproxy->mnt_ns->list, mnt_list) {
		p = dentry_path_raw(mnt->mnt_mountpoint, buf, sizeof(buf));
		char *devname = "none";
		if (mnt->mnt_mountpoint && mnt->mnt_mountpoint->d_sb)
			devname = mnt->mnt_mountpoint->d_sb->s_id;
		seq_printf(m, "%-15s\t%s\n", devname, p);
	}

	return (0);
}

static int mymounts_open(struct inode *inode, struct file *file)
{
	return single_open(file, my_show, NULL);
}

static const struct proc_ops mymounts_fops = {
	.proc_open = mymounts_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static int __init proc_init(void)
{
	struct proc_dir_entry *mymounts;

	mymounts = proc_create("mymounts", 0666, NULL, &mymounts_fops);
	if (!mymounts)
	{
		pr_info("Error when creatin the proc device.\n");
		return -ENOMEM;
	}
	pr_info("Proc device created.\n");
	return (0);
}

static void __exit proc_exit(void)
{
	remove_proc_entry("mymounts", NULL);
	pr_info("Cleaning up the proc modules.\n");
}

module_init(proc_init);
module_exit(proc_exit);
