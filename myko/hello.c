#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL v2");
static int __init hello_init(void)
{
	printk("Hello World!\r\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk("hello exit!\r\n");
	return;
}

module_init(hello_init);
module_exit(hello_exit);
