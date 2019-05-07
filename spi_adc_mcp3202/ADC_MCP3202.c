/* ************************************************************************
 * Created by    : Rahul Kumar Nonia
 * File name     : ADC_MCP3202.c
 * Created on    : 07-05-2019
 * Last modified : Tuesday 07 May 2019 06:29:43 PM IST
 * Description   : 
 * ***********************************************************************/


#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/err.h>
#include <linux/spi/spi.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/uaccess.h>

#define NR_INSTANCE 1
#define MINOR_NR 5

// global variables
char* devName = "SPI_ADC_MCP3202";
dev_t devno;
struct cdev* spi_ch_dev;
static struct class* spi_ch_class;
static struct device* spi_dev;

static const struct of_device_id SPI_ADC_ID[] = {
	{.compatible = "SPI_ADC_MCP3202",},
	{ }
};
MODULE_DEVICE_TABLE(of, SPI_ADC_ID);

// spi private data structure
struct mcp3202_private{
	struct spi_device* spi_dev;
	struct device* dev;
	struct mutex mutex_lock;
};
struct mcp3202_private* mcp3202_priv;

// open function 
int mcp3202_open(struct inode* inode, struct file* fp)
{
	 printk(KERN_INFO "file open successful\n");
	 return 0;
}

// release function
int mcp3202_release(struct inode* inode, struct file* fp)
{
	 printk(KERN_INFO "file released successful\n");
	 return 0;
}

// read function
ssize_t mcp3202_read(struct file* fp, char __user* buf, size_t count, loff_t* pos)
{
	unsigned char txbuf[3] = {0x01, 0xA0, 0x00};
	unsigned char rxbuf[3] = {0x00, 0x00, 0x00};
	int status;
	int result = 0;
	char* ptr = (char*)&result;
	
	struct spi_device* spi_dev = mcp3202_priv->spi_dev;

	struct spi_transfer txinfo = {
		.tx_buf = txbuf,
		.rx_buf = rxbuf,
		.len = 3,         // in bytes
		.bits_per_word = 8,
	};

	struct spi_message ms;

	spi_message_init(&ms);
	spi_message_add_tail(&txinfo, &ms);

	status = spi_sync(spi_dev, &ms);

	if(status == 0){
		*ptr = rxbuf[2];
		ptr++;
		*ptr = rxbuf[1];
		result = result & 0x00000FFF;

		status = copy_to_user((int*)buf, &result, sizeof(int));
	}

	return 0;
}

// files operation structure 
struct file_operations mcp3202_ops = {
	.owner = THIS_MODULE,
	.open = mcp3202_open,
	.release = mcp3202_release,
	.read = mcp3202_read,
};

// probe function
static int mcp3202_probe(struct spi_device* spi_dev)
{
	mcp3202_priv  = kzalloc(sizeof(mcp3202_priv), GFP_KERNEL);
	if(!mcp3202_priv){
		return -ENOMEM;
	}

	mcp3202_priv->spi_dev = spi_dev;
	mcp3202_priv->dev = &spi_dev->dev;
	mutex_init(&mcp3202_priv->mutex_lock);
	spi_set_drvdata(spi_dev, mcp3202_priv);

	return 0;
}

// remove function
static int mcp3202_remove(struct spi_device* spi_dev)
{
	mcp3202_priv = spi_get_drvdata(spi_dev);
	kfree(mcp3202_priv);
	return 0;
}

// spi driver structure
struct spi_driver SPIdriver = {
	.driver = {
		.name = "SPI_ADC_MCP3202",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(SPI_ADC_ID),
	},
	.probe = mcp3202_probe,
	.remove = mcp3202_remove,
};

// init module
int __init mcp3202_init(void)
{
	int ret = -ENODEV;
	int status;
	
	// dynamic allocation of major number
	status = alloc_chrdev_region(&devno, MINOR_NR, NR_INSTANCE, devName);
	if(status){
		printk(KERN_INFO "major number allocated: %d\n", MAJOR(devno));
	}
	else{
		printk(KERN_INFO "major number allocation failed.\n");
		goto err_major;
	}
	
	// dynamic allocation for character device
	spi_ch_dev = cdev_alloc();
	if(spi_ch_dev){
		printk(KERN_INFO "cdev_alloc successful.\n");
	}
	else{
		printk(KERN_INFO "cdev_allocation failed.\n");
		goto err_cdev_alloc;
	}
	// initialise device with file operations
	cdev_init(spi_ch_dev, &mcp3202_ops); 
	spi_ch_dev->owner = THIS_MODULE;

	// adding device
	status =  cdev_add(spi_ch_dev, devno, NR_INSTANCE);
	if(status){
		printk(KERN_INFO "device added successfuly.\n");
	}
	else{
		printk(KERN_INFO "failed to add the device.\n");
		goto err_cdev_add;
	}

	// creating class and entry in sysfs
	spi_ch_class = class_create(THIS_MODULE, devName);
	if(IS_ERR(spi_ch_class)){
		printk(KERN_INFO "failed to create class.\n");
		goto err_class_create;
	}
	else{
		printk(KERN_INFO "class created successfuly.\n");
	}

	// creating a devie entry in /dev directory
	spi_dev = device_create(spi_ch_class, NULL, devno, NULL, devName);
	if(IS_ERR(spi_ch_dev)){
		printk(KERN_INFO "failed to create device.\n");
		goto err_device_create;
	}
	else{
		printk(KERN_INFO "device created successfuly.\n");
	}
	
	// register spi drver
	status = spi_register_driver(&SPIdriver);
	if(status){
		printk(KERN_INFO "spi driver registered successfuly.\n");
	}
	else{
		printk(KERN_INFO "spi driver registeration failed.\n");
		goto err_spi_register;
	}

	return 0;

	err_cdev_alloc:
		unregister_chrdev_region(devno, NR_INSTANCE);

	err_cdev_add:
		kfree(spi_ch_dev);

	err_class_create:
		class_destroy(spi_ch_class);

	err_device_create:
		cdev_del(spi_ch_dev);

	err_spi_register:
		device_destroy(spi_ch_class, devno);
	
	err_major:
		return ret;
}	

// exit module
void __exit mcp3202_exit(void)
{
	spi_unregister_driver(&SPIdriver);
	device_destroy(spi_ch_class, devno);
	class_destroy(spi_ch_class);
	cdev_del(spi_ch_dev);
	unregister_chrdev_region(devno, NR_INSTANCE);
}

module_init(mcp3202_init);
module_exit(mcp3202_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("noOne");
MODULE_DESCRIPTION("SPI driver implementation for ADC_MCP3202");
