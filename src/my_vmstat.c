#include "my_vmstat.h"

static int __init vmstat_init(void)
{
    mutex_init(&lock);
    pr_info("vmstat module init\n");

    vmstat_dir = debugfs_create_dir(DIR_NAME, NULL);
    if (!vmstat_dir) {
        pr_err("Failed to create vmstat debug dir\n");
        return -ENOMEM;
    }

    vmstat_file = debugfs_create_file(FILE_NAME, 0666, vmstat_dir, NULL, &fops);
    if (!vmstat_file) {
        pr_err("Failed to create vmstat debug file\n");
        debugfs_remove_recursive(vmstat_dir);
        return -ENOMEM;
    }

    pr_info("vmstat module loaded\n");
    return 0;
}

static void __exit vmstat_exit(void)
{
    debugfs_remove_recursive(vmstat_dir);
    pr_info("vmstat module exit\n");
}


static ssize_t write_function(struct file *file, const char __user *buffer, size_t length, loff_t *ptr_offset) {
    char data_from_user_space[BUFFER_SIZE];
    if (copy_from_user(data_from_user_space, buffer, length) != 0) {
        single_open(file, print_error, NULL);
    }
    
    sscanf(data_from_user_space, "-o:%d", &settings);
    single_open(file, print_default_output, NULL);

    return length;
}

static int open_function(struct inode *inodep, struct file *filep) {
    mutex_lock(&lock);
    pr_info("File has been opened");
    return 0;
}

static int release_function(struct inode *inodep, struct file *filep) {
    pr_info("File has been closed");
    mutex_unlock(&lock);
    return 0;
}

static int print_default_output(struct seq_file *file, void *data) {
    if (settings == 0) {
        print_column_name(file);
    }
    print_stat(file);

    return 0;
}

static void print_stat(struct seq_file *file) {
    print_proc_stat(file);
    print_memory_info(file);
    print_system_info(file);
    print_time_info(file);
    seq_printf(file, "\n");

}

static void print_proc_stat(struct seq_file *file) {
    struct sysinfo si;

    si_meminfo(&si);

    int count = 0;
    for_each_process(task) {
        count++;
    }

    seq_printf(file, "%10lu%10lu | ", count, si.procs - si.loads[0]);

}

static void print_memory_info(struct seq_file *file) {
    struct sysinfo si;

    si_meminfo(&si);
    seq_printf(file, "%15lu%15lu%15lu%15lu | ", (si.totalswap - si.freeswap) << (PAGE_SHIFT - 10), si.freeram << (PAGE_SHIFT - 10), si.bufferram << (PAGE_SHIFT - 10),(si.totalram - si.freeram - si.bufferram - si.sharedram - si.totalswap - si.freeswap) << (PAGE_SHIFT - 10));
}

static void print_time_info(struct seq_file *file) {
    unsigned long usr_time = 0;
    unsigned long sys_time = 0;
    for_each_process(task) {
        usr_time += jiffies_to_msecs(task->utime)/1000;
        sys_time = jiffies_to_msecs(task->stime)/1000;
    }
    seq_printf(file, "%15lu%15li | ", usr_time, sys_time);
}

static void print_system_info(struct seq_file *file) {
    unsigned long total_voluntary_switches = 0;
    unsigned long total_involuntary_switches = 0;
    for_each_process(task) {
        total_voluntary_switches += task->nvcsw;
        total_involuntary_switches += task->nivcsw;
    }
    seq_printf(file, "%15lu%15li | ", total_voluntary_switches, total_involuntary_switches);
}


static void print_column_name(struct seq_file *file) {
    seq_printf(file, "%20s | %60s | %30s | %30s |\n", "proc", "memory", "system", "cpu");
    seq_printf(file, "%10s%10s | %15s%15s%15s%15s | %15s%15s | %15s%15s |\n", "r", "b", "swpd", "free", "buff", "usd+cache", "nvcsw", "nivcsw", "usr", "sys");
}



module_init(vmstat_init);
module_exit(vmstat_exit);
 
MODULE_LICENSE("GPL");

