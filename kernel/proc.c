
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               proc.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"

/*======================================================================*
                              schedule
 *======================================================================*/
PUBLIC void schedule()
{
	PROCESS* p;
	int queue = 2; //开始在1级队列
	int	 greatest_ticks = 0;
	int greatest_priority = 0;

	//那么每个进程的时间片都是一样的
	//给进程赋值时间片的点： 当所有进程的时间片都是空的，
	//刚来的时候都是空的，
	//每当一级队列运行完，都是空的

	while(!greatest_priority){
		int isEmpty = 1;
		//判断当前的所有进程是不是已经把分配的时间片都给用完了
		for(p = proc_table; p < proc_table+NR_TASKS;p++){
			if(p->remain != 0){
				//只要有一个进程的时间片没用完就不用重新分配时间片
				isEmpty = 0;
			}
		}
		//所有的进程的时间片都为空，也就是全部进入了新的队列
		if(isEmpty){
			for(p = proc_table; p < proc_table+NR_TASKS; p++){
				p->remain = 40*queue;  //给进程分配时间片
				queue += 1; //分配完之后队列自动下移
			}
		}

		//调度优先级最高的
		for(p = proc_table; p < proc_table+NR_TASKS; p++){
			//这时候调度的信息分为优先级，进程剩余运行时间，和进程还剩下的时间片
			//
			if(p->priority > greatest_priority && p->ticks > 0 && p->remain > 0){
				greatest_priority = p->priority;
				p_proc_ready = p;
				p_proc_ready->remain--; // 进程每调度一次时间片减少1
			}
		}
		if(!greatest_priority){
			for (p = proc_table; p < proc_table+NR_TASKS; p++) {
				p->ticks = p->priority; //进入到下一级队列，同样只做一次
			}
		}
	}
}

/*======================================================================*
                           sys_get_ticks
 *======================================================================*/
PUBLIC int sys_get_ticks()
{
	return ticks;
}

