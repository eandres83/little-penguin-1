// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/debugfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("eandres");
MODULE_DESCRIPTION("Debugfs");

static struct dentry *dir = NULL;

static ssize_t my_read(struct file *f, char __user *user_buf, size_t count, loff_t *f_pos)
{
	
}

static ssize_t my_write(struct file *f, char __user *user_buf, size_t count, loff_t *f_pos)
{

}

static struct file_operations read_and_write = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
};

static int	create_dir_and_file()
{
	dir = debugfs_create_dir("fortytwo", NULL);
	if (IS_ERR(dir))
	{
		pr_info("Failure to create directory");
		return 0;
	}

	struct dentry *file_id = debugfs_create_file("id", 0666, dir, NULL, &read_and_write)
	if (IS_ERR(filie_id))
	{
		pr_infor("Failure to create file");
		return 0;
	}
}

static int	__init my_module_init(void)
{
	if (create_dir_and_file() == 0)
		return PTR_ERR;
	return 0;
}

static void	__exit my_module_exit(void)
{
	pr_info("Cleaning up module.\n");
	if (dir)
		debugfs_remove_recursive(dir);
}

module_init(my_module_init);
module_exit(my_module_exit);
