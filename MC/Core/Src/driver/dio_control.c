/*
 * IO_Controller.c
 *
 *  Created on: Jan 14, 2026
 *      Author: CongChuc
 */


#include <dio_control.h>
uint8_t NumberBips = 0;
uint8_t modeBip = 1;
uint16_t TIME_SET_BIP_ON = 500;
uint16_t TIME_SET_BIP_OFF = 500;
uint16_t TimeDelayBip = 0;

Cylinder_Vacum_Init_t Handle_Pick[2];
Cylinder_Vacum_Init_t Handle_Release[2];

void SetBips(uint8_t numBips){
	NumberBips = numBips - 1;
}

void BipControl(void){
	switch(modeBip){
		case 0:
			ON_BUZZ;
			TimeDelayBip = TIME_SET_BIP_ON;
			modeBip = 1;
			break;
		case 1:
			if(TimeDelayBip == 0){
				OFF_BUZZ;
				TimeDelayBip = TIME_SET_BIP_OFF;
				modeBip = 2;
			}
			break;
		case 2:
			if(TimeDelayBip == 0){
				modeBip = 0;
				if(NumberBips > 0) NumberBips--;
				else modeBip = 3;
			}
			break;
		case 3:
			if(NumberBips != 0)
				modeBip = 0;
			break;
		default:
			break;
	}
	if(TimeDelayBip > 0) TimeDelayBip--;
}

void SetPickRubber(uint8_t vacum_id) // 0 or 1
{
    if (vacum_id >= 2) return;

    Handle_Pick[vacum_id].vacum_id   = vacum_id;
    Handle_Pick[vacum_id].retry      = 0;
    Handle_Pick[vacum_id].time_delay = 0;
    Handle_Pick[vacum_id].result     = RUNNING;
    Handle_Pick[vacum_id].state      = CYLINDER_GO_DOWN;
}

void SetReleaseRubber(uint8_t vacum_id)   // vacum_id: 0 or 1
{
    if (vacum_id >= 2) return;
    Handle_Release[vacum_id].vacum_id   = vacum_id;
    Handle_Release[vacum_id].retry      = 0;
    Handle_Release[vacum_id].time_delay = 0;
    Handle_Release[vacum_id].result     = RUNNING;
    Handle_Release[vacum_id].state      = CYLINDER_GO_DOWN;
}



