/**
 ******************************************************************************
 * @file    uart_user_messager.h
 * @author  Jonas Sigmund
 * @brief   Uart User Messager
 *          This module is for implementing the user commands.
 *          The default handling is already in place.
 *
 *          Add commands to the UART_MSG_CMD_t and the action to the a
 *          ucm_user_messager() which should be called in the main loop
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UART_MSG_HANDLER_H
#define UART_MSG_HANDLER_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#include "uart_message_handler.h"

/* Exported types ------------------------------------------------------------*/

typedef enum
{
    UMC_CMD_RESPONSE = 0xF0 // 0x7E <response_cmd> <response code> <chk> 0x7E
} UART_MSG_CMD_t;

typedef enum
{
    UMC_RESP_OK = 0x00,
    UMC_RESP_REQUEST_ERROR = 0x01,
    UMC_RESP_CHANNEL_ERROR = 0X02,
    UMC_RESP_PARAMETER_ERROR = 0x03,
    UMC_RESP_BUFFER_ERROR = 0x04,
    UMC_RESP_DELAYED = 0x05,
    UMC_RESP_DATA_INVALID = 0x06,
    UMC_RESP_DATA_ERROR = 0x07,
    UMC_RESP_CHECKSUM_ERROR = 0x08,
    UMC_RESP_NUMBER
} UART_MSG_RESPONSE_t;

/* Exported constants --------------------------------------------------------*/
#define UCM_POS_CMD 1
#define UCM_POS_DATA_START 2

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
UART_MSG_HANDLER_eReturnCode ucm_init();
UART_MSG_HANDLER_eReturnCode ucm_user_messager();

#endif /* UART_MSG_HANDLER_H */
