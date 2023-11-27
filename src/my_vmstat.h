#include <linux/init.h> 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/debugfs.h>
#include <linux/fs.h>
#include "util.h"
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/delay.h>
#include <linux/kernel_stat.h>

static struct dentry *vmstat_dir;
static struct dentry *vmstat_file;
static struct task_struct* task;
static output_settings settings = 0;

static int print_default_output(struct seq_file *file, void *data);

static ssize_t write_function(struct file *file, const char __user *buffer, size_t length, loff_t *ptr_offset);

static int open_function(struct inode *inodep, struct file *filep);

static int release_function(struct inode *inodep, struct file *filep);

struct file_operations fops = {
    .open = open_function,
    .write = write_function,
    .release = release_function,
    .read = seq_read
};

static void print_stat(struct seq_file *file);

static void print_proc_stat(struct seq_file *file);

static void print_column_name(struct seq_file *file);

static void print_memory_info(struct seq_file *file);

static void print_system_info(struct seq_file *file);

static void print_time_info(struct seq_file *file);
// static void print_cpu_info(struct seq_file *file);


static int print_error(struct seq_file *file, void *data) {
    seq_printf(file, "%s\n", "Failed");
    return 0;
}

