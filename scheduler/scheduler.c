
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/desc.h>
#include <asm/pgtable.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/highmem.h>
#include <linux/preempt.h>
#include <asm/topology.h>
#include <linux/kthread.h>
#include <linux/kernel_stat.h>
#include <linux/percpu-defs.h>
#include <linux/smp.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

struct OurTask
{
	int (*fp)(void);
	struct OurTask *next;
};
struct OurTask TaskForce;

EXPORT_SYMBOL(TaskForce);



int thread1(void *data)
{
	struct OurTask *temp;
	int res=0;
	int (*fp)(void);

	TaskForce.next = NULL;
	TaskForce.fp =NULL;	


	while(1)
	{	
		
		if(TaskForce.next == NULL)
		{
			schedule();
			continue;
		}
		printk("new task detected!\n");
		msleep(1000);
			
		

		fp = TaskForce.next->fp;
		fp();

		temp = TaskForce.next;
		TaskForce.next = TaskForce.next->next;	
		
		vfree(temp->fp);
		temp->fp = NULL;	
		kfree(temp);
		temp = NULL;

	}
	return 0;
}

static int hello_init(void)
{
	struct task_struct *task1;

	task1 = kthread_create(thread1,NULL,"task1");
	wake_up_process(task1);

	return 0;
}


static void hello_exit(void)
{

	printk("end module\n=========================================================\n");

}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");



