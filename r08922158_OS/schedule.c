#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "process.h"
#include "schedule.h"

// static int cnt_finished;

int proc_cmp(const void *p1, const void *p2){
    int p1_ready_time = ((Process *)p1)->ready_time;
    int p2_ready_time = ((Process *)p2)->ready_time;
    return p1_ready_time > p2_ready_time; 
}

void schedule_FIFO(Process *process, int num_process){
    /* Scheduling Policy : FIFO */
    int elapsed_time = 0;
    int running = -1;
    int cnt_finished = 0;
    while(1){
        /* Check if the running process has finished execution */
        if(running != -1 && process[running].exec_time == 0){
            waitpid(process[running].pid, NULL, 0);    
            running = -1;
            cnt_finished ++;
            if(cnt_finished == num_process)
                break;
        }

        /* Check if any process is ready */
        for(int i = 0; i < num_process; i ++){
            if(process[i].ready_time == elapsed_time){
                process[i].pid = exec_process(process[i]);
                reduce_priority(process[i].pid);
            }
        }
        
        /* Determine next process to be executed */
        int next = -1;
        if(running != -1)
            next = running;
        else{
            for(int i = 0; i < num_process; i ++){
                if(process[i].pid == -1 || process[i].exec_time == 0)
                    continue;
                else if(next == -1 || process[i].ready_time < process[next].ready_time)
                    next = i;
            }
        }
        
        /* Check if there is need for context switch */
        if(next != -1 && running != next){
            /* Context switch */
            //reduce_priority(process[running].pid);
            raise_priority(process[next].pid, 50);
	    reduce_priority(process[running].pid);
            running = next;
        }

        /* Run a unit time */
        unit_time();
        if(running != -1)
            process[running].exec_time --;
        elapsed_time ++;
    }
    return;
}

void schedule_RR(Process *process, int num_process, int quantum){
    /* Scheduling Policy : RR */
    int elapsed_time = 0;
    int running = -1;
    int cnt_finished = 0;
    int last_start_time = 0;
    int last_running = -1;

    while(1){
        /* Check if the running process has finished execution */
        if(running != -1 && process[running].exec_time == 0){
            waitpid(process[running].pid, NULL, 0);
            running = -1;
            cnt_finished ++;
            if(cnt_finished == num_process)
                break;
        }

        /* Check if any process is ready */
        for(int i = 0; i < num_process; i ++){
            if(process[i].ready_time == elapsed_time){
                process[i].pid = exec_process(process[i]);
                reduce_priority(process[i].pid);
            }
        }

        /* Determine next process to be executed */
        int next = -1;
        if(running == -1){
            if(last_running == -1){
                for(int i = 0; i < num_process; i ++){
                    if(process[i].pid != -1 && process[i].exec_time > 0){
                        next = i;
                        break;
                    }
                }
            }  
            else{
                next = (last_running + 1) % num_process;
                while(process[next].pid == -1 || process[next].exec_time == 0)
                    next = (next + 1) % num_process; 
            }
        }
        else if((elapsed_time - last_start_time) % quantum == 0){
            next = (running + 1) % num_process;
            while(process[next].pid == -1 || process[next].exec_time == 0)
                next = (next + 1) % num_process;
        }
        else next = running;

        /* Check if there is need for context switch */
        if(next != -1 && running != next){
            /* Context switch */
	    raise_priority(process[next].pid, 50);
            reduce_priority(process[running].pid);
            running = next;
            last_start_time = elapsed_time;
            last_running = running;
        }

        /* Run a unit time */
        unit_time();
        if(running != -1)
            process[running].exec_time --;
        elapsed_time ++;
    }
    return;
}

void schedule_SJF(Process *process, int num_process){
    /* Scheduling Policy : SJF */
    int elapsed_time = 0;
    int running = -1;
    int cnt_finished = 0;

    while(1){
        /* Check if the running process has finished execution */
        if(running != -1 && process[running].exec_time == 0){
            waitpid(process[running].pid, NULL, 0);
            running = -1;
            cnt_finished ++;
            if(cnt_finished == num_process)
                break;
        }

        /* Check if any process is ready */
        for(int i = 0; i < num_process; i ++){
            if(process[i].ready_time == elapsed_time){
                process[i].pid = exec_process(process[i]);
                reduce_priority(process[i].pid);
            }
        }

        /* Determine next process to be executed */
        int next = -1;
        if(running != -1)
            next = running;
        else{
            for(int i = 0; i < num_process; i ++){
                if(process[i].pid == -1 || process[i].exec_time == 0)
                    continue;
                else if(next == -1 || process[i].exec_time < process[next].exec_time)
                    next = i;
            }
        }

        /* Check if there is need for context switch */
        if(next != -1 && running != next){
            /* Context switch */
	    raise_priority(process[next].pid, 50);
            reduce_priority(process[running].pid);
            running = next;
        }

        /* Run a unit time */
        unit_time();
        if(running != -1)
            process[running].exec_time --;
        elapsed_time ++;
    }
    return;
}

void schedule_PSJF(Process *process, int num_process){
    /* Scheduling Policy : SJF */
    int elapsed_time = 0;
    int running = -1;
    int cnt_finished = 0;

    while(1){
        /* Check if the running process has finished execution */
        if(running != -1 && process[running].exec_time == 0){
            waitpid(process[running].pid, NULL, 0);
            running = -1;
            cnt_finished ++;
            if(cnt_finished == num_process)
                break;
        }

        /* Check if any process is ready */
        for(int i = 0; i < num_process; i ++){
            if(process[i].ready_time == elapsed_time){
                process[i].pid = exec_process(process[i]);
                reduce_priority(process[i].pid);
            }
        }

        /* Determine next process to be executed */
        int next = -1;
        for(int i = 0; i < num_process; i ++){
            if(process[i].pid == -1 || process[i].exec_time == 0)
                continue;
            else if(next == -1 || process[i].exec_time < process[next].exec_time)
                next = i;
        }

        /* Check if there is need for context switch */
        if(next != -1 && running != next){
            /* Context switch */
	    raise_priority(process[next].pid, 50);
            reduce_priority(process[running].pid);
            running = next;
        }

        /* Run a unit time */
        unit_time();
        if(running != -1)
            process[running].exec_time --;
        elapsed_time ++;
    }
    return;
}
void schedule(int policy_id, Process *process, int num_process){
    //qsort(process, num_process, sizeof(Process), proc_cmp);

    /* -1 indicates that the process is not ready */
    for(int i = 0; i < num_process; i ++)
        process[i].pid = -1;

    /* Prevent parent from preempting childs */
    assign_proc_to_cpu(getpid(), 0);

    /* Raise the priority of scheduler */
    raise_priority(getpid(), 40);

    /* Schedule with different scheduling policy */
    switch(policy_id){
        case FIFO:
            schedule_FIFO(process, num_process);
	    break;
        case RR:
            schedule_RR(process, num_process, 500);
	    break;
        case SJF:
            schedule_SJF(process, num_process);
	    break;
        case PSJF:
            schedule_PSJF(process, num_process);
	    break;
    }
    return;
}
