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
static struct mutex foo_mutex;
static	char foo_data[4096];

static ssize_t id_read(struct file *f, char __user *user_buf, size_t count, loff_t *f_pos)
{
	char *login = "eandres\n";
	size_t len = 9;

	return simple_read_from_buffer(user_buf, count, f_pos, login, len);
}

static ssize_t id_write(struct file *f, char __user *user_buf, size_t count, loff_t *f_pos)
{
	char kbuf[32];
	ssize_t copied;
	char *my_login = "eandres";
	int login_len = 8;

	copied = simple_write_to_buffer(kbuf, sizeof(kbuf) -1, f_pos, user_buf, count);
	if (copied < 0)
		return copied;

	if (strncmp(kbuf, my_login, login_len) == 0)
		return copied;
	else
		return -EINVAL;
}

static ssize_t jiffies_read(struct file *f, char __user *user_buf, size_t count, loff_t *f_pos)
{
	// Crear un buffer temporal para guardar el texto
	char temp_buffer[32];
	int len;

	// Convertir el numbero 'jiffies' a texto
	// Guardamos el resultado en temp_buffer
	len = snprintf(temp_buffer, sizeof(temp_buffer), "%lu\n", jiffies);

	// Enviar este buffer de texto al usuario
	// simple_read_from_buffer hace la copia segura al espacio de usuario
	// y gestiona el offset (f_pos)
	return simple_read_from_buffer(user_buf, count, f_pos, temp_buffer, len);
}

static ssize_t foo_read(struct file *f, char __user *user_buf, size_t count, loff_t *f_pos)
{
	mutex_lock(&foo_mutex);
	ssize_t result = simple_write_from_buffer(user_buf, count, f_pos, fs, sizeof(foo_data));
	mutex_unlock(&foo_mutex);
	return result;
}

static ssize_t foo_write(struct file *f, char __user *user_buf, size_t count, loff_t *f_pos)
{
	mutex_lock(&foo_mutex);
	ssize_t result = simple_read_from_buffer(foo_data, sizeof(foo_data) -1, f_pos, user_buf, count);
	mutex_unlock(&foo_mutex);
	return 0;
}

static struct file_operations fops_id = {
	.owner = THIS_MODULE,
	.read = id_read,
	.write = id_write,
};

static struct file_operations fops_jiffies = {
	.owner = THIS_MODULE,
	.read = jiffies_read,
};

static struct file_operations fops_foo = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write,
};

static int	__init my_module_init(void)
{
	dir = debugfs_create_dir("fortytwo", NULL);
	if (IS_ERR(dir))
	{
		pr_info("Failure to create directory");
		return 0;
	}

	struct dentry *file_id = debugfs_create_file("id", 0666, dir, NULL, &fops_id);
	if (IS_ERR(file_id))
	{
		pr_info("Failure to create file");
		return 0;
	}

	file_id = debugfs_create_file("jiffies", 0444, dir, NULL, &fops_jiffies);
	if (IS_ERR(file_id))
	{
		pr_info("Failure to create file");
		return 0;
	}

	file_id = debugfs_create_file("foo", 0600, dir, NULL, &fops_foo);
	if (IS_ERR(file_id))
	{
		pr_info("Failure to create file");
		return 0;
	}
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
