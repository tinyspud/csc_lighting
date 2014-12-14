/*
 * LogTask.c
 *
 *  Created on: Jan 17, 2014
 *      Author: rsnoo_000
 */

/* SafeRTOS Includes */
#include "FreeRTOS.h"
#include "LogTask.h"

#include <string.h>


#include "alfat_app.h"
#include "mcs_time.h"
#include "Logging.h"

/* Forward prototypes */
//portBaseType log_receive_logging_message(queues_log_input_t* p_log_msg, portTickType recv_delay);

void log_task( void* p_params )
{
	uint32_t log_msg_received = 0;
	uint32_t log_log_msg_received = 0;

	if(alfat_init() == pdPASS){
		if(get_sys_configs() == pdPASS){
			write_compile_data();
//			process_log_message(TRUE_FLAG, "LogTask.c/log_task/SYSTEM_COMMAND_POWER_UP\0", "EVENT: Config file restored\0", NULL_VAL);
//
//			system_send_response(SYSTEM_COMMAND_CMD_ACK, TASKS_APP_LOG);

		}
		else{
//			process_log_message(TRUE_FLAG, "LogTask.c/log_task/SYSTEM_COMMAND_POWER_UP\0", "ERROR: Config file restore failed\0", NULL_VAL);
		}
	}
	else{
		//printf_ui_uart("LOGGING Initialization failed\r\n");
//		process_log_message(TRUE_FLAG, "LogTask.c/log_task/SYSTEM_COMMAND_START\0", "ERROR: ALFAT initialize failed\0", NULL_VAL);
	}

	for (;;)
	{
		/* Wait for command input message */
//		if (pdPASS == queues_receive_command_message(QUEUES_APP_LOG_INPUT, &cmd_msg, 10))
//		{
//			++log_log_msg_received;
//			/* Process command from system */
//			/* Look at who's calling so you know where to give the response */
//			switch(cmd_msg.task_id){
//			case TASKS_APP_SYSTEM_INTEGRITY:
//			{
//				switch (cmd_msg.command){
//
//				case QUEUES_APP_SYSTEM_INTEGRITY_INPUT:
//					/* System integrity checked in - system's still running, that's about all we care about */
//					break;
//
//				default:
//					break;
//				}
//			}
//			break;
//
//				/* Process UI message */
//				switch (cmd_msg.command){
//
//				case QUEUE_CMD_LOG_WRITE_USB_LOGFILE:
//					//								set_log_state(TASK_BUSY);
//					//								if (append_usb_log_file((uint8_t*)&g_sd_configbuf[0]) == pdPASS){
//					//									set_log_state(TASK_SUCCESS);
//					//								}
//					//								else{
//					//									set_log_state(TASK_FAIL);
//					//								}
//					break;
//
//					//							case QUEUE_CMD_LOG_APPEND_USB_LOGFILE:
//					//
//					//								if( close_usb_log_file() == pdPASS){
//					//									printf_ui_uart("USB LogFile Close Succeeded\r\n");
//					//								}
//					//								else{
//					//									printf_ui_uart("USB LogFile Close Failed\r\n");
//					//								}
//					//							break;
//					//
//					//							case QUEUE_CMD_LOG_READ_USB_LOGFILE:
//					////								if (vreadLogUSBFile() == pdPASS){
//					////									printf_ui_uart("USB LogFile Read Succeeded\r\n");
//					////								}
//					////								else{
//					////									printf_ui_uart("USB LogFile Read Failed\r\n");
//					////								}
//					//							break;
//
//				case QUEUE_CMD_LOG_DELETE_USB_LOGFILE:
//					set_log_state(TASK_BUSY);
//					if (delete_usb_log_file() == pdPASS){
//						set_log_state(TASK_SUCCESS);
//					}
//					else{
//						set_log_state(TASK_FAIL);
//					}
//					break;
//
//				case QUEUE_CMD_LOG_APPEND_SD_LOGFILE:
//					set_log_state(TASK_BUSY);
//					if (append_sd_log_file((uint8_t*)&g_sd_configbuf[0]) == pdPASS){
//						set_log_state(TASK_SUCCESS);
//					}
//					else{
//						set_log_state(TASK_FAIL);
//					}
//					break;
//
//				case QUEUE_CMD_LOG_CLOSE_SD_LOGFILE:
//					set_log_state(TASK_BUSY);
//					if( close_sd_log_file() == pdPASS){
//						set_log_state(TASK_SUCCESS);
//					}
//					else{
//						set_log_state(TASK_FAIL);
//					}
//					break;
//
//				case QUEUE_CMD_LOG_READ_SD_CFGFILE:
//					set_log_state(TASK_BUSY);
//					if (read_sd_config_file() == pdPASS){
//						printf_ui_uart("SD CFGFile Read Succeeded\r\n");
//						set_log_state(TASK_SUCCESS);
//					}
//					else{
//						printf_ui_uart("SD CFGFile Read Failed\r\n");
//						set_log_state(TASK_FAIL);
//					}
//					break;
//
//				case QUEUE_CMD_LOG_WRITE_SD_CFGFILE:
//					set_log_state(TASK_BUSY);
//					if (write_sd_config_file() == pdPASS){
//						printf_ui_uart("SD CFGFile Write Succeeded\r\n");
//						set_log_state(TASK_SUCCESS);
//					}
//					else{
//						printf_ui_uart("SD CFGFile Write Failed\r\n");
//						set_log_state(TASK_FAIL);
//					}
//					break;
//
//				case QUEUE_CMD_LOG_COPY_SDUSB_CFGFILE:
//					set_log_state(TASK_BUSY);
//					if (copy_sd_to_usb(FH_CFG_USB, FH_CFG_SD) == pdPASS){
//						printf_ui_uart("Copy SD CFGFile Succeeded\r\n");
//						set_log_state(TASK_SUCCESS);
//					}
//					else{
//						printf_ui_uart("Copy SD CFGFile Failed\r\n");
//						set_log_state(TASK_FAIL);
//					}
//					break;
//
//				case QUEUE_CMD_LOG_COPY_SDUSB_LOGFILE:
//					set_log_state(TASK_BUSY);
//					if (copy_sd_to_usb(FH_LOG_USB, FH_LOG_SD) == pdPASS){
//						printf_ui_uart("Copy SD LOGFile Succeeded\r\n");
//						set_log_state(TASK_SUCCESS);
//					}
//					else{
//						printf_ui_uart("Copy SD LOGGFile Failed\r\n");
//						set_log_state(TASK_FAIL);
//					}
//					break;
//
//				case QUEUE_CMD_LOG_DELETE_SD_LOGFILE:
//					set_log_state(TASK_BUSY);
//					if (delete_sd_log_file() == pdPASS){
//						set_log_state(TASK_SUCCESS);
//					}
//					else{
//						set_log_state(TASK_FAIL);
//					}
//					break;
//
//				case QUEUE_CMD_LOG_SMS:
//					set_log_state(TASK_BUSY);
//					write_sms_sd_file((receive_sms_data_struct_t *)cmd_msg.data_ptr);
//					break;
//
//				default:
//					/* Unhandled case */
//					break;
//				}
//#endif
//				break;
//
//			}
//		}//else
//
//
//		/* Wait for message response from other tasks */
//		if (pdPASS == log_receive_logging_message(&log_msg, 10))
//		{
//			set_log_state(TASK_BUSY);
//			/* Number of log messages received increase by one */
//			++log_msg_received;
//
//			/* Process log message */
//			/* @@TODO: write the log message somewhere */
//			if (append_sd_log_file((uint8_t*)&(log_msg.log_msg[0])) == pdPASS){
//				set_log_state(TASK_IDLE);
//			}
//			else{
//				set_log_state(TASK_IDLE);
//				//printf_ui_uart("SD LogFile Write Failed\r\n");
//			}
//
//			/* Clear out log message for next receive */
//			memset(&log_msg, 0, sizeof(queues_log_input_t));
//		}
//
//		/* sleep */
//		( void ) xTaskDelayUntil( &xPollLast, xPollRate );

	}//for
}




//
//
//portBaseType log_receive_logging_message(queues_log_input_t* p_log_msg, portTickType recv_delay)
//{
//	xQueueHandle queue = NULL;
//
//	/* Get handle to logging queue */
//	queue = queues_get_queue_handle(QUEUES_APP_LOG_LOGGING_INPUT);
//
//	/* Validate queue handle */
//	return xQueueReceive(queue, p_log_msg, recv_delay);
//}
//
///* Log a string by length */
//portBaseType log_message(char* p_msg, uint8_t msg_len)
//{
//	/* Log queue message structure */
//	queues_log_input_t msg;
//
//	/* Truncated length of input message */
//	uint8_t msg_trunc_len = (msg_len < QUEUES_APP_LOG_LOGGING_INPUT_MSG_SIZE) ? msg_len : QUEUES_APP_LOG_LOGGING_INPUT_MSG_SIZE;
//
//	memset(&msg, 0, sizeof(queues_log_input_t));
//	memcpy(msg.log_msg, p_msg, msg_trunc_len);
//
//	return queues_send_message(QUEUES_APP_LOG_LOGGING_INPUT, &msg, 10);
//}
//
//
//
