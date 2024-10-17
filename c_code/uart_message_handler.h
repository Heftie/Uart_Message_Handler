/**
  ******************************************************************************
  * @file    uart_message_handler.h
  * @author  Jonas Sigmund
  * @brief   Uart Message Handler
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UART_MSG_HANDLER_H
#define UART_MSG_HANDLER_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdlib.h"
/* Exported types ------------------------------------------------------------*/
typedef enum
{
    UMH_RET_OK               = 0,    /**< Function call was accepted */
    UMH_REQUEST_ERROR        = 1,    /**< Function call cannot be honored */
    UMH_CHANNEL_ERROR        = 2,    /**< Invalid sequencer argument */
    UMH_PARAMETER_ERROR      = 3,    /**< Arguments are invalid */
    UMH_BUFFER_ERROR         = 4,    /**< Communication failure */
    UMH_DELAYED              = 5,    /**< Request accepted but delayed */
    UMH_DATA_INVALID         = 6,    /**< Data area not valid */
    UMH_DATA_ERROR           = 7,    /**< Data area not valid */
    UMH_CHKSUM_ERROR         = 8,    /**< Checksum error */
    UMH_INITVAL_CODE         = 0xFF  /**< UCOM Module internal use only */
} UART_MSG_HANDLER_eReturnCode;

/* Exported constants --------------------------------------------------------*/
#define UMH_RX_BUFFER_SIZE  256
#define UMH_TX_BUFFER_SIZE  256

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern uint8_t *umh_rx_buffer;
extern uint8_t *umh_tx_buffer;

/* Exported functions --------------------------------------------------------*/
UART_MSG_HANDLER_eReturnCode umh_init();


#endif  /* UART_MSG_HANDLER_H */


