/* ************************************************************************
 * Created by    : Rahul Kumar Nonia
 * File name     : driver.c
 * Created on    : 15-04-2019
 * Last modified : Tuesday 16 April 2019 06:58:13 PM IST
 * Description   : 
 * ***********************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define FIRST_MINOR 100		// first minor number
#define NR_DEV 2			// number of devices

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("device driver and app");
MODULE_VERSION("v0.1");

static int dev_open(struct inode* inod, struct file* fil)
{

}

static int dev_release(struct inode* inod, struct file* fil)
{

}

static ssize_t dev_read(struct file* filep, char* buff, size_t len, loff_t* off)
{

}

static ssize_t dev_write(struct file* filep, const char* buff, size_t len, loff_t* off)
{

}

struct struct file_operations fops = 
{
	.read = dev_read;
	.write = dev_write;
	.open = dev_open;
	.release = dev_release;
};

// global variables
char* dev_name;		// contains device name
dev_t dev_no = 0;	// contains major no.(12-bit) and minor no(20-bit)
struct cdev *dev;	// holds character device driver descriptor

// class and device structures
static struct class* char_class;
static struct device* char_device; 

static int __init dev_init(void)
{
	int status;
	int ret = -ENODEV;
/*
 * static allocation of device has following parameters.
 * int register_chrdev_region(dev_t first, unsigned int count, char *name);
 */

 // dynamic allocation
 status = alloc_chrdev_region(&dev_no, FIRST_MINOR, NR_DEV, dev_name);
 if(status < 0){
	printk(KERN_INFO "device number allocation failed\n");
	goto err;
 }
 printk(KERN_INFO "Major number allocated succesfuly.\n");

 char_device = cdev_alloc(); // allocate memory for char_device
 if(char_device = NULL){
	 printk(KERN_INFO "cdev allocation failed\n");
	 goto err_char_dev_alloc;
 }

 cdev_init(char_device, &fops); 	// initialise the char_device
 char_device-> = THIS_MODULE;

 status = cdev_add(char_device, dev_no, NR_DEV);

 else{
	printk(KERN_INFO "major number allocation failed\n");
 }

}

static void __exit dev_exit(void)
{

}

module_init(dev_init);
module_exit(dev_exit);
