#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

MODULE_AUTHOR("Motomi Ikeda");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

const u32 led[3] = {23, 24, 25};
static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
	int n=0, i=0, j = 0;
	
	if(copy_from_user(&c,buf,sizeof(char)))
		return -EFAULT;
	
	//printk(KERN_INFO "receive %c\n");
	
	i = c;
	i = i + 1;	

	/*for(n=0; i>n; n++){
		gpio_base[7] = 1 << 25;
		msleep(500);
		gpio_base[10] = 1 << 25;
		msleep(500);
	}*/

	if(c=='0'){
		gpio_base[10] = 1 << 25;
	}else if(c == "green" | c == 'g'){
		gpio_base[7] = 1 << 25;
	}


	if(c=='0'){
		gpio_base[10] = 1 << 24;
	}else if(c == "yellow" | c == 'y'){
		gpio_base[7] = 1 << 24;
	}

	if(c=='0'){
		gpio_base[10] = 1 << 23;
	}else if(c == "red" | c == 'r'){
		gpio_base[7] = 1 << 23;
	}

	return 1;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write
};

static int __init init_mod(void)
{
	int retval;
	retval = alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
		printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;
	}

	printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,  MAJOR(dev));
	
	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv,  dev, 1);
	if(retval < 0){
		printk(KERN_ERR "cdev_add failed. major:%d, minor:%d", MAJOR(dev), MINOR(dev));
		return retval;
	}
	
	cls = class_create(THIS_MODULE, "myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.");
		return PTR_ERR(cls);
	}
	device_create(cls,  NULL, dev,  NULL, "myled%d",MINOR(dev));

	gpio_base = ioremap_nocache(0x3f200000,  0xA0);
	
	const u32 led25 = 25, led24 = 24, led23 = 23;
	const u32 index25 = led25/10, index24 = led24/10, index23 = led23/10;
	const u32 shift25 = (led25%10)*3, shift24 = (led24%10)*3, shift23 = (led23%10)*3 ;
	const u32 mask25 = ~(0x7 << shift25), mask24 = ~(0x7 << shift24), mask23 = ~(0x7 << shift23);
	gpio_base[index25] = (gpio_base[index25] & mask25) | (0x1 << shift25);
	gpio_base[index24] = (gpio_base[index24] & mask24) | (0x1 << shift24);
	gpio_base[index23] = (gpio_base[index23] & mask23) | (0x1 << shift23);

	return 0;
}

static void __exit cleanup_mod(void)
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__, MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);
