/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "application.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
uint32_t Tick = 0;
volatile uint32_t lastFrameTime = 0;
volatile ModbusTransport_t activeTransport = MODBUS_NONE;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern Axis_t AxisX, AxisY, AxisZ;
extern  uint8_t RxData[256];
extern volatile SystemFlag_t SystemFlag;
extern Input_indicator_t* Input_indicator;
extern volatile ModbusTransport_t activeTransport;
//extern Tab_main_t* Tab_main;
//extern Tab_main_t* Tab_main_indicator ;
//extern Taskbar_t* Taskbar;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim9;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */
	Input_indicator->bits.sensor_homex =  get_home_x();
	if(get_home_x() == home_x){
		switch(AxisX.mode){
		case MOVE_HOME1:
			output_x_pull_stop();
			reset_counter_timer_slave_x();
			AxisX.mode = MOVE_HOME2;
			Home_process_x();
			AxisX.current_pos = 0;
			Set_PC_Position_Axis_X(AxisX.current_pos);
			AxisX.old_pos = 0;
			break;
		case MOVE_HOME3:
			output_x_pull_stop();
			reset_counter_timer_slave_x();
			AxisX.current_pos = 0;
			Set_PC_Position_Axis_X(AxisX.current_pos);
			AxisX.mode  = STOP;
			AxisX.old_pos = 0;
			break;
	  }
 }
  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(i1_home_x_Pin);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */
  	Input_indicator->bits.sensor_homey =  get_home_y();
	if(get_home_y() == home_y){
		switch(AxisY.mode){
		case MOVE_HOME1:
			output_y_pull_stop();
			reset_counter_timer_slave_y();
			AxisY.mode = MOVE_HOME2;
			Home_process_y();
			AxisY.current_pos = 0;
			Set_PC_Position_Axis_Y(AxisY.current_pos);
			AxisY.old_pos = 0;
			break;
		case MOVE_HOME3:
			output_y_pull_stop();
			reset_counter_timer_slave_y();
			AxisY.current_pos = 0;
			Set_PC_Position_Axis_Y(AxisY.current_pos);
			AxisY.mode  = STOP;
			AxisY.old_pos = 0;
			break;
	  }
  }

  /* USER CODE END EXTI2_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(i2_home_y_Pin);
  /* USER CODE BEGIN EXTI2_IRQn 1 */

  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */
	Input_indicator->bits.sensor_homez =  get_home_z();
	if(get_home_z() == home_z){
	switch(AxisZ.mode){
		case MOVE_HOME1:
			output_z_pull_stop();
			reset_counter_timer_slave_z();
			AxisZ.mode = MOVE_HOME2;
			Home_process_z();
			AxisZ.current_pos = 0;
			Set_PC_Position_Axis_Z(AxisZ.current_pos);
			AxisZ.old_pos = 0;
		  break;
		case MOVE_HOME3:
			output_z_pull_stop();
			reset_counter_timer_slave_z();
			AxisZ.current_pos = 0;
			Set_PC_Position_Axis_Z(AxisZ.current_pos);
			AxisZ.mode  = STOP;
			AxisZ.old_pos = 0;
		  break;
	  }
	}
  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(i3_home_z_Pin);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

  /* USER CODE END DMA1_Stream5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
  /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

  /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
  * @brief This function handles TIM1 break interrupt and TIM9 global interrupt.
  */
void TIM1_BRK_TIM9_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_TIM9_IRQn 0 */
  /* USER CODE END TIM1_BRK_TIM9_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  HAL_TIM_IRQHandler(&htim9);
  /* USER CODE BEGIN TIM1_BRK_TIM9_IRQn 1 */
	if(AxisZ.mode == MOVE_MANUAL){
		AxisZ.mode = MOVE_AUTO;}
		Stop_motor_z();

  /* USER CODE END TIM1_BRK_TIM9_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */
	if(AxisY.mode == MOVE_MANUAL){
		AxisY.mode = MOVE_AUTO;}
		Stop_motor_y();

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(i13_start_Pin);
  HAL_GPIO_EXTI_IRQHandler(i14_stop_Pin);
  HAL_GPIO_EXTI_IRQHandler(i15_reset_Pin);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM5_IRQn 0 */
	if(AxisX.mode == MOVE_MANUAL){
		AxisX.mode = MOVE_AUTO;}
		Stop_motor_x();
  /* USER CODE END TIM5_IRQn 0 */
  HAL_TIM_IRQHandler(&htim5);
  /* USER CODE BEGIN TIM5_IRQn 1 */

  /* USER CODE END TIM5_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
	task_timer6();
  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */
  Tick++;
  task_timer7();
  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles USB On The Go FS global interrupt.
  */
