#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define MAJOR_NUMBER 61

/* forward declaration */
int fourmb_device_open(struct inode *inode, struct file *filep);
int fourmb_device_release(struct inode *inode, struct file *filep);
ssize_t fourmb_device_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t fourmb_device_write(struct file *filep, const char *buf,size_t count, loff_t *f_pos);
static void fourmb_device_exit(void);

/* definition of file_operation structure */
struct file_operations fourmb_device_fops = {
	read: fourmb_device_read,
	write: fourmb_device_write,
	open: fourmb_device_open,
	release: fourmb_device_release
};

char *fourmb_device_data = NULL;

int fourmb_device_open(struct inode *inode, struct file *filep)
{
	return 0; // always successful
}

int fourmb_device_release(struct inode *inode, struct file *filep)
{
	return 0; // always successful
}

ssize_t fourmb_device_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{
	if(copy_to_user(buf, fourmb_device_data, 1))
	{
		return -EFAULT; // error happens
	}
	*f_pos += 1;
	if(*f_pos > 1)
	{
		return 0;
	}
	return 1;
}

ssize_t fourmb_device_write(struct file *filep, const char *buf,size_t count, loff_t *f_pos)
{
	if(*f_pos == 0)
	{
		if(copy_from_user(fourmb_device_data, buf, 1))
		{
			return -EFAULT; // error happens
		}
		*f_pos += 1;
		return 1;
	}
	else
	{
		return -ENOSPC; // No space left on device
	}
}

static int fourmb_device_init(void)
{
	int result;
	// register the device
	result = register_chrdev(MAJOR_NUMBER, "fourmb_device",&fourmb_device_fops);
	if (result < 0) {
		return result;
	}

	// allocate one byte of memory for storage
	// kmalloc is just like malloc, the second parameter is
	// the type of memory to be allocated.
	// To release the memory allocated by kmalloc, use kfree.
	fourmb_device_data = kmalloc(sizeof(char), GFP_KERNEL);
	if (!fourmb_device_data) {
		fourmb_device_exit();
		// cannot allocate memory
		// return no memory error, negative signify a failure
		return -ENOMEM;
	}

	// initialize the value to be X
	*fourmb_device_data = 'X';
	printk(KERN_ALERT "This is a fourmb_device device module\n");
	return 0;
}

static void fourmb_device_exit(void)
{
	// if the pointer is pointing to something
	if (fourmb_device_data) {
		// free the memory and assign the pointer to NULL
		kfree(fourmb_device_data);
		fourmb_device_data = NULL;
	}
	// unregister the device
	unregister_chrdev(MAJOR_NUMBER, "fourmb_device");
	printk(KERN_ALERT "fourmb_device device module is unloaded\n");
}

MODULE_LICENSE("GPL");
module_init(fourmb_device_init);
module_exit(fourmb_device_exit);
