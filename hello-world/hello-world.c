#include <linux/module.h>     /* Needed by all modules */ 
#include <linux/kernel.h>     /* Needed for KERN_INFO */ 
#include <linux/init.h>       /* Needed for the macros */ 
  
///< The license type -- this affects runtime behavior 
MODULE_LICENSE("GPL"); 
  
///< The author -- visible when you use modinfo 
MODULE_AUTHOR("LooseR"); 
  
MODULE_DESCRIPTION("A simple Hello world LKM!"); 
  
MODULE_VERSION("v0.0"); 
  
static int __init hello_init(void) 
{ 
    printk(KERN_INFO "Hello world initialised\n"); 
    return 0; 
} 
  
static void __exit hello_exit(void) 
{ 
    printk(KERN_INFO "Exiting hello-world\n"); 
} 
  
module_init(hello_init); 
module_exit(hello_exit); 
