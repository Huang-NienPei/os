#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/timer.h>

asmlinkage long sys_my_get_time(void){
    struct timespec time;
    getnstimeofday(&time);
    return time.tv_sec * 1000000000 + time.tv_nsec; 
}
