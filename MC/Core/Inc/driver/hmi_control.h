/*
 * hmi_control.h
 *
 *  Created on: Jan 24, 2026
 *      Author: Admin
 */

#ifndef INC_DRIVER_HMI_CONTROL_H_
#define INC_DRIVER_HMI_CONTROL_H_

#include "dio_control.h"
#include "modbus_rtu.h"
#include "main.h"
#include "flash.h"
#include "stdbool.h"
#include "cmsis_gcc.h"
#include "motor_control.h"



typedef struct
{
    uint8_t is_homing : 1;   // Đang về HOME
    uint8_t is_stop   : 1;   // Dừng hệ thống
    uint8_t is_start  : 1;   // Bắt đầu chạy
    uint8_t is_err	  : 1; 	 // Gắp sịt
    uint8_t reserved  : 4;   // Dự phòng (bắt buộc để đủ 1 byte)
} SystemFlag_t;



typedef union {
    struct {
    	uint8_t Home_process		: 1;
        uint8_t Set_point			: 1;
        uint8_t x_left 				: 1;
        uint8_t x_right 			: 1;
        uint8_t y_backward 			: 1;
        uint8_t y_forward 			: 1;
        uint8_t z_up 				: 1;
        uint8_t z_down 				: 1;
    } bits;
    uint8_t all;
}Motor_control_t;

typedef union {
    struct {
    	uint8_t output1			: 1;
        uint8_t output2			: 1;
        uint8_t output3 		: 1;
        uint8_t output4			: 1;
        uint8_t output5 		: 1;
        uint8_t output6 		: 1;
        uint8_t output7 		: 1;
        uint8_t output8 		: 1;
    } bits;
    uint8_t all;
}Output_control_t;

typedef union {
    struct {
    	uint8_t sensor_homex        : 1;
        uint8_t sensor_homey		: 1;
        uint8_t sensor_homez 		: 1;
        uint8_t start 				: 1;
        uint8_t stop 				: 1;
        uint8_t reset 				: 1;
        uint8_t home_state 			: 1;
        uint8_t motor_state 		: 1;
    } bits;
    uint8_t all;
}Input_indicator_t;


typedef struct{
	uint16_t pos_x;
	uint16_t speed_x;
	uint16_t state_x;
	uint16_t pos_y;
	uint16_t speed_y;
	uint16_t state_y;
	uint16_t pos_z;
	uint16_t speed_z;
	uint16_t state_z;
}Axisfeedback_t;

typedef struct{
	uint16_t target_x;
	uint16_t speed_x;
	uint16_t limit_x;
	uint16_t target_y;
	uint16_t speed_y;
	uint16_t limit_y;
	uint16_t target_z;
	uint16_t speed_z;
	uint16_t limit_z;
}Axiscommand_t;




#define Mark_all_rubber()  		(SetBit(&Inputs_Database[3], 0, 200))
#define Mark_rubber(f) 			(SetBit(&Inputs_Database[3], f, 1))
#define Clear_mark_rubber(f) 	(ClearBit(&Inputs_Database[3], f, 1))

#define Clear_all_tray1() 	(ClearBit(&Inputs_Database[28], 0, 24))
#define Mark_tray1(f) 			(SetBit(&Inputs_Database[28], f, 1))
#define Clear_mark_tray1(f) 	(ClearBit(&Inputs_Database[28], f, 1))

#define Clear_all_tray2() 	(ClearBit(&Inputs_Database[31], 0, 24))
#define Mark_tray2(f) 			(SetBit(&Inputs_Database[31], f, 1))
#define Clear_mark_tray2(f) 	(ClearBit(&Inputs_Database[31], f, 1))

#define Mark_rubber_working(f)  Input_Registers_Database[0] =  f+1
#define Mark_tray1_working(f)  Input_Registers_Database[1] =  f+1
#define Mark_tray2_working(f)  Input_Registers_Database[2] =  f+1

#define Close_Popup(f) 		(ClearBit(&Inputs_Database[34], f, 1))
#define Open_Popup(f) 			(SetBit(&Inputs_Database[34], f, 1))
#define Read_Warring(f)        (ReadBit(&Inputs_Database[34], f, 1))

#define popup_err  0
#define popup_door 1
#define popup_home 2

#define Clear_Rubber_and_tray_indicator() Rubber_and_tray_indicator->all &= (~0x1ff)

void Set_PC_Position_Axis_X(uint16_t value);
void Set_PC_Position_Axis_Y(uint16_t value);
void Set_PC_Position_Axis_Z(uint16_t value);
uint16_t Get_PC_Position_Axis_X(void);
uint16_t Get_PC_Position_Axis_Y(void);
uint16_t Get_PC_Position_Axis_Z(void);

void Set_PC_Speed_Axis_X(uint16_t value);
void Set_PC_Speed_Axis_Y(uint16_t value);
void Set_PC_Speed_Axis_Z(uint16_t value);
uint16_t Get_PC_Speed_Axis_X(void);
uint16_t Get_PC_Speed_Axis_Y(void);
uint16_t Get_PC_Speed_Axis_Z(void);

void Set_PC_State_Axis_X(uint16_t value);
void Set_PC_State_Axis_Y(uint16_t value);
void Set_PC_State_Axis_Z(uint16_t value);
uint16_t  Get_PC_Limit_Axis_X(void);
uint16_t  Get_PC_Limit_Axis_Y(void);
uint16_t  Get_PC_Limit_Axis_Z(void);




void Handle_motor(void);
void Handle_output(void);

void Handle_homeprocess(void);
void Handle_setpoint(void);
void Handle_X_Left (void);
void Handle_X_Right (void);
void Handle_Y_Forward(void);
void Handle_Y_Backward(void);
void Handle_Z_Up(void);
void Handle_Z_Down(void);


#endif /* INC_DRIVER_HMI_CONTROL_H_ */
