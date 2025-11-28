// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/nsproxy.h>
#include <linux/ns_common.h>
#include <../fs/mount.h>

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
		seq_printf(m, "%-15%s\t", mnt->mnt_devname, p);
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
	proc_create("mymounts", 0666, NULL, &mymounts_fops);
	return (0);
}

static void __exit proc_exit(void)
{
	remove_proc_entry("mymounts", NULL);
}

module_init(proc_init);
module_exit(proc_exit);
