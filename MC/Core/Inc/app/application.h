/*
 * application.h
 *
 *  Created on: Jan 3, 2026
 *      Author: Admin
 */

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_
#include "dio_control.h"
#include "modbus_rtu.h"
#include "motor_control.h"
#include "stm32f4xx_ll_tim.h"
#include "flash.h"
#include "common.h"
#include "hmi_control.h"
#include "tick.h"


void Handle(void);
void wait_handler_stop();
void application_init(void);
void application_run_main(void);
void task_timer6(void);
void task_timer7(void);
void Try_go_home();
typedef enum {
	NOT_YET = 0,
	DONE = 1
}State;




#endif /* INC_APPLICATION_H_ */
