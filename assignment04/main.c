// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/mod_devicetable.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("USB Keyboard Detector for Assignment 04");

static struct usb_device_id my_usb_id_table[] = {
	{
		USB_INTERFACE_INFO(0x03, 0x00, 0x00)
	},
	{ }

};

MODULE_DEVICE_TABLE(usb, my_usb_id_table);

static int my_usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	printk(KERN_DEBUG "USB Detector: Usb CONNECTED\n");
	return 0;
}

static void my_usb_disconnect(struct usb_interface *interface)
{
	printk(KERN_DEBUG "USB Detector: Usb DISCONNECTED\n");
}

static struct usb_driver my_usb_driver = {
	.name = "my_usb_keyboard_driver",
	.id_table = my_usb_id_table,
	.probe = my_usb_probe,
	.disconnect = my_usb_disconnect,
};

static int	__init usb_driver_init(void)
{
	int result;

	result = usb_register(&my_usb_driver);
	if (result)
		printk(KERN_DEBUG "USB Detector: Driver registration failed. Errorr %d\n", result);
	else
		printk(KERN_DEBUG "USB Detector: Driver registered.\n");
	return result;
}

static void	__exit usb_driver_exit(void)
{
	usb_deregister(&my_usb_driver);
	printk(KERN_DEBUG "Cleaning up module.\n");
}

module_init(usb_driver_init);
module_exit(usb_driver_exit);
