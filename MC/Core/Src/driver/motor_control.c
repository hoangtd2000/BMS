/*
 * motor_control.c
 *
 *  Created on: Oct 17, 2025
 *      Author: TranHoang
 */

#include <dio_control.h>
#include "motor_control.h"

#define truc_z_len_het
#define truc_z_len_sau

extern Motor_control_t* Motor_control;
extern Axiscommand_t* Axiscommand;
extern Input_indicator_t* Input_indicator;
Axis_t AxisX = {
		.dir =  DIR_POS,
		.mode = STOP,
		.pre_mode = STOP,
		.current_pos = 0 ,
		.old_pos = 0,
};

Axis_t AxisY = {
		.dir =  DIR_POS,
		.mode = STOP,
		.pre_mode = STOP,
		.current_pos = 0 ,
		.old_pos = 0,
};
Axis_t AxisZ = {
		.dir =  DIR_POS,
		.mode = STOP,
		.pre_mode = STOP,
		.current_pos = 0 ,
		.old_pos = 0,
};


void Set_Speed_Motor_x(uint16_t f, uint16_t f_max){
	if((f <= 0)||(f > f_max)) f = f_max;
	set_speed_x(period_x/f);
	set_pulse_x(period_x/f/2);
}
void Set_Speed_Motor_y(uint16_t f, uint16_t f_max){
	if((f <= 0)||(f > f_max)) f = f_max;
	set_speed_y(period_y/f);
	set_pulse_y(period_y/f/2);
}
void Set_Speed_Motor_z(uint16_t f, uint16_t f_max){
	if((f <= 0)||(f > f_max)) f = f_max;
	set_speed_z(period_z/f);
	set_pulse_z(period_z/f/2);
}


void excute_move_x_left(void){
	output_x_sig_left();
	output_x_pull_start();
}
void excute_move_x_right(void){
	output_x_sig_right();
	output_x_pull_start();
}
void excute_move_y_forward(void){
	output_y_sig_forward();
	output_y_pull_start();
}
void excute_move_y_backward(void){
	output_y_sig_backward();
	output_y_pull_start();
}
void excute_move_z_up(void){
	output_z_sig_up();
	output_z_pull_start();
}
void excute_move_z_down(void){
	output_z_sig_down();
	output_z_pull_start();
}


void move_x_left(uint16_t d){
	AxisX.dir = DIR_NEG;
	reset_counter_timer_x();
	set_x_target_pull(d-1);
	excute_move_x_left();
}


void move_x_right(uint16_t d){
	AxisX.dir = DIR_POS;
	reset_counter_timer_x();
	set_x_target_pull(d-1);
	excute_move_x_right();
}

void move_y_forward(uint16_t d){
	AxisY.dir = DIR_POS;
	reset_counter_timer_y();
	set_y_target_pull(d-1);
	excute_move_y_forward();
}

void move_y_backward(uint16_t d){
	AxisY.dir = DIR_NEG;
	reset_counter_timer_y();
	set_y_target_pull(d-1);
	excute_move_y_backward();
}

void move_z_up(uint16_t d){
	AxisZ.dir = DIR_NEG;
	reset_counter_timer_z();
	set_z_target_pull(d-1);
	excute_move_z_up();
}

void move_z_down(uint16_t d){
	AxisZ.dir = DIR_POS;
	reset_counter_timer_z();
	set_z_target_pull(d-1);
	excute_move_z_down();
}

static inline uint16_t abs_diff_u16(uint16_t a, uint16_t b)
{
    return (a > b) ? (a - b) : (b - a);
}

