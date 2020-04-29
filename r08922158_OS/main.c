#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "schedule.h"
#include "priority.h"

int main(int argc, char const *argv[])
{
    char policy[10];
    int num_process;
    
    scanf("%s", policy);
    scanf("%d", &num_process);

    Process *process = (Process *)malloc(sizeof(Process) * num_process);
    for(int i = 0; i < num_process; i ++)
        scanf("%s%d%d", process[i].name, &process[i].ready_time, &process[i].exec_time);

    int policy_id;
    if(strcmp(policy, "FIFO") == 0)
        policy_id = FIFO;
    else if(strcmp(policy, "RR") == 0)
        policy_id = RR;
    else if(strcmp(policy, "SJF") == 0)
        policy_id = SJF;
    else if(strcmp(policy, "PSJF") == 0)
        policy_id = PSJF;
    else{
        printf("Invalid policy. Please choose from one of the following \
            scheduling policy: FIFO, RR, SJF, PSJF.");
        return 0;
    }

    schedule(policy_id, process, num_process);
    free(process);
    return 0;
}
