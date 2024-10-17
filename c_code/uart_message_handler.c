/**
  ******************************************************************************
  * @file    uart_message_handler.c
  * @author  Jonas Sigmund
  * @brief   Uart Message Handler
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
# include "uart_message_handler.h"  

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t umh_rx_buffer_A[UMH_RX_BUFFER_SIZE];
uint8_t umh_rx_buffer_B[UMH_RX_BUFFER_SIZE];

uint8_t umh_tx_buffer[UMH_TX_BUFFER_SIZE];

/* Exported variables --------------------------------------------------------*/
uint8_t *umh_rx_receive_ptr = umh_rx_buffer_A;
uint8_t *umh_rx_read_ptr = umh_rx_buffer_B;
uint8_t *umh_tx_ptr = umh_tx_buffer;
uint8_t umh_rx_received_flag = 0;

/* External variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize the UART Message Handler
 * 
 * @return UART_MSG_HANDLER_eReturnCode 
 */
UART_MSG_HANDLER_eReturnCode umh_init()
{
    // Initialize the buffers
    memset(umh_rx_buffer_A, 0, UMH_RX_BUFFER_SIZE);
    memset(umh_rx_buffer_B, 0, UMH_RX_BUFFER_SIZE);
    memset(umh_tx_buffer, 0, UMH_TX_BUFFER_SIZE);
    // Set the default buffer
    umh_rx_buffer = umh_rx_buffer_A;
    umh_tx_ptr = umh_tx_buffer;
    // reset the flags
    umh_rx_received_flag = 0;
    return UMH_RET_OK;
}

/**
 * @brief Copy the data from the read buffer to a given buffer
 * 
 * @param buffer - Pointer to the buffer to copy to
 * @param length - Length of the data to copy (in bytes, max UMH_RX_BUFFER_SIZE)
 * @return UART_MSG_HANDLER_eReturnCode 
 */
UART_MSG_HANDLER_eReturnCode umh_get_buffer(uint8_t *buffer, uint16_t length)
{
    if (length > UMH_RX_BUFFER_SIZE)
    {
        return UMH_PARAMETER_ERROR;
    }
    memcpy(buffer, umh_rx_read_ptr, length);
    return UMH_RET_OK;
}

/* Private functions ---------------------------------------------------------*/
/**
 * @brief Swap the receive buffers
 * 
 * @return UART_MSG_HANDLER_eReturnCode 
 */
UART_MSG_HANDLER_eReturnCode umh_swap_rx_buffer()
{
    uint8_t *temp = umh_rx_receive_ptr;
    umh_rx_receive_ptr = umh_rx_read_ptr;
    umh_rx_read_ptr = temp;
    return UMH_RET_OK;
}
