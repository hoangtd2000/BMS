	/*
 * hmi_control.c
 *
 *  Created on: Jan 24, 2026
 *      Author: Admin
 */


#include "hmi_control.h"

Motor_control_t* Motor_control = (Motor_control_t*)&Coils_Database[0];
Input_indicator_t* Input_indicator = (Input_indicator_t*) &Inputs_Database[0];
Axisfeedback_t* Axisfeedback = (Axisfeedback_t*)&Input_Registers_Database[0];
Axiscommand_t* Axiscommand =  (Axiscommand_t*)&Holding_Registers_Database[0];
Output_control_t* Output_control = (Output_control_t*)&Coils_Database[2];

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} OutputMap_t;


OutputMap_t output_map[18] = {
    {O1_GPIO_Port, O1_Pin},
	{O2_GPIO_Port, O2_Pin},
	{O3_GPIO_Port, O3_Pin},
	{O4_GPIO_Port, O4_Pin},
	{O5_GPIO_Port, O5_Pin},
	{O6_GPIO_Port, O6_Pin},
	{O7_GPIO_Port, O7_Pin},
	{O8_GPIO_Port, O8_Pin},
	{O9_GPIO_Port, O9_Pin},
	{O10_GPIO_Port, O10_Pin},
	{O11_GPIO_Port, O11_Pin},
	{O12_GPIO_Port, O12_Pin},
	{O13_GPIO_Port, O13_Pin},
	{O14_GPIO_Port, O14_Pin},
	{O15_GPIO_Port, O15_Pin},
	{O16_GPIO_Port, O16_Pin},
	{O17_GPIO_Port, O17_Pin},
	{O18_GPIO_Port, O18_Pin},
};


extern Axis_t AxisX, AxisY, AxisZ;

volatile SystemFlag_t SystemFlag={
		.is_homing = 0 ,
		.is_start = 0,
		.is_stop= 0,
		.is_err = 0,
};

ActionHandler_t Tab_main_table[] =  {
    	 Handle_gohome  ,
         Handle_setpoint,
         Handle_estop 	,
         Handle_restart ,
         Handle_stop ,
		 Handle_reserve,
		 Handle_reserve,
		 Handle_reserve,
		 Handle_X_Left,
		 Handle_X_Right,
		 Handle_Y_Backward,
		 Handle_Y_Forward,
		 Handle_Z_Up,
		 Handle_Z_Down,
		 Handle_reserve,
		 Handle_reserve,
};

 void Set_PC_Position_Axis_X(uint16_t value){
	 Axisfeedback->pos_x = value;
}
 void Set_PC_Position_Axis_Y(uint16_t value){
	 Axisfeedback->pos_y = value;
}
void Set_PC_Position_Axis_Z(uint16_t value){
	Axisfeedback->pos_z = value;
}

uint16_t Get_PC_Position_Axis_X(void){
	return Axiscommand->target_x;
}
uint16_t Get_PC_Position_Axis_Y(void){
	return Axiscommand->target_y;
}
uint16_t Get_PC_Position_Axis_Z(void){
	return Axiscommand->target_z;
}

void Set_PC_Speed_Axis_X(uint16_t value){
	Axisfeedback->speed_x = value * period_x;
}
void Set_PC_Speed_Axis_Y(uint16_t value){
	Axisfeedback->speed_y = value *period_y;
}
void Set_PC_Speed_Axis_Z(uint16_t value){
	Axisfeedback->speed_z = value*period_z;
}
uint16_t Get_PC_Speed_Axis_X(void){
	return ((Axiscommand->speed_x ));
}
uint16_t Get_PC_Speed_Axis_Y(void){
	return ((Axiscommand->speed_y ));
}
uint16_t Get_PC_Speed_Axis_Z(void){
	return ((Axiscommand->speed_z));
}

void Set_PC_State_Axis_X(uint16_t value){
	Axisfeedback->state_x = value;
}
void Set_PC_State_Axis_Y(uint16_t value){
	Axisfeedback->state_y = value;
}
void Set_PC_State_Axis_Z(uint16_t value){
	Axisfeedback->state_z = value;
}
uint16_t Get_PC_Maxspeed_Axis_X(void){
	return (Axiscommand->maxspeed_x );
}
uint16_t Get_PC_Maxspeed_Axis_Y(void){
	return (Axiscommand->maxspeed_y );
}
uint16_t Get_PC_Maxspeed_Axis_Z(void){
	return (Axiscommand->maxspeed_z );
}



// taskbar
void Handle_main(void){
	uint16_t builtin_Handle_main = __builtin_ffs(Motor_control->all);
		if (builtin_Handle_main > 0) {
			builtin_Handle_main -= 1;
		    if (builtin_Handle_main < (int)(sizeof(Tab_main_table) / sizeof(Tab_main_table[0]))) {
		    	Tab_main_table[builtin_Handle_main]();
		    }
		}
}

