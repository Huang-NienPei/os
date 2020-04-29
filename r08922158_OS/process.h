#include <sys/types.h>

typedef struct process{
    char name[40];
    int ready_time;
    int exec_time;
    pid_t pid;
} Process;

void unit_time();

void assign_proc_to_cpu(pid_t pid, int cpu_id);

int exec_process(Process proc);