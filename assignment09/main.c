// SPDX-License-Identifier: GPL-2.0

#include <linux/fs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/nsproxy.h>
#include <linux/sched.h>
#include <linux/mount.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("eandres");
MODULE_DESCRIPTION("Module creates a file in procfs and show");

struct mnt_namespace {
	atomic_t count;
	struct ns_common ns;
	struct mount *root;
	struct list_head list;
};

struct mount {
	struct hlist_mode mnt_hash;
	struct mount *mnt_parent;
	struct dentry *mnt_mountpoint;
	struct vfsmount mnt;
	union {
		struct rcu_head mnt_rcu;
		struct llist_node mnt_llist;
	};
#ifdef CONFIG_SMP
	struct llist_node mnt_llist_extra;
#endif
	struct list_head mnt_list;
	struct list_head mnt_child;
	struct list_head mnt_instance;
	const char *mnt_devname;
	struct list_head mnt_share;
	struct list_head mnt_slave_list;
	struct mount *mnt_master;
	struct mnt_namespace *mnt_ns;
	struct mountpoint *mnt_mp;
};

static int my_show(struct seq_file *m, void *v)
{
	struct mount *mnt;
	struct mnt_namespace *ns;

	// Obtener el namespace de montaje del proceso actual
	ns = current->nsproxy->mnt_ns;

	// Iterar sobre la lista de montajes
	list_for_each_entry(mnt, &ns->list, mnt_list) {
		// Imprimir el nombre del dispositivo y el punto de montaje
		seq_printf(m, "%-20s\t", mnt->mnt_devname ? mnt->mnt_devname : "none");
		seq_path(m, &mnt->mnt.mnt_root->d_path, "\n");
	}

	return (0);
}

static int __init proc_init(void)
{
	struct proc_dir_entry *pde;

	pde = proc_create_single("mymounts", 0, NULL, my_show);
	if (!pde) {
		return -ENOMEM;
	}

	return (0);
}

static void __exit proc_exit(void)
{
	remove_proc_entry("mymounts", NULL);
	pr_info("Remove mymounts\n");
}

module_init(proc_init);
module_exit(proc_exit);