uint32_t output_shadow = 0 ;

void Handle_output(void){
	uint32_t current = Output_control->all;
	uint32_t changed = current ^ output_shadow;
	    if(changed == 0)
	        return;
	    for(uint8_t i = 0; i < 18; i++)
	    {
	        if(changed & (1UL << i))
	        {
	            if(current & (1UL << i))
	            {
	                HAL_GPIO_WritePin(output_map[i].port,output_map[i].pin,GPIO_PIN_RESET);
	            }
	            else
	            {
	                HAL_GPIO_WritePin(output_map[i].port,output_map[i].pin,GPIO_PIN_SET);
	            }
	        }
	    }
	    output_shadow = current;
}



void Handle_reset(void){
}
void Handle_start(void){
}
void Handle_stop(void){
}

void Handle_gohome(void){


	if(AxisX.mode == STOP && AxisY.mode == STOP && AxisZ.mode == STOP ){
		 Input_indicator->bits.motor_busy = 1;
		Input_indicator->bits.home_done = 0;
		  if(get_home_z() == home_z){
			  AxisZ.mode = MOVE_HOME2;
			  		Set_Speed_Motor_z(speed_home2_z, speed_z_max);
			  		move_z_down(2500);
		  }else {
			  AxisZ.mode = MOVE_HOME1;
			  		Set_Speed_Motor_z(speed_home1_z, speed_z_max);
			  		// cộng thêm tránh trường hợp đi từ max vào
			  		move_z_up(max_z + 1000);
		  }
		  		while((AxisZ.mode != MOVE_HOME3));
		  		while((AxisZ.mode != STOP));
		  if(get_home_x() == home_x){
			  AxisX.mode = MOVE_HOME2;
			  		Set_Speed_Motor_x(speed_home2_x, speed_x_max);
			  		move_x_right(2000);
		  }else{
			  AxisX.mode = MOVE_HOME1;


			  		Set_Speed_Motor_x(speed_home1_x, speed_x_max);
			  		move_x_left(max_x+1000);
		  }

		  if(get_home_y() == home_y){
			  AxisY.mode = MOVE_HOME2;

			  		Set_Speed_Motor_y(speed_home2_y, speed_y_max);
			  		move_y_forward(2000);
		  }else{
			  AxisY.mode = MOVE_HOME1;

			  		Set_Speed_Motor_y(speed_home1_y, speed_y_max);
			  		move_y_backward(max_y + 500);
		  }



		  wait_handler_stop();

//			Input_indicator->bits.motor_busy = 0;
//			Input_indicator->bits.home_done = 1;
	}
}

void Handle_setpoint(void){
	Motor_control->bits.setpoint = 0 ;
	//wait_handler_stop();
	if(AxisX.mode == STOP && AxisY.mode == STOP && AxisZ.mode == STOP ){
	move_axis(Get_PC_Position_Axis_X(),Get_PC_Position_Axis_Y(), Get_PC_Position_Axis_Z());
	}
}

void Handle_estop(void){

}
void Handle_restart(void){

}
void Handle_reserve(void){

}

void Handle_X_Left (void){
	if(AxisX.current_pos <= 0) {
		return;
		}
	if(AxisX.mode == STOP) {
		AxisX.mode =  MOVE_MANUAL;
		move_x_left(AxisX.current_pos);
		}
}
void Handle_X_Right (void){
	if(AxisX.current_pos >= max_x){
		return;
	}
	if(AxisX.mode == STOP) {
	  AxisX.mode =  MOVE_MANUAL;
	  move_x_right(max_x - AxisX.current_pos);
	}

}
void Handle_Y_Forward(void){
	if(AxisY.current_pos >= max_y) {
		return;
		}
	if(AxisY.mode == STOP){
		AxisY.mode = MOVE_MANUAL;
		move_y_forward(max_y- AxisY.current_pos);
	}
}
void Handle_Y_Backward(void){
	if(AxisY.current_pos <= 0  ){
		return ;
	}
	if(AxisY.mode == STOP){
		AxisY.mode = MOVE_MANUAL;
		move_y_backward( AxisY.current_pos);
	}
}
void Handle_Z_Up(void){
	if(AxisZ.current_pos <=0  ){
			return ;
		}
	if(AxisZ.mode == STOP) {
		AxisZ.mode = MOVE_MANUAL;
		move_z_up(AxisZ.current_pos);
	}
}
void Handle_Z_Down(void){
	if(AxisZ.current_pos >=max_z  ){
			return ;
		}
	if(AxisZ.mode == STOP) {
		AxisZ.mode = MOVE_MANUAL;
		move_z_down(max_z-AxisZ.current_pos);
	}

}






