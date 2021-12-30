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
TaskHandle_t uart_com_handle;
TaskHandle_t heartbeat_handle;


void MotorPidUpdateTask( void* pvParameters );
void SpiCommunicationTask( void* pvParameters );
void UartCommunicationTask( void* pvParameters );
void HeartbeatTask( void* pvParameters );

void setup_tasks(){
	BaseType_t status;
	status = xTaskCreate( MotorPidUpdateTask, "MotorPidUpdate", 128,
				 NULL, tskIDLE_PRIORITY, &motor_pid_update_handle
				);
    status = xTaskCreate( UartCommunicationTask, "UartCom", 128,
				 NULL, tskIDLE_PRIORITY, &uart_com_handle
				);
    status = xTaskCreate( HeartbeatTask, "Heartbeat", 128,
				 NULL, tskIDLE_PRIORITY, &heartbeat_handle
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
    
    HAL_SPI_TransmitReceive_IT( board.spi_handle, &(tx_buffer[0]), &(rx_buffer[0]), command_size);
    for (;;){
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        board.ExecuteCommand(rx_buffer, tx_buffer);
        HAL_SPI_TransmitReceive_IT( board.spi_handle, &(tx_buffer[0]), &(rx_buffer[0]), command_size);
    }
}


void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef * hspi){
    if( hspi == board.spi_handle ){
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR( spi_com_handle, &xHigherPriorityTaskWoken );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}



void UartCommunicationTask( void* pvParameters ){
    
    CommandBuffer rx_buffer;
    ResponseBuffer tx_buffer;
    
    HAL_UART_Transmit(&huart1, &(tx_buffer[0]), command_size, 10000 );


    HAL_UART_Receive_IT( &huart1, &(rx_buffer[0]), command_size );

    for(;;){
        // wait for signal from the UART finished interrupt handler and then process command
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        board.ExecuteCommand(rx_buffer, tx_buffer);
        HAL_UART_Transmit_IT(&huart1, &(tx_buffer[0]), command_size );
        HAL_UART_Receive_IT( &huart1, &(rx_buffer[0]), command_size );

    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if( huart->Instance == USART1){
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR( uart_com_handle, &xHigherPriorityTaskWoken );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}


void HeartbeatTask( void* pvParameters ){
    constexpr TickType_t delay_led = 500 / portTICK_PERIOD_MS;

    for(;;){
        HAL_GPIO_WritePin(LED_STATUS_GPIO_Port,LED_STATUS_Pin,GPIO_PIN_SET);
        vTaskDelay(delay_led);
        HAL_GPIO_WritePin(LED_STATUS_GPIO_Port,LED_STATUS_Pin,GPIO_PIN_RESET);
        vTaskDelay(delay_led);
    }
}