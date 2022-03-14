/*
 * Task_MGR.h
 *
 *  Created on: Jan 26, 2022
 *      Author: Jesus Rodriguez
 */

#ifndef INC_TASK_MGR_H_
#define INC_TASK_MGR_H_

/*
 * General Tasks
 */

void Get_Time_RTC_Runnable(void * parameters);
void Print_Time_Runnable(void * parameters);
void Process_UART_Data_Runnable(void * parameters);

#endif /* INC_TASK_MGR_H_ */
