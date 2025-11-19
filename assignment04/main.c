// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/mod_devicetable.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("eandres");
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
	pr_info("USB Detector: Usb CONNECTED\n");
	return 0;
}

static void my_usb_disconnect(struct usb_interface *interface)
{
	pr_info("USB Detector: Usb DISCONNECTED\n");
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
		pr_info("USB Detector: Driver registration failed. Error %d\n", result);
	else
		pr_info("USB Detector: Driver registered.\n");
	return result;
}

static void	__exit usb_driver_exit(void)
{
	usb_deregister(&my_usb_driver);
	pr_info("Cleaning up module.\n");
}

module_init(usb_driver_init);
module_exit(usb_driver_exit);
