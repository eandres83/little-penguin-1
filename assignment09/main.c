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

struct mnt_namespace {
	atomic_t				count;
	struct ns_common		ns;
	struct mount			*root;
	struct list_head		list; // <--- El campo que buscabas
	struct user_namespace   *user_ns;
	struct ucounts		  *ucounts;
	u64					 seq;
};

/* * Definimos struct mount. 
 * IMPORTANTE: Esta estructura es GIGANTE. Solo definimos el principio 
 * para acceder a mnt_mountpoint y mnt_list.
 * Para acceder a mnt_devname, necesitamos trucos o la struct completa.
 * Esta versión simplificada suele funcionar para los primeros campos.
 */
struct mount {
	struct hlist_node mnt_hash;
	struct mount *mnt_parent;
	struct dentry *mnt_mountpoint;
	struct mount *mnt_master;
	struct mount *mnt_slave;
	struct mount *mnt_share;
	struct list_head mnt_slave_list;
	struct list_head mnt_share_list;
	struct list_head mnt_expire;
	struct list_head mnt_share;
	struct list_head mnt_list;
	struct list_head mnt_expire_list;
	struct list_head mnt_share_list;
	struct list_head mnt_slave_list;
	struct list_head mnt_list_all;
	struct mount *mnt_ex_mountpoint;
	struct fs_struct *mnt_fs;
	/* * OJO: mnt_devname suele estar MUY abajo en la estructura. 
	 * Acceder a él directamente con una estructura incompleta causará SEGFAULT.
	 * Para este ejercicio, intenta acceder a él a través del superbloque si es posible,
	 * o usa esta definición extendida "a ojo":
	 */
	 int mnt_id;
	 int mnt_group_id;
	 int mnt_expiry_mark;
	 int mnt_flags;
	 /* ... mucho relleno aquí ... */
	 /* Si falla, borra el acceso a mnt_devname en tu código */
	 const char *mnt_devname; 
};

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
		seq_printf(m, "%-15s\t%s\n", mnt->mnt_devname ? mnt->mnt_devname : "none", p);
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
