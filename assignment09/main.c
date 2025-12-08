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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("eandres");
MODULE_DESCRIPTION("Module creates a file in procfs and show");

struct mnt_namespace {
	struct ns_common	ns;
	struct mount		*root;
	struct list_head	list;
	struct user_namespace   *user_ns;
	struct ucounts		*ucounts;
	u64			 seq;
};

struct mount {
	struct hlist_node mnt_hash;
	struct mount *mnt_parent;
	struct dentry *mnt_mountpoint;
	struct vfsmount mnt;
	union {
		struct rb_node mnt_node; /* node in the ns->mounts rbtree */
		struct rcu_head mnt_rcu;
		struct llist_node mnt_llist;
	};
#ifdef CONFIG_SMP
	struct mnt_pcp __percpu *mnt_pcp;
#else
	int mnt_count;
	int mnt_writers;
#endif
	struct list_head mnt_mounts;	/* list of children, anchored here */
	struct list_head mnt_child;	/* and going through their mnt_child */
	struct list_head mnt_instance;	/* mount instance on sb->s_mounts */
	struct list_head mnt_list;
	struct list_head mnt_expire;	/* link in fs-specific expiry list */
	struct list_head mnt_share;	/* circular list of shared mounts */
	struct list_head mnt_slave_list;/* list of slave mounts */
	struct list_head mnt_slave;	/* slave list entry */
	struct mount *mnt_master;	/* slave is on master->mnt_slave_list */
	struct mnt_namespace *mnt_ns;	/* containing namespace */
	struct mountpoint *mnt_mp;	/* where is it mounted */
	union {
		struct hlist_node mnt_mp_list;	/* list mounts with the same mountpoint */
		struct hlist_node mnt_umount;
	};
	struct list_head mnt_umounting; /* list entry for umount propagation */
#ifdef CONFIG_FSNOTIFY
	struct fsnotify_mark_connector __rcu *mnt_fsnotify_marks;
	__u32 mnt_fsnotify_mask;
	struct list_head to_notify;	/* need to queue notification */
	struct mnt_namespace *prev_ns;	/* previous namespace (NULL if none) */
#endif
	int mnt_id;			/* mount identifier, reused */
	u64 mnt_id_unique;		/* mount ID unique until reboot */
	int mnt_group_id;		/* peer group identifier */
	int mnt_expiry_mark;		/* true if marked for expiry */
	struct hlist_head mnt_pins;
	struct hlist_head mnt_stuck_children;
} __randomize_layout;

static void print_mount_recursive(struct mount *mnt, struct seq_file *m, char *buf, int len)
{
	struct super_block *sb;
	char *p;
	struct mount *child;

	// conseguir la ruta actual y imprimirla
	p = dentry_path_raw(mnt->mnt_mountpoint, buf, len);
	if (!IS_ERR(p)) {
		sb = mnt->mnt.mnt_sb;
		if (sb) {
			if (sb->s_id[0])
				seq_printf(m, "%-15s %s\n", sb->s_id, p);
			else
				seq_printf(m, "%-15s %s\n", "none", p);
		}
	}
	// recurvidad para recorrer todos los hijos
	list_for_each_entry(child, &mnt->mnt_mounts, mnt_child)
		print_mount_recursive(child, m, buf, len);
}

static int my_show(struct seq_file *m, void *v)
{
	struct mount *root_mnt;
	char buf[256];

	root_mnt = current->nsproxy->mnt_ns->root;

	if (root_mnt)
		print_mount_recursive(root_mnt, m, buf, sizeof(buf));

	return 0;
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

	mymounts = proc_create("mymounts", 0644, NULL, &mymounts_fops);
	if (!mymounts) {
		pr_info("Error when creatin the proc device.\n");
		return -ENOMEM;
	}
	pr_info("Proc device created.\n");
	return 0;
}

static void __exit proc_exit(void)
{
	remove_proc_entry("mymounts", NULL);
	pr_info("Cleaning up the proc modules.\n");
}

module_init(proc_init);
module_exit(proc_exit);
