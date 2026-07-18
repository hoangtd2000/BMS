	/*
 * hmi_control.c
 *
 *  Created on: Jan 24, 2026
 *      Author: Admin
 */


#include "hmi_control.h"

Motor_control_t* Motor_control 		= (Motor_control_t*)&Coils_Database[0];
Output_control_t* Output_control 	= (Output_control_t*)&Coils_Database[1];
Input_indicator_t* Input_indicator 	= (Input_indicator_t*) &Inputs_Database[0];
Axisfeedback_t* Axisfeedback 		= (Axisfeedback_t*)&Input_Registers_Database[0];
Axiscommand_t* Axiscommand 			= (Axiscommand_t*)&Holding_Registers_Database[0];

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} OutputMap_t;


OutputMap_t output_map[8] = {
    {O1_GPIO_Port, O1_Pin},
	{O2_GPIO_Port, O2_Pin},
	{O3_GPIO_Port, O3_Pin},
	{O4_GPIO_Port, O4_Pin},
	{O5_GPIO_Port, O5_Pin},
	{O6_GPIO_Port, O6_Pin},
	{O7_GPIO_Port, O7_Pin},
	{O8_GPIO_Port, O8_Pin},
};


extern Axis_t AxisX, AxisY, AxisZ;

volatile SystemFlag_t SystemFlag={
		.is_homing = 0 ,
		.is_start = 0,
		.is_stop= 0,
		.is_err = 0,
};

ActionHandler_t Tab_main_table[] =  {
    	 Handle_homeprocess  ,
         Handle_setpoint,
		 Handle_X_Left,
		 Handle_X_Right,
		 Handle_Y_Backward,
		 Handle_Y_Forward,
		 Handle_Z_Up,
		 Handle_Z_Down,
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
	if(value == STOP ){
		Axisfeedback->state_x = DONE;
	}else{
		Axisfeedback->state_x = NOT_YET;
	}
}
void Set_PC_State_Axis_Y(uint16_t value){
	if(value == STOP ){
		Axisfeedback->state_y = DONE;
	}else{
		Axisfeedback->state_y = NOT_YET;
	}
}
void Set_PC_State_Axis_Z(uint16_t value){
	if(value == STOP ){
		Axisfeedback->state_z = DONE;
	}else{
		Axisfeedback->state_z = NOT_YET;
	}
}
uint16_t Get_PC_Limit_Axis_X(void){

	if((Axiscommand->limit_x <= max_x/2)||(Axiscommand->limit_x  >= max_x)){
		Axiscommand->limit_x =  max_x;
	}
	return (Axiscommand->limit_x );
}
uint16_t Get_PC_Limit_Axis_Y(void){
	if((Axiscommand->limit_y <= max_y/2)||(Axiscommand->limit_y  >= max_y)){
		Axiscommand->limit_y =  max_y;
	}
	return (Axiscommand->limit_y );
}
uint16_t Get_PC_Limit_Axis_Z(void){
	if((Axiscommand->limit_z <= max_z/2)||(Axiscommand->limit_z  >= max_z)){
		Axiscommand->limit_z =  max_z;
	}
	return (Axiscommand->limit_z );
}



// taskbar
void Handle_motor(void){
	uint16_t builtin_Handle_motor = __builtin_ffs(Motor_control->all);
		if (builtin_Handle_motor > 0) {
			builtin_Handle_motor -= 1;
		    if (builtin_Handle_motor < (int)(sizeof(Tab_main_table) / sizeof(Tab_main_table[0]))) {
		    	Tab_main_table[builtin_Handle_motor]();
		    }
		}
}

uint32_t output_shadow = 0 ;

void Handle_output(void){
	uint32_t current = Output_control->all;
	uint32_t changed = current ^ output_shadow;
	    if(changed == 0)
	        return;
	    for(uint8_t i = 0; i < 8; i++)
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

void Handle_homeprocess(void){
	if((AxisX.mode == STOP) &&(AxisY.mode == STOP) && (AxisZ.mode == STOP)){
		Input_indicator->bits.motor_state = NOT_YET;
		Input_indicator->bits.home_state = NOT_YET;

		if(get_home_z() == home_z){
		  AxisZ.mode = MOVE_HOME2;
		}else {
		  AxisZ.mode = MOVE_HOME1;
		}
		Home_process_z();
		while((AxisZ.mode != MOVE_HOME3));
		while((AxisZ.mode != STOP));

		if(get_home_x() == home_x){
		  AxisX.mode = MOVE_HOME2;
		}else{
		  AxisX.mode = MOVE_HOME1;
		}
		Home_process_x();
		if(get_home_y() == home_y){
		  AxisY.mode = MOVE_HOME2;
		}else{
		  AxisY.mode = MOVE_HOME1;
		}
		Home_process_y();
		wait_handler_stop();
	}
}

void Handle_setpoint(void){
	Motor_control->bits.Set_point = 0 ;
	if((AxisX.mode == STOP) &&(AxisY.mode == STOP) && (AxisZ.mode == STOP)){
	move_axis(Get_PC_Position_Axis_X(),Get_PC_Position_Axis_Y(), Get_PC_Position_Axis_Z());
	if((AxisX.mode == STOP) &&(AxisY.mode == STOP) && (AxisZ.mode == STOP)){
		Input_indicator->bits.motor_state = DONE ;
	}else{
		Input_indicator->bits.motor_state = NOT_YET ;
	}	
	}
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
	if(AxisX.current_pos >= Get_PC_Limit_Axis_X()){
		return;
	}
	if(AxisX.mode == STOP) {
	  AxisX.mode =  MOVE_MANUAL;
	  move_x_right((uint16_t)Get_PC_Limit_Axis_X() - AxisX.current_pos);
	}

}
void Handle_Y_Forward(void){
	if(AxisY.current_pos >= (uint16_t)Get_PC_Limit_Axis_Y()) {
		return;
		}
	if(AxisY.mode == STOP){
		AxisY.mode = MOVE_MANUAL;
		move_y_forward((uint16_t)Get_PC_Limit_Axis_Y()- AxisY.current_pos);
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
	if(AxisZ.current_pos >= (uint16_t)Get_PC_Limit_Axis_Z()  ){
			return ;
		}
	if(AxisZ.mode == STOP) {
		AxisZ.mode = MOVE_MANUAL;
		move_z_down((uint16_t)Get_PC_Limit_Axis_Z()-AxisZ.current_pos);
	}
}






