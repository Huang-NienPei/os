#include <sys/types.h>

int raise_priority(pid_t pid, int priority);

int reduce_priority(pid_t pid);
