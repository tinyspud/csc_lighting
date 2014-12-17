/*
 * GPSTask.c
 *
 *  Created on: Jan 17, 2014
 *      Author: rsnoo_000
 */

/* SafeRTOS Includes */
#include "GPSTask.h"
#include "LogTask.h"
#include "mcs_time.h"

//#include <stdint.h>
//#include <string.h>
//#include <stdio.h>



//#ifdef GPS_MODULE
#include "gps.h"
//#endif

//static const uint8_t instr[69] ={'$','G','P','R','M','C',',',
//							'0','5','3','7','4','0','.','0','0','0',',',
//							'A',',',
//							'2','5','0','3','.','6','3','1','9',',',
//							'N',',',
//							'1','2','1','3','6','.','0','0','9','9',',',
//							'E',',',
//							'2','.','6','9',',',
//							'7','9','.','6','5',',',
//							'1','0','0','1','0','6',',',',',',',
//							'A','*','5','3'
//							};


/* how often to poll GPS for coords in msec */
#define GPS_POLL_TIME_MSEC 				(30000)






void gps_task( void* p_params )
{
//	static uint32_t gps_msg_received = 0;
	//static GPSDATA_S rmc_msg = {0};
//	GPSDATA_S rmc_msg = {0};
//
//	portTickType  xPollLast;
//	portTickType  xPollRate;
//

	/* Init GPS */
	init_gps();

	/* TODO Start looking for a position */


	for (;;){
//		/* Send task check-in message */
//		system_integrity_check_in(TASKS_APP_GPS);
//
//		/* Wait for message response from other tasks */
//		if (pdPASS == queues_receive_command_message(QUEUES_APP_GPS_INPUT, &gps_input_msg, 10)){
//			/* Number of log messages received increase by one */
//			++gps_msg_received;
//
//			/* Process command from system first */
//			if (gps_input_msg.task_id == TASKS_APP_SYSTEM)
//			{
//				/* Process by command */
//				switch (gps_input_msg.command)
//				{
//				/* Send response */
//				case SYSTEM_COMMAND_START:
//#ifdef GPS_MODULE
//					/* get the semaphore first */
//					if (take_bsemaphores(UARTMUX_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){
//
//						reconfigure_uart(GPS_UART_MUX_PORT, TRUE_FLAG);
//						if(init_gps() == pdPASS){
//							set_gps_state(TASK_IDLE);
//							process_log_message(TRUE_FLAG, "GPSTask.c/gps_task/SYSTEM_COMMAND_START\0", "EVENT: GPS Initialized\0", NULL_VAL);
//						}
//						else{
//							set_gps_state(TASK_UNINIT);
//							process_log_message(TRUE_FLAG, "GPSTask.c/gps_task\0", "ERROR: GPS Init Failed\0", NULL_VAL);
//						}
//						if(give_bsemaphores(UARTMUX_BSEMAPHORE_ID)!= pdPASS){
//							process_log_message(TRUE_FLAG, "GPSTask.c/gps_task/SYSTEM_COMMAND_START\0", "ERROR: Give Semaphore\0", NULL_VAL);
//							set_gps_state(TASK_UNINIT);
//						}
//					}
//					else{
//						process_log_message(TRUE_FLAG, "GPSTask.c/gps_task/SYSTEM_COMMAND_START\0", "ERROR: Take Semaphore\0", NULL_VAL);
//						set_gps_state(TASK_UNINIT);
//					}
//#endif
//					system_send_response(SYSTEM_COMMAND_CMD_ACK, TASKS_APP_GPS);
//					break;
//
//				case SYSTEM_COMMAND_POWER_UP:
//					system_send_response(SYSTEM_COMMAND_CMD_ACK, TASKS_APP_GPS);
//					break;
//
//				case SYSTEM_COMMAND_INIT_HARDWARE:
//					set_gps_state(TASK_IDLE);
//					system_send_response(SYSTEM_COMMAND_CMD_ACK, TASKS_APP_GPS);
//					break;
//
//				case SYSTEM_COMMAND_IDLE:
//				case SYSTEM_COMMAND_TEST_START:
//				case SYSTEM_COMMAND_TEST_END:
//				case SYSTEM_COMMAND_TEST_ERROR:
//				case SYSTEM_COMMAND_ERROR:
//				case SYSTEM_COMMAND_SHUTDOWN:
//				case SYSTEM_COMMAND_OFF:
//					system_send_response(SYSTEM_COMMAND_CMD_ACK, TASKS_APP_GPS);
//					break;
//				default:
//					break;
//				}
//			}
//			else{
//				/* Process UI message */
//				switch (gps_input_msg.command){
//
//				case QUEUES_APP_SYSTEM_INTEGRITY_INPUT:
//					/* System integrity checked in - system's still running, that's about all we care about */
//					break;
//
//				case QUEUE_CMD_GPS_GET_COORDINATE:
//					/* Look at who's calling so you know where to give the response */
////					switch(gps_input_msg.task_id){
////
////					case TASKS_APP_UI:
//#ifdef GPS_MODULE
//						if( get_gps_state() == TASK_IDLE ){
//							set_gps_state(TASK_BUSY);
//
//							//printf_ui_uart("GPS Query: \r\n");
//							/* get the semaphore first */
//							if (take_bsemaphores(UARTMUX_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){
//
//								reconfigure_uart(GPS_UART_MUX_PORT, FALSE_FLAG);
//
//								/* Query GPS for data */
//								send_nmea_query_msg(RMC);
//								receive_gps_response(&rmc_msg, 5000, 0);
//
//								/* put pointer to response in Queue */
//								gps_output_msg.data_ptr= (uint32_t*)&rmc_msg;
//								/* TODO fix Harvey's task (I broke it by commenting out line below) */
////								queues_send_message(QUEUES_APP_GPS_OUTPUT, &gps_output_msg, 10);
//
//								/* give back semaphore */
//								if(give_bsemaphores(UARTMUX_BSEMAPHORE_ID)!= pdPASS){
//									process_log_message(TRUE_FLAG, "GPSTask.c/gps_task/QUEUE_CMD_GPS_GET_COORDINATE\0", "ERROR: Give Semaphore\0", NULL_VAL);
//								}
//
//								gps_inter_proc_msg.command = QUEUE_CMD_WILDCARD_RX_GPS;
//								gps_inter_proc_msg.data_ptr = gps_output_msg.data_ptr;
//
//								/* Do the reverse look up of the calling task and put the pointer
//								 * in its in queue */
//								queues_send_command_message(task_id_to_input_queue_id(gps_input_msg.task_id), &gps_inter_proc_msg, 0);
//							}
//							else{
//								process_log_message(TRUE_FLAG, "GPSTask.c/gps_task/QUEUE_CMD_GET_COORDINATE\0", "ERROR: Take Semaphore\0", NULL_VAL);
//							}
//
//							set_gps_state(TASK_IDLE);
//						}
//#endif
////						break;
////
////					default:
////						/* Unhandled case */
////						break;
////					}
//					break;
//
//					default:
//						/* Unrecognized command */
//						break;
//				}//switch
//			}//else
//		}//if
//
//		/* sleep */
//		( void ) xTaskDelayUntil( &xPollLast, xPollRate );

	}//for
}//func



