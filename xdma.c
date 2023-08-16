/**
 * @file   xdma.c
 * 
 * @brief  XDMA HDL-comp Linux driver code.
 * 
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>      // MKDEV
#include <linux/fs.h>          // file_operations
#include <linux/device.h>      // class_create, device_create
#include <linux/cdev.h>        // cdev_init
#include <linux/errno.h>       // EINVAL, ..
#include <linux/dma-mapping.h> // ioremap_nocache, dma_alloc_coherent, ..
#include <linux/uaccess.h>     // copy_to_user, copy_from_user

#include "xdma.h"              // IOCTL_XDMA
#include "xdma_regs.h"

//-----------------------------------------------------------------------------
#ifndef DOXYGEN_SHOULD_SKIP_THIS
 MODULE_AUTHOR("Smirnov");
 MODULE_DESCRIPTION("xdma-linux-module");
 MODULE_VERSION("1.0");
 MODULE_LICENSE("GPL");
#endif // DOXYGEN_SHOULD_SKIP_THIS

//-----------------------------------------------------------------------------
/**
 * @brief DebugPrint wrap : USER must use '-DDBGP' during compilation to enable debug-data output. \n
 *        NB!!! ERROR-data information will be OUT in any case !!!
 */
#ifdef DBGP
#define DEBUGP(...) printk(...)
#else
#define DEBUGP(...)
#endif // DBGP

//-----------------------------------------------------------------------------
/**
 * @brief Struct used to hold complete DEVICE addr region data
 * @sa xdma_open()
 */
struct dev_address_t {
    size_t physical_address; /**< Linux physical addr */
    void *virtual_address;   /**< Linux virtual addr */
    size_t size;             /**< size of occupied memory for DEVICE, used in ioremap_nocache() */
};

/**
 * @brief Struct used to hold complete DMA addr region data
 * @sa xdma_open(), XDMA_MEM_SIZE
 */
struct dma_address_t {
    dma_addr_t physical_address; /**< Linux physical addr */
    char *virtual_address;       /**< Linux virtual addr */
    size_t size;                 /**< size of occupied memory for DMA, used in dma_alloc_coherent() */
};

//-----------------------------------------------------------------------------
static dev_t g_devno = MKDEV(0, 0); /**< Device type to hold the device MAJOR/MINOR numbers for current DRIVER : STAGE#0 */
static struct class *g_cl = NULL;   /**< Pointer to 'struct class' for routine 'class_create()' output : STAGE#1 */
struct device *g_device = NULL;     /**< Pointer to 'struct device' for routine 'device_create()' output : STAGE#2 */
static struct cdev g_cdev;          /**< Kernel's internal structure that represents char devices : STAGE#3 */
// Addr
struct dev_address_t dev_address; /**< DEVICE addr region data */
struct dma_address_t dma_address; /**< DMA addr region data */


//-----------------------------------------------------------------------------
/** @brief Use to make a XDMA device-specific system call. 
 *         This function is called whenever a USER tries to do an 'ioctl' on our device file.
 *
 *  @param [in]    f    pointer to FILE structure assigned to our Linux device
 *  @param [in]    cmd  special COMMAND
 *  @param [inout] arg  pointer to an argument from USER-space: location of IN-data in case of WRITE, or location of OUT-data in case of READ
 *  @return result of READ/WRITE operation : ZERO if success, NEGATIVE otherwise
 *  @sa xdma.h
 */
static long xdma_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
    // dec vars
    unsigned int xdma_data;
    void *__user arg_ptr;
    
    // Coerce the argument as a userspace pointer
    arg_ptr = (void __user *)arg;
    
    switch(cmd) {
      
	    // XDMA_ADDRESS
        case IOCTL_XDMA_ADDRESS: {
	        xdma_data = dma_address.virtual_address;
            if (copy_to_user(arg_ptr, &xdma_data, sizeof(xdma_data)) != 0) {
                printk("Unable to copy to userspace : IOCTL_XDMA_ADDRESS \n");
                return -EFAULT;
            }
            break;
        }
        
        // XDMA_LENGTH
        case IOCTL_XDMA_SIZE: {
            xdma_data = dma_address.size;
            if (copy_to_user(arg_ptr, &xdma_data, sizeof(xdma_data)) != 0) {
                printk("Unable to copy to userspace : IOCTL_XDMA_LENGTH \n");
                return -EFAULT;
            }
            break;
        }
        
        // XDMA_COMMAND
        case IOCTL_XDMA_COMMAND: {
            if (copy_from_user(&xdma_data, arg_ptr, sizeof(xdma_data)) != 0) {
                printk("Unable to copy from userspace : IOCTL_XDMA_COMMAND \n");
                return -EFAULT;
            }
            writel(xdma_data, (dev_address.virtual_address + COMMAND_OFFSET));
            break;
        }
        
        // INVALID cmd
        default: {
            return -EINVAL;
        }
    }
    
    return 0;
}


//-----------------------------------------------------------------------------
/**
 * @brief Called when USER open our device
 */
static int xdma_open(struct inode *i, struct file *f) {
    // DEV
    dev_address.virtual_address = ioremap_nocache(dev_address.physical_address, dev_address.size);
    if(!dev_address.virtual_address) {
        printk("Kernel: ioremap_nocache failed\n");
        return -ENOMEM;
    }
    // DMA
    dma_address.virtual_address = dma_alloc_coherent(NULL, dma_address.size, &dma_address.physical_address, GFP_KERNEL);
    if (!dma_address.virtual_address) {
        printk("Kernel: dma_alloc_coherent failed\n");
        return -ENOMEM;
    }
    {
        // init XDMA_ADDRESS
        writel(dma_address.physical_address, (dev_address.virtual_address + ADDRESS_OFFSET));
	    // init XDMA_LENGTH
        writel((dma_address.size / 4), (dev_address.virtual_address + LENGTH_OFFSET)); 
    }
    return 0;
}


