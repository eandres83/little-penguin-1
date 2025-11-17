// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define MY_LOGIN "eandres"
#define MY_LOGIN_LEN (sizeof(MY_LOGIN) - 1)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("eandres");
MODULE_DESCRIPTION("Misc Device");

static ssize_t my_device_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
	// Check the offset: if it's not 0, we've already sent the data.
	if (*offset > 0)
		return 0;

	// Copy our login string from kernelspace to the user's buffer
	if (copy_to_user(buf, MY_LOGIN, MY_LOGIN_LEN))
		return -EFAULT; // Error al copiar

	*offset += MY_LOGIN_LEN;
	return MY_LOGIN_LEN;
}

static ssize_t my_device_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
	// We cannot trust 'buf' directly as it's a userspace pointer.
	// We must copy it to a trusted kernel buffer first.

	char kernel_buf[32];

	// Protect our small kernel buffer from overflow
	if (len >= sizeof(kernel_buf))
		return -EINVAL; // Invalid argument

	// Safely copy data from userspace
	if (copy_from_user(kernel_buf, buf, len))
		return -EFAULT; // Error al copiar

	kernel_buf[len] = '\0';

	// Compare the recived string with out login
	if (len == MY_LOGIN_LEN && strncmp(kernel_buf, MY_LOGIN, len) == 0) {
		pr_info("Correct login!\n");
		return len;
	}

	pr_err("Incorrect login. Recived: %s", kernel_buf);
	return -EINVAL; // Invalid value

}

static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = my_device_read,
	.write = my_device_write,
};

static struct miscdevice fortytwo_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &my_fops,
};

static int	__init my_module_init(void)
{
	int error;

	pr_info("Loading fortytwo module...\n");

	error = misc_register(&fortytwo_device);
	if (error) {
		pr_err("Failed to register misc device\n");
		return error;
	}
	pr_info("Misc device /dev/fortytwo created.\n");

	return 0;
}

static void	__exit my_module_exit(void)
{
	pr_info("Cleaning up module.\n");
	misc_deregister(&fortytwo_device);
}

module_init(my_module_init);
module_exit(my_module_exit);
