#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>
#include "process.h"
#include "priority.h"

#define MY_GET_TIME 334
#define MY_PRINTK 333

void unit_time(){
    volatile unsigned long i;
    for(i = 0; i < 1000000UL; i ++);
    return; 
}

void assign_proc_to_cpu(pid_t pid, int cpu_id){
    /* Assign a process to a particular cpu */
    if(cpu_id > sizeof(cpu_set_t)){
        fprintf(stderr, "Invalid CPU ID.");
        return; 
    }

    cpu_set_t cpu_mask;
    CPU_ZERO(&cpu_mask);
    CPU_SET(cpu_id, &cpu_mask);

    if(sched_setaffinity(pid, sizeof(cpu_mask), &cpu_mask) < 0){
        perror("sched_setaffinity");
        exit(0);
    }
}

int exec_process(Process proc){
    /* Execute the given process */
    pid_t pid = fork();
    
    if(pid < 0){
        perror("fork");
        return -1;
    }

    if(pid == 0){
        
        //printf("%s %d\n", proc.name, cur_pid);
        long start_time, end_time;
        start_time = syscall(MY_GET_TIME);

        for(int t = 0; t < proc.exec_time; t ++)
            unit_time();

        end_time = syscall(MY_GET_TIME);
       
        char print_to_dmesg[100];
        
        sprintf(print_to_dmesg, "[Project1] %d %ld.%ld %ld.%ld\n", 
            getpid(), start_time / 1000000000, start_time % 1000000000,
            end_time / 1000000000, end_time % 1000000000);
        
        syscall(MY_PRINTK, print_to_dmesg);
	printf("%s %d\n", proc.name, getpid());
	exit(0);
    }
    reduce_priority(pid);
    assign_proc_to_cpu(pid, 1);
    return pid;
}

