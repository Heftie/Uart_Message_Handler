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
    UMH_BUFFER_ERROR         = 4,    /**< Buffer Size is not sufficent */
    UMH_DELAYED              = 5,    /**< Request accepted but delayed */
    UMH_DATA_INVALID         = 6,    /**< Data area not valid */
    UMH_DATA_ERROR           = 7,    /**< Data area not valid */
    UMH_CHKSUM_ERROR         = 8,    /**< Checksum error */
    UMH_INITVAL_CODE         = 0xFF  /**< UCOM Module internal use only */
} UART_MSG_HANDLER_eReturnCode;

/* Exported constants --------------------------------------------------------*/
#define UMH_RX_BUFFER_SIZE  (256)
#define UMH_TX_BUFFER_SIZE  (256)
#define UMH_FRAME_SIZE      (3) // Start byte, data, checksum
#define UMH_MAX_DATA_SIZE   ((UMH_TX_BUFFER_SIZE >> 1) - UMH_FRAME_SIZE) // Space for escaping data


/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern uint8_t *umh_rx_read_ptr;
extern uint8_t *umh_tx_ptr;

/* Exported functions --------------------------------------------------------*/
UART_MSG_HANDLER_eReturnCode umh_init();
UART_MSG_HANDLER_eReturnCode umh_get_rx_ptr(uint8_t **ptr);
UART_MSG_HANDLER_eReturnCode umh_get_rx_buffer(uint8_t *buffer, uint32_t length);
UART_MSG_HANDLER_eReturnCode umh_clear_rx_read_buffer();
UART_MSG_HANDLER_eReturnCode umh_send_buffer(uint8_t *buffer, uint32_t length);
UART_MSG_HANDLER_eReturnCode umh_send_buffer_u16(uint16_t *buffer, uint32_t length);
UART_MSG_HANDLER_eReturnCode umh_send_buffer_u32(uint32_t *buffer, uint32_t length);

void umh_ISR(uint16_t size);

#endif  /* UART_MSG_HANDLER_H */