// move z -> x,y
void move_axis(uint16_t xd, uint16_t yd, uint16_t zd)
{
    /* Check giới hạn */
    if (xd > Get_PC_Limit_Axis_X() || yd > Get_PC_Limit_Axis_Y() || zd > Get_PC_Limit_Axis_Z()) {
        return;
    }

//#ifdef truc_z_len_het
//    while(AxisZ.mode != STOP);
//#endif

    /* ================= X AXIS ================= */
    uint16_t dx = abs_diff_u16(AxisX.current_pos, xd);
    if (dx > 1 && AxisX.mode == STOP) {
        Set_Speed_Motor_x(Axiscommand->speed_x, speed_x_max);
        AxisX.mode = MOVE_AUTO;

        if (AxisX.current_pos > xd) {
            move_x_left(dx);
        } else {
            move_x_right(dx);
        }
    }

    /* ================= Y AXIS ================= */
    uint16_t dy = abs_diff_u16(AxisY.current_pos, yd);
    if (dy > 1 && AxisY.mode == STOP) {
        Set_Speed_Motor_y(Axiscommand->speed_y, speed_y_max);
        AxisY.mode = MOVE_AUTO;

        if (AxisY.current_pos > yd) {
            move_y_backward(dy);
        } else {
            move_y_forward(dy);
        }
    }
    while ( !(AxisX.mode == STOP && AxisY.mode == STOP ));
    /* ================= Z AXIS ================= */
    uint16_t dz = abs_diff_u16(AxisZ.current_pos, zd);
    if (dz > 1 && AxisZ.mode == STOP) {
        Set_Speed_Motor_z(Axiscommand->speed_z,speed_z_max);
        AxisZ.mode = MOVE_AUTO;

        if (AxisZ.current_pos > zd) {
            move_z_up(dz);
        } else {
            move_z_down(dz);
        }
    }
}

// move x,y -> z
void move_axis1(uint16_t xd, uint16_t yd, uint16_t zd)
{
    /* Check giới hạn */
    if (xd > max_x || yd > max_y || zd > max_z) {
        return;
    }

    /* ================= X AXIS ================= */
    uint16_t dx = abs_diff_u16(AxisX.current_pos, xd);
    if (dx > 1 && AxisX.mode == STOP) {
        Set_Speed_Motor_x(speed_run, speed_x_max);
        AxisX.mode = MOVE_AUTO;

        if (AxisX.current_pos > xd) {
            move_x_left(dx);
        } else {
            move_x_right(dx);
        }
    }

    /* ================= Y AXIS ================= */
    uint16_t dy = abs_diff_u16(AxisY.current_pos, yd);
    if (dy > 1 && AxisY.mode == STOP) {
        Set_Speed_Motor_y(speed_run, speed_y_max);
        AxisY.mode = MOVE_AUTO;

        if (AxisY.current_pos > yd) {
            move_y_backward(dy);
        } else {
            move_y_forward(dy);
        }
    }
    while ( !(AxisX.mode == STOP && AxisY.mode == STOP ));

    /* ================= Z AXIS ================= */
    uint16_t dz = abs_diff_u16(AxisZ.current_pos, zd);
    if (dz > 1 && AxisZ.mode == STOP) {
        Set_Speed_Motor_z(speed_run_z, speed_z_max);
        AxisZ.mode = MOVE_AUTO;

        if (AxisZ.current_pos > zd) {
            move_z_up(dz);
        } else {
            move_z_down(dz);
        }
    }
}

