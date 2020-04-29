#include "priority.h"
#define FIFO 1
#define RR   2 
#define SJF  3
#define PSJF 4


void schedule_FIFO(Process *process, int num_process);

void schedule_RR(Process *process, int num_process, int quantum);

void schedule_SJF(Process *process, int num_process);

void schedule_PSJF(Process *process, int num_process);

void schedule(int policy_id, Process *process, int num_process);