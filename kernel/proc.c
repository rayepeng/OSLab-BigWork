
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
	int	 greatest_ticks = 0;
	int  greatest_level = 100;
	int  count = 0;
	int  save = 0;
	
	while (!greatest_ticks) {
		for (p = proc_table; p < proc_table+NR_TASKS; p++) {
			if (p->level < greatest_level && p->remain > 0) {
				greatest_ticks = p->remain;
				greatest_level = p->level;
				save = count;
			}
			count++;
		}
		if (!greatest_ticks) {
			for (p = proc_table; p < proc_table+NR_TASKS; p++) {
				p->remain = p->priority;
				p->ticks = 1;
				p->level = 1;
			}
			disp_str("\n");
			continue;
		}
		count = 0;
		disp_str(save);
		for (p = proc_table; p < proc_table+NR_TASKS; p++) {
			if(save == count){
				if(p->level == 1){
					p->ticks = 2;
					p->remain -= 2;
				}
				else if(p->level == 2){
					p->ticks = 3;
					p->remain -= 3;
				}
				else if(p->level > 2){
					p->ticks = 4;
					p->remain -= 4;
				}
				if(p->remain < 0){
					p->ticks += p->remain;
					p->remain = 0;
				}
				p->level += 1;
				p_proc_ready = p;
				break;
			}
			count++;
		}
		count = 0;
		save = 0;
	}
}

/*======================================================================*
                           sys_get_ticks
 *======================================================================*/
PUBLIC int sys_get_ticks()
{
	return ticks;
}

