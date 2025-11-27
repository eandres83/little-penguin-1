// SPDX-License-Identifier: GPL-2.0

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("eandres");
MODULE_DESCRIPTION("Module creates a file in procfs and show");

static int my_show(struct seq_file *m, void *v)
{
	struct mount *mnt;
	struct mnt_namespace *ns;

	// Obtener el namespace de montaje del proceso actual
	ns = current->nsproxy->mnt_ns;

	// Iterar sobre la lista de montajes
	list_for_each_entry(mnt, &ns->list, mnt_list) {
		// Imprimir el nombre del dispositivo y el punto de montaje
		seq_printf(m, "%-20s\t", mnt->mnt_devname ? mnt->mnt_davname : "none");
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
