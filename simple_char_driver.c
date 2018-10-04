#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
//#include <asm/uaccess.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 1024

/* Define device_buffer and other global data structures you will need here */
static char device_buffer[BUFFER_SIZE] = {0};
int openCounter = 0;
int closeCounter = 0;
char* buffer;
struct file_operations;

ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/* length is the length of the userspace buffer*/
	/* offset will be set to current position of the opened file after read*/
	/* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */
  if(*offset >= BUFFER_SIZE)
    {
      return -1;
    }
  //copy kernel buffer to user buffer
  copy_to_user(buffer, device_buffer + *offset, length);
  //print alert to kernel
  printk(KERN_ALERT, "simple_char_driver: read %d bytes", length);
  //increment offset by length
  *offset += length;
  return length; 

}



ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */
  if(*offset >= BUFFER_SIZE)
    {
      return -1;
    }
  copy_from_user(device_buffer + *offset, buffer, length);
  printk(KERN_ALERT "simple_char_driver: wrote %d bytes", length);
  //*offset += length;
  return length;
}


int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
        openCounter++;
        printk(KERN_ALERT "The device has been opened %d times. \n", openCounter); 
	      return 0;
}

int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
  closeCounter++;
  printk(KERN_ALERT "The device has been closed %d times. \n", closeCounter);
	return 0;
}

loff_t simple_char_driver_llseek (struct file *pfile, loff_t offset, int whence)
{
	/* Update open file position according to the values of offset and whence */
  if (whence == 0)
    {
      if(offset > BUFFER_SIZE || offset < 0)
	     {
	       return -1;
	     }
      pfile->f_pos = offset;
    }
  if (whence == 1)
    {
      if (offset > BUFFER_SIZE)
	       {
	        return -1;
	       }
      pfile->f_pos += offset;
     
    }
  if (whence == 2)
    {
      if (offset > BUFFER_SIZE || offset < 0)
	     {
	       return -1;
	     }
      pfile->f_pos = BUFFER_SIZE - offset;
  
    }
    return 0;
}

struct file_operations simple_char_driver_file_operations = {

	      .owner = THIS_MODULE,
        .open = simple_char_driver_open,
        .release = simple_char_driver_close,
        .llseek = simple_char_driver_llseek,
        .read = simple_char_driver_read,
        .write = simple_char_driver_write,
        
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
};

static int simple_char_driver_init(void)
{
  printk(KERN_ALERT);
  register_chrdev(255, "simple_char_driver", &simple_char_driver_file_operations);
  buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
  
	/* print to the log file that the init function is called.*/
	/* register the device */
	return 0;
}

static void simple_char_driver_exit(void)
{
  printk(KERN_ALERT);
  unregister_chrdev(255, "simple_char_driver");
  kfree(buffer);
	/* print to the log file that the exit function is called.*/
	/* unregister  the device using the register_chrdev() function. */
  return;
}

module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);

/* add module_init and module_exit to point to the corresponding init and exit function*/
