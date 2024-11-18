/**
  ******************************************************************************
  * @file    uart_user_messager.h
  * @author  Jonas Sigmund
  * @brief   Uart User Messager
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
    UMC_ACK = 0xf0,
    UMC_MSG_ERROR = 0xff
} UART_MSG_CMD_t;

/* Exported constants --------------------------------------------------------*/
#define UCM_POS_CMD           1
#define UCM_POS_DATA_START    2

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/
UART_MSG_HANDLER_eReturnCode ucm_init();
UART_MSG_HANDLER_eReturnCode ucm_user_messager();

#endif  /* UART_MSG_HANDLER_H */


