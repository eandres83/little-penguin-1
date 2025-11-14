// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/mod_devicetable.h>

MODULE_LICENSE("GPL");

static struct usb_device_id my_usb_id_table[] = {
	{ USB_DEVICE(0x80ee, 0x0021) },

	{ }

};

MODULE_DEVICE_TABLE(usb, my_usb_id_table);

static int my_usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	printk(KERN_DEBUG "USB Detector: !Se ha CONECTADO el raton");
	return -ENODEV;
}

static void my_usb_disconnect(struct usb_interface *interface)
{
	printk(KERN_DEBUG "USB Detector: !Se ha DESCONECTADO el raton");
}

static struct usb_driver my_usb_driver = {
	.name = "usb_driver_kernel",
	.id_table = my_usb_id_table,
	.probe = my_usb_probe,
	.disconnect = my_usb_disconnect,
};

static int	__init usb_driver_init(void)
{
	int result;

	result = usb_register(&my_usb_driver);
	if (result)
		printk(KERN_DEBUG "USB Detector: Fallo el registro del driver.\n");
	else
		printk(KERN_DEBUG "USB Detector: Driver registrado.\n");
	return result;
}

static void	__exit usb_driver_exit(void)
{
	usb_deregister(&my_usb_driver);
	pr_info("Cleaning up module.\n");
}

module_init(usb_driver_init);
module_exit(usb_driver_exit);