void Control_motor_x(){
	Set_PC_State_Axis_X(AxisX.mode);
	switch(AxisX.mode) {
	case MOVE_AUTO:
		AxisX.current_pos = AxisX.old_pos + (AxisX.dir*( get_counter_timer_slave_x() ) );
		Set_PC_Position_Axis_X(AxisX.current_pos);
		//Set_PC_Speed_Axis_X(get_speed_x() * 1000);
		break;
	case MOVE_MANUAL:
		AxisX.current_pos = AxisX.old_pos + (AxisX.dir*( get_counter_timer_slave_x()  ) );
		Set_Speed_Motor_x( (get_counter_timer_slave_x()) + speed_default, speed_x_max);
		Set_PC_Position_Axis_X(AxisX.current_pos);
		if((Motor_control->bits.x_left == 0 ) && (Motor_control->bits.x_right == 0 )){
			if(get_counter_timer_x() >= get_pulse_x() ){
			Stop_motor_x();
			}
		}
		break;

	case MOVE_HOME3:
		Set_Speed_Motor_x(speed_home3_x, speed_x_max);
		move_x_left(max_x);
		break;
	case STOP:
		break;
	}
}
void Home_process_x(void){
	switch(AxisX.mode) {
		case MOVE_HOME1:
			Set_Speed_Motor_x(speed_home1_x, speed_x_max);
			move_x_left(max_x+1000);
			break;
		case MOVE_HOME2:
			Set_Speed_Motor_x(speed_home2_x, speed_x_max);
			move_x_right(2000);
			break;
		case MOVE_HOME3:
			Set_Speed_Motor_x(speed_home3_x, speed_x_max);
			move_x_left(max_x);
			break;
		case STOP:

			break;
		}
}


void Control_motor_y(){
	Set_PC_State_Axis_Y(AxisY.mode);
	switch(AxisY.mode) {

	case MOVE_AUTO:
		AxisY.current_pos = AxisY.old_pos + (AxisY.dir*( get_counter_timer_slave_y()  ) );
		Set_PC_Position_Axis_Y(AxisY.current_pos);
	//	Set_PC_Speed_Axis_Y(get_speed_y() * 1000);

		break;
	case MOVE_MANUAL:
		AxisY.current_pos = AxisY.old_pos + (AxisY.dir*( get_counter_timer_slave_y()  ) );
		Set_PC_Position_Axis_Y(AxisY.current_pos);
		Set_Speed_Motor_y( (get_counter_timer_slave_y()) + speed_default, speed_y_max);
	//	Set_PC_Speed_Axis_Y(get_speed_y() * 1000);
		if((Motor_control->bits.y_forward == 0 ) && (Motor_control->bits.y_backward == 0 )){
			if(get_counter_timer_y() >= get_pulse_y() ){
					Stop_motor_y();
					}
				}
		break;

	case MOVE_HOME3:
		Set_Speed_Motor_y(speed_home3_y, speed_y_max);
		move_y_backward(max_y );
		break;
	case STOP:
		break;
	}
}
void Home_process_y(void){
	switch(AxisY.mode) {
		case MOVE_HOME1:
			Set_Speed_Motor_y(speed_home1_y, speed_y_max);
			move_y_backward(max_y + 500);
			break;
		case MOVE_HOME2:
			Set_Speed_Motor_y(speed_home2_y, speed_y_max);
			move_y_forward(2000);
			break;
		case MOVE_HOME3:
			Set_Speed_Motor_y(speed_home3_y, speed_y_max);
			move_y_backward(max_y );
			break;
		case STOP:
			break;
		}
}
void Control_motor_z(){
	Set_PC_State_Axis_Z(AxisZ.mode);
	switch(AxisZ.mode) {
	case MOVE_AUTO:
		AxisZ.current_pos = AxisZ.old_pos + (AxisZ.dir*(get_counter_timer_slave_z() ) );
		Set_PC_Position_Axis_Z(AxisZ.current_pos);
	//	Set_PC_Speed_Axis_Z(get_speed_z() * 1000);
		break;
	case MOVE_MANUAL:
		AxisZ.current_pos = AxisZ.old_pos + (AxisZ.dir*(get_counter_timer_slave_z() ) );
		Set_PC_Position_Axis_Z(AxisZ.current_pos);
		Set_Speed_Motor_z((get_counter_timer_slave_z()) + speed_default, speed_z_max);
	//	Set_PC_Speed_Axis_Z(get_speed_z() * 1000);
		if((Motor_control->bits.z_up == 0 ) && (Motor_control->bits.z_down == 0 )){
			if(get_counter_timer_z() >= get_pulse_z() ){
				Stop_motor_z();
					}
				}
		break;

	case MOVE_HOME3:
		Set_Speed_Motor_z(speed_home3_z, speed_z_max);
		move_z_up(max_z);
		break;
	case STOP:
		break;
	}
}
void Home_process_z(void){
	switch(AxisZ.mode) {
		case MOVE_HOME1:
			Set_Speed_Motor_z(speed_home1_z, speed_z_max);
			// cộng thêm tránh trường hợp đi từ max vào
			move_z_up(max_z + 1000);
			break;
		case MOVE_HOME2:
		//	if( get_counter_timer_slave_z() == 0 ){
			Set_Speed_Motor_z(speed_home2_z, speed_z_max);
			move_z_down(2500);
			//}
			break;
		case MOVE_HOME3:
			Set_Speed_Motor_z(speed_home3_z, speed_z_max);
			move_z_up(max_z);
			break;
		case STOP:
			break;
		}
}
void Stop_motor_x(void)
{
    output_x_pull_stop();
    switch (AxisX.mode) {
    case MOVE_MANUAL:
        AxisX.current_pos =
            AxisX.old_pos + (AxisX.dir * get_counter_timer_slave_x());
        Set_PC_Position_Axis_X(AxisX.current_pos);
        reset_counter_timer_slave_x();
        break;

    case MOVE_AUTO:
        AxisX.current_pos =
            AxisX.old_pos + (AxisX.dir * (get_x_target_pull() + 1));
        Set_PC_Position_Axis_X(AxisX.current_pos);
        break;
    case MOVE_HOME2:
        AxisX.mode = MOVE_HOME3;
        return;   // thoát hàm, KHÔNG set STOP (giữ đúng logic cũ)
    default:
        break;
    }

    AxisX.mode = STOP;
	Set_PC_State_Axis_X(AxisX.mode);
    AxisX.old_pos = AxisX.current_pos;
}


