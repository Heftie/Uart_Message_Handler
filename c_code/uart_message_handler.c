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
/* Private variables ---------------------------------------------------------*/
uint8_t umh_rx_buffer_A[UMH_RX_BUFFER_SIZE];
uint8_t umh_rx_buffer_B[UMH_RX_BUFFER_SIZE];

uint8_t umh_tx_buffer[UMH_TX_BUFFER_SIZE];

/* Exported variables --------------------------------------------------------*/
uint8_t *umh_rx_receive_ptr = umh_rx_buffer_A;
uint8_t *umh_rx_read_ptr = umh_rx_buffer_B;
uint8_t *umh_tx_ptr = umh_tx_buffer;

uint32_t umh_rx_size = 0;

uint8_t umh_rx_received_flag = 0;
uint8_t umh_tx_ready_flag = 0;
/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
UART_MSG_HANDLER_eReturnCode umh_decode_msg(uint8_t *input_buffer);
UART_MSG_HANDLER_eReturnCode umh_encode_msg(uint8_t *buffer, uint32_t length);
UART_MSG_HANDLER_eReturnCode umh_swap_rx_buffer();

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
    umh_rx_size = 0;
    // Set the default buffer
    umh_rx_receive_ptr = umh_rx_buffer_A;
    umh_rx_read_ptr = umh_rx_buffer_B;
    umh_tx_ptr = umh_tx_buffer;
    // reset the flags
    umh_rx_received_flag = 0;
    umh_tx_ready_flag = 0;
    // First start of receive
    if (HAL_OK != HAL_UARTEx_ReceiveToIdle_DMA(&huart2, umh_rx_receive_ptr, UMH_RX_BUFFER_SIZE)){
    	return UMH_REQUEST_ERROR;
    }

    return UMH_RET_OK;
}

/**
 * @brief Get the current read pointer
 * 
 * @param ptr - Pointer to hold the adresse of the current read pointer
 * @return UART_MSG_HANDLER_eReturnCode 
 */
UART_MSG_HANDLER_eReturnCode umh_get_rx_ptr(uint8_t **ptr)
{
    *ptr = umh_rx_read_ptr;
    return UMH_RET_OK;
}

/**
 * @brief Copy the data from the read buffer to a given buffer
 * 
 * @param buffer - Pointer to the buffer to copy to
 * @param length - Length of the data to copy (in bytes, max UMH_RX_BUFFER_SIZE)
 * @return UART_MSG_HANDLER_eReturnCode 
 */
UART_MSG_HANDLER_eReturnCode umh_get_rx_buffer(uint8_t *buffer, uint32_t length)
{
    if (length > UMH_RX_BUFFER_SIZE)
    {
        return UMH_PARAMETER_ERROR;
    }
    memcpy(buffer, umh_rx_read_ptr, length);
    return UMH_RET_OK;
}

/**
 * @brief Clear the read buffer
 * 
 * @return UART_MSG_HANDLER_eReturnCode 
 */
UART_MSG_HANDLER_eReturnCode umh_clear_rx_read_buffer()
{
    memset(umh_rx_read_ptr, 0, UMH_RX_BUFFER_SIZE);
    return UMH_RET_OK;
}

/**
 * @brief Send a buffer of uint8_t data over the UART
 * 
 * @param buffer - Pointer to the buffer to copy from
 * @param length - Length of the data to copy (in bytes, max UMH_MAX_DATA_SIZE)
 * @return UART_MSG_HANDLER_eReturnCode 
 */
UART_MSG_HANDLER_eReturnCode umh_send_buffer(uint8_t *buffer, uint32_t length)
{
    if (length > UMH_MAX_DATA_SIZE)
    {
        return UMH_PARAMETER_ERROR;
    }
    umh_transmit_data(buffer, length);
    return UMH_RET_OK;
}

/**
 * @brief Send a buffer of uint16_t data with MSB first alignment
 * 
 * @param buffer - Pointer to the buffer to send
 * @param length - Length of the data to send (in words, max UMH_MAX_DATA_SIZE / 2)
 * @return UART_MSG_HANDLER_eReturnCode 
 */
UART_MSG_HANDLER_eReturnCode umh_send_buffer_u16(uint16_t *buffer, uint32_t length)
{
    // check if the length is too large
    if (length > UMH_MAX_DATA_SIZE / 2)
    {
        return UMH_PARAMETER_ERROR;
    }
    // align data msb first
    for (uint32_t i = 0; i < length; i++)
    {
        umh_tx_buffer[i * 2] = (buffer[i] >> 8) & 0xFF;
        umh_tx_buffer[i * 2 + 1] = buffer[i] & 0xFF;
    }
    umh_transmit_data(umh_tx_buffer, length * 2);
    return UMH_RET_OK;
}

/**
 * @brief Send a buffer of uint32_t data with MSB first alignment
 * 
 * @param buffer - Pointer to the buffer to send
 * @param length - Length of the data to send (in longs, max UMH_MAX_DATA_SIZE / 4)
 * @return UART_MSG_HANDLER_eReturnCode 
 */
