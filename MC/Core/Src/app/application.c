/*
 * application.c
 *
 *  Created on: Jan 3, 2026
 *      Author: Admin
 */

#include "application.h"

extern  uint8_t RxData[256];
//extern UART_HandleTypeDef huart2;
extern volatile SystemFlag_t SystemFlag;
extern Axis_t AxisX, AxisY, AxisZ;
extern Input_indicator_t* Input_indicator;
extern uint32_t Tick;


extern volatile ModbusTransport_t activeTransport;
extern volatile uint32_t lastFrameTime;

extern  Motor_control_t* Motor_control;


void application_init(){
		HAL_Delay(7000);
		//HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxData, 256);
		HAL_TIM_Base_Start_IT(&htim5); //x
		HAL_TIM_Base_Start_IT(&htim9); //y
		HAL_TIM_Base_Start_IT(&htim2); //z
	//	HAL_TIM_Base_Start_IT(&htim6); // kiem tra hmi
		HAL_TIM_Base_Start_IT(&htim7); // kiem tra trang thai x, y, z
		Set_Speed_Motor_x( speed_default, speed_x_max);
		Set_Speed_Motor_y( speed_default, speed_y_max);
		Set_Speed_Motor_z( speed_default, speed_z_max);
		reset_counter_timer_x();
		reset_counter_timer_slave_x();
		reset_counter_timer_y();
		reset_counter_timer_slave_y();
		reset_counter_timer_z();
		reset_counter_timer_slave_z();
		Try_go_home();

}
void Try_go_home(){
	Handle_homeprocess();
	// Input_indicator->bits.home_state = 1;
	// Input_indicator->bits.motor_state = 0;

	//   if(get_home_z() == home_z){
	// 	  AxisZ.mode = MOVE_HOME2;
	//   }else {
	// 	  AxisZ.mode = MOVE_HOME1;
	//   }
	//   Home_process_z();
	// 	while((AxisZ.mode != MOVE_HOME3));
	// 	while((AxisZ.mode != STOP));


	//   if(get_home_x() == home_x){
	// 	  AxisX.mode = MOVE_HOME2;
	//   }else{
	// 	  AxisX.mode = MOVE_HOME1;
	//   }
	//   Home_process_x();

	//   if(get_home_y() == home_y){
	// 	  AxisY.mode = MOVE_HOME2;
	//   }else{
	// 	  AxisY.mode = MOVE_HOME1;
	//   }
	//   Home_process_y();
	//   wait_handler_stop();
}

void application_run_main(void){
//	if(activeTransport!=MODBUS_NONE){
//		if(Tick- lastFrameTime >=0 ){
//			activeTransport = MODBUS_NONE;
//		}
//	}
	Handle_motor();
	Handle_output();
	if((AxisX.mode == STOP) &&(AxisY.mode == STOP) && (AxisZ.mode == STOP)){
		Input_indicator->bits.motor_state = DONE ;
	}else{
		Input_indicator->bits.motor_state = NOT_YET ;
	}

	if((AxisX.current_pos == 0 ) && (AxisY.current_pos == 0) && (AxisZ.current_pos == 0)){
	Input_indicator->bits.home_state  = DONE ;
	}else{
	Input_indicator->bits.home_state  = NOT_YET ;
	}
}


void task_timer6(){
//	Handle_motor();
//	Handle_output();
}
void task_timer7(){
	Control_motor_y();
	Control_motor_x();
	Control_motor_z();
}