void OTG_FS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_FS_IRQn 0 */

  /* USER CODE END OTG_FS_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
  /* USER CODE BEGIN OTG_FS_IRQn 1 */

  /* USER CODE END OTG_FS_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	if(huart-> Instance == USART2){
		if (activeTransport == MODBUS_NONE)
		{
			activeTransport  = MODBUS_UART;
		}
		lastFrameTime = Tick + MODBUS_TIMEOUT_MS;
		process_data();
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxData, 256);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == i13_start_Pin ){
		Input_indicator->bits.start =  HAL_GPIO_ReadPin(i13_start_GPIO_Port, i13_start_Pin);
	}else if(GPIO_Pin == i14_stop_Pin){
		Input_indicator->bits.stop = HAL_GPIO_ReadPin(i14_stop_GPIO_Port, i14_stop_Pin);
	}else if(GPIO_Pin == i15_reset_Pin){
		Input_indicator->bits.reset = HAL_GPIO_ReadPin(i15_reset_GPIO_Port, i15_reset_Pin);
	}
}
/*
 * 	Input_indicator->bits.sensor_homex =  get_home_x();
	if(get_home_x() == home_x){
		switch(AxisX.mode){
		case MOVE_HOME1:
			output_x_pull_stop();
			reset_counter_timer_slave_x();
			AxisX.mode = MOVE_HOME2;

			Set_Speed_Motor_x(speed_home2_x, speed_x_max);
			move_x_right(2000);

			AxisX.current_pos = 0;
			Set_PC_Position_Axis_X(AxisX.current_pos);
			AxisX.old_pos = 0;
			break;
		case MOVE_HOME3:
			output_x_pull_stop();
			reset_counter_timer_slave_x();
			AxisX.current_pos = 0;
			Set_PC_Position_Axis_X(AxisX.current_pos);
			AxisX.mode  = STOP;
			AxisX.old_pos = 0;
			break;
	  }

	  	Input_indicator->bits.sensor_homey =  get_home_y();
	if(get_home_y() == home_y){
		switch(AxisY.mode){
		case MOVE_HOME1:
			output_y_pull_stop();
			reset_counter_timer_slave_y();
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
			AxisY.current_pos = 0;
			Set_PC_Position_Axis_Y(AxisY.current_pos);
			AxisY.old_pos = 0;
			break;
		case MOVE_HOME3:
			output_y_pull_stop();
			reset_counter_timer_slave_y();
			AxisY.current_pos = 0;
			Set_PC_Position_Axis_Y(AxisY.current_pos);
			AxisY.mode  = STOP;
			AxisY.old_pos = 0;
			break;
	  }


	  Input_indicator->bits.sensor_homez =  get_home_z();
	 if(get_home_z() == home_z){
	switch(AxisZ.mode){
		case MOVE_HOME1:
			output_z_pull_stop();
			reset_counter_timer_slave_z();
			AxisZ.mode = MOVE_HOME2;


			//	case MOVE_HOME1:
			//		Set_Speed_Motor_z(speed_home1_z, speed_z_max);
			//		// cộng thêm tránh trư�?ng hợp đi từ max vào
			//		move_z_up(max_z + 1000);
			//		break;
			//	case MOVE_HOME2:
			//		if( get_counter_timer_slave_z() == 0 ){
					Set_Speed_Motor_z(speed_home2_z, speed_z_max);
					move_z_down(2500);
			//		}
			//		break;



			AxisZ.current_pos = 0;
			Set_PC_Position_Axis_Z(AxisZ.current_pos);
			AxisZ.old_pos = 0;
		  break;
		case MOVE_HOME3:
			output_z_pull_stop();
			reset_counter_timer_slave_z();
			AxisZ.current_pos = 0;
			Set_PC_Position_Axis_Z(AxisZ.current_pos);
			AxisZ.mode  = STOP;
			AxisZ.old_pos = 0;
		  break;
	  }
 * */
/* USER CODE END 1 */
