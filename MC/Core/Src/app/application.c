/*
 * application.c
 *
 *  Created on: Jan 3, 2026
 *      Author: Admin
 */

#include "application.h"

extern  uint8_t RxData[256];
extern UART_HandleTypeDef huart2;
extern volatile SystemFlag_t SystemFlag;
extern Axis_t AxisX, AxisY, AxisZ;
extern Input_indicator_t* Input_indicator;
extern uint32_t Tick;


extern volatile ModbusTransport_t activeTransport;
extern volatile uint32_t lastFrameTime;

extern  Motor_control_t* Motor_control;


void application_init(){
		HAL_Delay(7000);
		while(get_estop());
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxData, 256);
		HAL_TIM_Base_Start_IT(&htim5); //x
		HAL_TIM_Base_Start_IT(&htim9); //y
		HAL_TIM_Base_Start_IT(&htim2); //z
		HAL_TIM_Base_Start_IT(&htim6); // kiem tra hmi
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
	Input_indicator->bits.motor_busy = 1;
	Input_indicator->bits.home_done = 0;

	  if(get_home_z() == home_z){
		  AxisZ.mode = MOVE_HOME2;
		  //	case MOVE_HOME1:
		  //		Set_Speed_Motor_z(speed_home1_z, speed_z_max);
		  //		// cộng thêm tránh trường hợp đi từ max vào
		  //		move_z_up(max_z + 1000);
		  //		break;
		  //	case MOVE_HOME2:
		  //		if( get_counter_timer_slave_z() == 0 ){
		  		Set_Speed_Motor_z(speed_home2_z, speed_z_max);
		  		move_z_down(2500);
		  //		}
		  //		break;
	  }else {
		  AxisZ.mode = MOVE_HOME1;
		  //	case MOVE_HOME1:
		  		Set_Speed_Motor_z(speed_home1_z, speed_z_max);
		  		// cộng thêm tránh trường hợp đi từ max vào
		  		move_z_up(max_z + 1000);
		  //		break;
		  //	case MOVE_HOME2:
		  //		if( get_counter_timer_slave_z() == 0 ){
		  //		Set_Speed_Motor_z(speed_home2_z, speed_z_max);
		  //		move_z_down(2500);
		  //		}
		  //		break;
	  }
		while((AxisZ.mode != MOVE_HOME3));
		while((AxisZ.mode != STOP));


	  if(get_home_x() == home_x){
		  AxisX.mode = MOVE_HOME2;


		  //	case MOVE_HOME1:
		  //		Set_Speed_Motor_x(speed_home1_x, speed_x_max);
		  //		move_x_left(max_x+1000);
		  //		break;
		  //	case MOVE_HOME2:
		  //		if( get_counter_timer_slave_x() == 0 ){
		  		Set_Speed_Motor_x(speed_home2_x, speed_x_max);
		  		move_x_right(2000);
		  //		}
		  //		break;

	  }else{
		  AxisX.mode = MOVE_HOME1;

		  //	case MOVE_HOME1:
		  		Set_Speed_Motor_x(speed_home1_x, speed_x_max);
		  		move_x_left(max_x+1000);
		  //		break;
		  //	case MOVE_HOME2:
		  //		if( get_counter_timer_slave_x() == 0 ){
		  //		Set_Speed_Motor_x(speed_home2_x, speed_x_max);
		  //		move_x_right(2000);
		  //		}
		  //		break;



	  }




	  if(get_home_y() == home_y){
		  AxisY.mode = MOVE_HOME2;


//case MOVE_HOME1:
//		Set_Speed_Motor_y(speed_home1_y, speed_y_max);
//		move_y_backward(max_y + 500);
//		break;
//	case MOVE_HOME2:
//		if( get_counter_timer_slave_y() == 0 ){
		Set_Speed_Motor_y(speed_home2_y, speed_y_max);
		move_y_forward(2000);
//		}
//		break;


	  }else{
		  AxisY.mode = MOVE_HOME1;
		  //case MOVE_HOME1:
		  		Set_Speed_Motor_y(speed_home1_y, speed_y_max);
		  		move_y_backward(max_y + 500);
		  //		break;
		  //	case MOVE_HOME2:
		  //		if( get_counter_timer_slave_y() == 0 ){
		  //		Set_Speed_Motor_y(speed_home2_y, speed_y_max);
		  //		move_y_forward(2000);
		  //		}
		  //		break;
	  }
	  wait_handler_stop();
//		Input_indicator->bits.motor_busy = 0;
//		Input_indicator->bits.home_done = 1;
}

void application_run_main(void){
//	  if(Timer_Check(0, 500)){
//		  OFF_LED_RED;
//	//	  OFF_BUZZ;
//		  TOGGLE_LED_GREEN;
//	  }
//	  else if(Timer_Check(2, 500)  && SystemFlag.is_err){
//		  OFF_LED_GREEN;
//		  TOGGLE_LED_RED;
//		//  TOGGLE_BUZZ;
//	  }
//	if(AxisX.mode == STOP && AxisY.mode == STOP && AxisZ.mode == STOP ){
//		Input_indicator->bits.home_done = 1 ;
//		Input_indicator->bits.motor_busy = 0 ;
//	}
	if(activeTransport!=MODBUS_NONE){
		if(Tick- lastFrameTime >=0 ){
			activeTransport = MODBUS_NONE;
		}
	}
//	if(AxisX.mode ==MOVE_MANUAL ){
//			if((Motor_control->bits.x_left == 0 ) && (Motor_control->bits.x_right == 0 )){
//				if(get_counter_timer_x() >= get_pulse_x() ){
//				Stop_motor_x();
//				}
//			}
//	}
}


void task_timer6(){
	Handle_main();
	Handle_output();
}
void task_timer7(){
	Control_motor_y();
	Control_motor_x();
	Control_motor_z();
}







