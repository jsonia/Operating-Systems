#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>

static struct proc_dir_entry *ent;

static int proc_count_show(struct seq_file *file, void *x){
struct task_struct *t;
int n = 0;
for_each_process(t) {n++;}
seq_printf(file,"%d\n", n);
return 0;
}

static int __init proc_count_init(void)
{
    	ent = proc_create_single("count", 0, NULL, proc_count_show);
	return 0;
}

static void __exit proc_count_exit(void)
{
	proc_remove(ent);
}

module_init(proc_count_init);
module_exit(proc_count_exit);

MODULE_AUTHOR("Your name");
MODULE_DESCRIPTION("One sentence description");
MODULE_LICENSE("GPL");
