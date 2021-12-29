/*
 * Test.cpp
 *
 *  Created on: Nov 13, 2021
 *      Author: Christian Koernig
 */
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"

#include "cpp_bridge.h"
#include "TB6612.hpp"
#include "SojournerST.hpp"
#include "SojournerSTConfig.hpp"


SojournerST board;
TaskHandle_t motor_pid_update_handle;
TaskHandle_t spi_com_handle;

void MotorPidUpdateTask( void* pvParameters );
void SpiCommunicationTask( void* pvParameters );

void setup_tasks(){
	BaseType_t status;
	status = xTaskCreate( MotorPidUpdateTask, "MotorPidUpdate", 128,
				 NULL, tskIDLE_PRIORITY, &motor_pid_update_handle
				);
}
void entrypoint(){

	
	setup_tasks();
	vTaskStartScheduler();
}


/**
 * FreeRTOS task to run the PID loop for the motor controllers
 */
void MotorPidUpdateTask( void* pvParameters ){

    constexpr TickType_t delay_pid = PID_UPDATE_RATE_MS / portTICK_PERIOD_MS;

    for(;;)
    {
        board.UpdatePidLoop(delay_pid);
        vTaskDelay(delay_pid);
    }
}



void SpiCommunicationTask( void* pvParameters ){
    
    CommandBuffer rx_buffer;
    ResponseBuffer tx_buffer;
    
    HAL_SPI_TransmitReceive_IT( board.spi_handle, &(tx_buffer[0]), &(pRxData[0]), command_size);
    for (;;){
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        board.ExecuteCommand(rx_buffer, tx_buffer);
        HAL_SPI_TransmitReceive_IT( board.spi_handle, &(tx_buffer[0]), &(pRxData[0]), command_size);
    }
}


void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef * hspi){
    if( hspi->Instance == board.spi_handle ){
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR( spi_com_handle, &xHigherPriorityTaskWoken );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}