/**
 * @brief Called when USER close our device
 */
static int xdma_close(struct inode *i, struct file *f)
{
    // DEV
    if (dev_address.virtual_address) {
        iounmap(dev_address.virtual_address);
        dev_address.virtual_address = 0;
        DEBUGP("Kernel: c-iounmap\n");
    }
    // DMA
    if (dma_address.virtual_address) {
        dma_free_coherent(NULL, dma_address.size, dma_address.virtual_address, dma_address.physical_address);
        dma_address.virtual_address = 0;
        DEBUGP("Kernel: c-dma-free\n");
    }
    DEBUGP("Kernel: close\n");
    return 0;
}


//-----------------------------------------------------------------------------
/**
 * @brief Driver-specific memory-mapping facility, creates a new mapping in the virtual address space of the calling process.
 *
 *  @param [in] file  pointer to FILE structure assigned to our Linux device
 *  @param [in] vma   pointer to virtual memory space in which mapping is made
 *  @return result of MMAP operation : ZERO if success, NEGATIVE otherwise
 */
static int xdma_mmap(struct file *file, struct vm_area_struct *vma)
{   
    int rc;
    size_t requested_size;
    vma->vm_pgoff = dma_address.physical_address >> PAGE_SHIFT;
    
    // sz
    requested_size = vma->vm_end - vma->vm_start;
    DEBUGP("Kernel: vm_start=%x, vm_end=%x, rsz=%x, vm_pgoff=%x\n", (unsigned int)vma->vm_start, (unsigned int)vma->vm_end, (unsigned int)requested_size, (unsigned int)vma->vm_pgoff);
    if (requested_size > XDMA_MEM_SIZE) {
	    printk("Kernel: mmap warning! (requested_size > XDMA_MEM_SIZE)\n");
        //return -EAGAIN;
    }
    
    // prot
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
    
    //
    rc = remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, requested_size, vma->vm_page_prot);
    if (rc != 0) {
        printk("Kernel: remap_pfn_range failed. er_code=%x\n", rc);
        return rc;
    }
        
    DEBUGP("Kernel: mmap\n");
    return 0;
}


//-----------------------------------------------------------------------------
/**
 * @brief Struct points to all of XDMA device functions available
 */
static struct file_operations g_fops = {
    .owner = THIS_MODULE,
    
    .unlocked_ioctl = xdma_ioctl,
    .mmap = xdma_mmap,
    
    .open = xdma_open,
    .read = NULL,
    .write = NULL,
    .release = xdma_close,
};


//-----------------------------------------------------------------------------
/**
 * @brief XDMA driver Entry Point
 * @return result of INIT operation : ZERO if success, NEGATIVE otherwise
 */
static int __init xdma_init(void){
    // dec vars
    int error = 0;
    
    // Allocate a major and minor number region for the character device
    error = alloc_chrdev_region(&g_devno, 0, 1, XDMA_DEV_NAME); // dev_t, baseminor, count, name
    if (error < 0) { // err?
        printk("Kernel: alloc_chrdev_region failed\n");
        return error;
    }
    
    // Create a device class for our device
    g_cl = class_create(THIS_MODULE, XDMA_DEV_NAME);
    if(!g_cl) {
        unregister_chrdev_region(g_devno, 1);
        printk("Kernel: class_create failed\n");
        return -EINVAL;
    }
    
    // Create a device for our module. This will create a file on the filesystem, under "/dev/DEV_NAME"
    g_device = device_create(g_cl, NULL, g_devno, NULL, XDMA_DEV_NAME);
    if (!g_device) {
        class_destroy(g_cl);
        unregister_chrdev_region(g_devno, 1);
        printk("Kernel: device_create failed\n");
        return -EINVAL;
    }
    
    // Register our character device with the kernel
    cdev_init(&g_cdev, &g_fops);
    error = cdev_add(&g_cdev, g_devno, 1);
    if (error < 0) {
        device_destroy(g_cl, g_devno);
        class_destroy(g_cl);
        unregister_chrdev_region(g_devno, 1);
        printk("Kernel: cdev_add failed\n");
        return -EINVAL;
    }
    
    // 
    dev_address.physical_address = XDMA_DEFAULT_BASEADDR;
    dev_address.size = PAGE_SIZE;
    dev_address.virtual_address = 0;
    // 
    dma_address.physical_address = 0;
    dma_address.size = XDMA_MEM_SIZE;
    dma_address.virtual_address = 0;
    
    DEBUGP("Kernel: Module is up!!!\n");
    return error;
}


/**
 * @brief XDMA driver Exit Point
 * @return none
 */
static void __exit xdma_exit(void){
    // 
    if (dev_address.virtual_address) {
        iounmap(dev_address.virtual_address);
        dev_address.virtual_address = 0;
        DEBUGP("Kernel: e-iounmap\n");
    }
    if (dma_address.virtual_address) {
        dma_free_coherent(NULL, dma_address.size, dma_address.virtual_address, dma_address.physical_address);
        dma_address.virtual_address = 0;
        DEBUGP("Kernel: e-dma-free\n");
    }
    // 
    cdev_del(&g_cdev);
    device_destroy(g_cl, g_devno);
    class_destroy(g_cl);
    unregister_chrdev_region(g_devno, 1);
    DEBUGP("Kernel: Module is down!!!\n");
}


//-----------------------------------------------------------------------------
#ifndef DOXYGEN_SHOULD_SKIP_THIS
module_init(xdma_init);
module_exit(xdma_exit);
#endif // DOXYGEN_SHOULD_SKIP_THIS
//-----------------------------------------------------------------------------