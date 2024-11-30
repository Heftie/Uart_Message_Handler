/**
 ******************************************************************************
 * @file    uart_user_messager.c
 * @author  Jonas Sigmund
 * @brief   Uart User messager
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "uart_user_messager.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize the user messager
 *
 * @return UART_MSG_HANDLER_eReturnCode
 */
UART_MSG_HANDLER_eReturnCode ucm_init()
{
    return UMH_RET_OK;
}

/**
 * @brief
 *
 * @return UART_MSG_HANDLER_eReturnCode
 */
UART_MSG_HANDLER_eReturnCode ucm_user_messager()
{
    UART_MSG_HANDLER_eReturnCode ret = UMH_RET_OK;
    UART_MSG_CMD_t command = UMC_CMD_RESPONSE;
    uint8_t *ptr;
    // Check if a message has been received
    if (umh_rx_received_flag == 0)
    {
        return UMH_RET_OK;
    }

    // get the received buffer
    umh_get_rx_ptr(ptr);
    ret = umh_decode_msg(ptr); // buffer will decoded and checked
    if (ret != UMH_RET_OK)
    {
        return ret;
    }

    // Get the command
    command = umh_rx_buffer[UCM_POS_CMD];

    switch (command)
    {
    default:
        break;
    }

    return UMH_RET_OK;
}

/* Private functions ---------------------------------------------------------*/