void Stop_motor_y(void)
{
    output_y_pull_stop();
    switch (AxisY.mode) {
    case MOVE_MANUAL:
        AxisY.current_pos =
            AxisY.old_pos + (AxisY.dir * get_counter_timer_slave_y());
        Set_PC_Position_Axis_Y(AxisY.current_pos);
        reset_counter_timer_slave_y();
        break;
    case MOVE_AUTO:
        AxisY.current_pos =
            AxisY.old_pos + (AxisY.dir * (get_y_target_pull() + 1));
        Set_PC_Position_Axis_Y(AxisY.current_pos);
        break;
    case MOVE_HOME2:
        AxisY.mode = MOVE_HOME3;
        return;
    default:
        break;
    }
    AxisY.mode = STOP;
    Set_PC_State_Axis_Y(AxisY.mode);
    AxisY.old_pos = AxisY.current_pos;
}


void Stop_motor_z(void)
{
    output_z_pull_stop();
    switch (AxisZ.mode) {
    case MOVE_MANUAL:
        AxisZ.current_pos =
            AxisZ.old_pos + (AxisZ.dir * get_counter_timer_slave_z());
        Set_PC_Position_Axis_Z(AxisZ.current_pos);
        reset_counter_timer_slave_z();
        break;
    case MOVE_AUTO:
        AxisZ.current_pos =
            AxisZ.old_pos + (AxisZ.dir * (get_z_target_pull() + 1));
        Set_PC_Position_Axis_Z(AxisZ.current_pos);
        break;
    case MOVE_HOME2:
        AxisZ.mode = MOVE_HOME3;
        return;
    default:
        break;
    }
    AxisZ.mode = STOP;
    Set_PC_State_Axis_Z(AxisZ.mode);
    AxisZ.old_pos = AxisZ.current_pos;
}

static inline bool AllAxisStop(void)
{
    return (AxisX.mode == STOP &&
            AxisY.mode == STOP &&
            AxisZ.mode == STOP);
}

void wait_handler_stop(){
	while (!AllAxisStop())
	{
		__NOP();
	}
	Input_indicator->bits.home_state  = DONE ;
	Input_indicator->bits.motor_state = DONE ;
}
