// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#define MY_LOGIN "eandres"
#define MY_LOGIN_LEN (sizeof(MY_LOGIN) - 1)

MODULE_LICENSE("GPL");

static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = my_device_read,
	.write = my_device_write,
};

static const struct file_operations fortytwo_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &my_fops,
};

static ssize_t my_device_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
	pr_info("eandres");
	return (0);
}

static ssize_t my_device_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
	// if (strcmp(buf, "eandres") == 0) No se puede hacer esto porque el kernel no se puede
	// fiar de algo que viene del userspace.

	char kernel_buf[32];

	if (len >= sizeof(kernel_buf))
		return -EINVAL;

	if (copy_from_user(kernel_buf, buf, len))
		return -EFAULT; // Error al copiar

	kernel_buf[len] = '\0';

	if (len == MY_LOGIN_LEN && strncmp(kernel_buf, MY_LOGIN, len) == 0) {
		pr_info("Login correcto!\n");
		return len;
	}

	pr_err("Login incorrecto. Se recibio: %s\n", kernel_buf);
	return -EINVAL; // Invalid value

}

static int	__init my_module_init(void)
{
	int error;

	pr_info("Cargando el modulo fortytwo ...\n");

	error = misc_register(&fortytwo_device);
	if (error) {
		pr_err("Error al registrar el misc device\n");
		return error;
	}
	pr_info("Misc device /dev/fortytwo creado.\n");

	return 0;
}

static void	__exit my_module_exit(void)
{
	pr_info("Cleaning up module.\n");
	misc_deregister(&fortytwo_device);
}

module_init(my_module_init);
module_exit(my_module_exit);
