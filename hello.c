/* Kernel Programming */

#define MODULE
#define LINUX
#define __KERNEL__

#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */


int init_module(void) {
    printk("Hello world.\n");
    // A non 0 return means init_module failed; module can't be loaded.
    return 0;
}


void cleanup_module(void) {
    printk(KERN_ALERT "Goodbye world.\n");
}


MODULE_LICENSE("GPL");
