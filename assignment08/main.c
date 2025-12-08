// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

// Dont hace a license, LOL
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Useless module");

static ssize_t myfd_read(struct file *fp, char __user *user,
			 size_t size, loff_t *offs);
static ssize_t myfd_write(struct file *fp, const char __user *user,
			  size_t size, loff_t *offs);

static const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write,
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops,
};

char str[PAGE_SIZE];

ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	size_t len;
	size_t i;
	ssize_t ret;
	char *tmp2;

	len = strlen(str);

	if (len == 0)
		return 0;
	tmp2 = kmalloc(len + 1, GFP_KERNEL);
	if (!tmp2)
		return -ENOMEM;
	for (i = 0; i < len; i++)
		tmp2[i] = str[len - 1 - i];
	tmp2[len] = '\0';
	ret = simple_read_from_buffer(user, size, offs, tmp2, len);
	kfree(tmp2);
	return ret;
}

ssize_t myfd_write(struct file *fp, const char __user *user, size_t size,
		   loff_t *offs)
{
	ssize_t res;

	res = 0;
	res = simple_write_to_buffer(str, PAGE_SIZE - 1, offs, user, size);
	if (res > 0)
		// 0x0 = '\0'
		str[size + 1] = 0x0;
	return res;
}

static int __init myfd_init(void)
{
	int retval;

	memset(str, 0, PAGE_SIZE);
	retval = misc_register(&myfd_device);
	return 1;
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
}

module_init(myfd_init);
module_exit(myfd_cleanup);