UART_MSG_HANDLER_eReturnCode umh_send_buffer_u32(uint32_t *buffer, uint32_t length)
{
    // check if the length is too large
    if (length > UMH_MAX_DATA_SIZE / 4)
    {
        return UMH_PARAMETER_ERROR;
    }
    // align data msb first
    for (uint32_t i = 0; i < length; i++)
    {
        umh_tx_buffer[i * 2]     = (buffer[i] >> 24) & 0xFF;
        umh_tx_buffer[i * 2 + 1] = (buffer[i] >> 16) & 0xFF;
        umh_tx_buffer[i * 2 + 2] = (buffer[i] >> 8)  & 0xFF;
        umh_tx_buffer[i * 2 + 3] = buffer[i] & 0xFF;
    }
    umh_transmit_data(umh_tx_buffer, length * 4);
    return UMH_RET_OK;
}    

/**
 * @brief ISR function to be called when the UART received a complete message
 * 
 * @param size - Size of the received message
 */
void umh_ISR(uint16_t size)
{
    // save size received
    umh_rx_size = size;
    umh_swap_rx_buffer();
    // set receive flag
    umh_rx_received_flag = 1;
    // restart HAL RX
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, umh_rx_receive_ptr, UMH_RX_BUFFER_SIZE);
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief Dummy function to transmit data. This needs to be adjusted to the actual hardware
 * 
 * @param buffer - Pointer to the buffer to transmit
 * @param length - Length of the data to transmit (in bytes)
 * @return UART_MSG_HANDLER_eReturnCode 
 */
UART_MSG_HANDLER_eReturnCode umh_transmit_data(uint8_t *buffer, uint32_t length)
{
    // Transmit the data
	if (HAL_OK != HAL_UART_Transmit_DMA(&huart2, buffer, length)){
		return UMH_REQUEST_ERROR;
	}
    return UMH_RET_OK;
}

/**
 * @brief Decode a message in the given input buffer.
 *      The message format is in the following way:
 *      - Start byte: 0x7E
 *      - Data : n bytes
 *      - Checksum: 1 byte
 *      - End byte: 0x7E
 *      The checksum is calculated as sum of all data bytes (excluding start and end byte)
 *      The message is escaped as follows:
 *      - 7D 5E -> 7E
 *      - 7D 5D -> 7D
 * 
 * @param buffer 
 * @param length 
 * @return UART_MSG_HANDLER_eReturnCode 
 */
UART_MSG_HANDLER_eReturnCode umh_decode_msg(uint8_t *input_buffer)
{
    uint8_t *read_ptr = input_buffer;
    uint8_t *write_ptr = input_buffer;
    uint8_t checksum = 0;
    uint8_t sofeof_cnt = 0;
    for (uint32_t i = 0; i < UMH_RX_BUFFER_SIZE; i++)
    {
        // Check for escape characters
        if (*read_ptr == 0x7D)
        {
            read_ptr++;
            if (*read_ptr == 0x5E)
            {
                *write_ptr = 0x7E;
            }
            else if (*read_ptr == 0x5D)
            {
                *write_ptr = 0x7D;
            }
        } else {
            *write_ptr = *read_ptr;
        }
        // Check for start and end byte
        if (*read_ptr == 0x7E)
        {
            sofeof_cnt++;
        } else {
            checksum += *read_ptr;
        }
        // Check for end of message and break
        if (sofeof_cnt >= 2)
        {
            // clear the rest of the buffer with 0
            // current position is the checksum byte (i+1), the end byte (i+2), to keep the complete message
            // also the write_ptr stays at the checksum byte for later use!
            memset(write_ptr + 2, 0, UMH_RX_BUFFER_SIZE - i - 2);
            break;
        }
        // Move to the next byte
        read_ptr++;
        write_ptr++;
    }
    // Check if the message is valid
    if (sofeof_cnt != 2)
    {
        return UMH_DATA_INVALID;
    }
    // Check the checksum, the last byte is the checksum
    if (checksum != *write_ptr)
    {
        return UMH_CHKSUM_ERROR;
    }
    return UMH_RET_OK;
}

/**
 * @brief Encode a message in the buffer to be sent
 *      The message format is in the following way:
 *      - Start byte: 0x7E
 *      - Data : n bytes
 *      - Checksum: 1 byte
 *      - End byte: 0x7E
 *      The checksum is calculated as sum of all data bytes (excluding start and end byte)
 *      The message is escaped as follows:
 *      - 7E -> 7D 5E
 *      - 7D -> 7D 5D
 * @param buffer 
 * @param length 
 * @return UART_MSG_HANDLER_eReturnCode 
 */
UART_MSG_HANDLER_eReturnCode umh_encode_msg(uint8_t *buffer, uint32_t length)
{
    uint8_t *ptr = buffer;
    uint8_t *tx_ptr = umh_tx_ptr;
    uint8_t checksum = 0;
    // Check if the buffer is empty
    if (length == 0)
    {
        return UMH_PARAMETER_ERROR;
    }
    // Check if the buffer is too large
    if (length > UMH_MAX_DATA_SIZE)
    {
        return UMH_BUFFER_ERROR;
    }
    // Add the start byte
    *tx_ptr++ = 0x7E;
    // Calculate the checksum
    for (uint32_t i = 0; i < length; i++)
    {
        checksum += *ptr;
        // Check for escape characters
        if (*ptr == 0x7E)
        {
            *tx_ptr++ = 0x7D;
            *tx_ptr++ = 0x5E;
        }
        else if (*ptr == 0x7D)
        {
            *tx_ptr++ = 0x7D;
            *tx_ptr++ = 0x5D;
        }
        else
        {
            *tx_ptr++ = *ptr;
        }
        ptr++;
    }
    // Add the checksum
    *tx_ptr++ = checksum;
    // Add the end byte
    *tx_ptr++ = 0x7E;
    return UMH_RET_OK;
}

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
