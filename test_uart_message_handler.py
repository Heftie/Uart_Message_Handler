import unittest
from code.uart_message_handler import UartMessageHandler

class TestUartMessageHandler(unittest.TestCase):

    def test_decode_encode_msg_no_parsing(self):
        uart = UartMessageHandler('COM22', 115200)
        # Test the encode and decode functions
        data = b'\x01\x02\x03\x04\x0A'
        data_orig = data
        print('Original Message:', ''.join('{:02x}'.format(x) for x in data))
        msg = uart.encode_message(data)
        print('Encoded Message: ', ''.join('{:02x}'.format(x) for x in msg))
        self.assertEqual(msg, b'\x7e\x01\x02\x03\x04\x0a\x7e', 'Encoded message not equal, test failed')
        data = uart.decode_message(msg)
        print('Decoded Message: ', ''.join('{:02x}'.format(x) for x in data))
        self.assertEqual(data, data_orig, 'Data not equal, test failed')      

    def test_decode_encode_msg_with_parsing(self):
        uart = UartMessageHandler('COM22', 115200)
        # Test the encode and decode functions
        data = b'\x7E\x7d\x02\x03\x04\x04'
        data_orig = data
        print('Original Message:', ''.join('{:02x}'.format(x) for x in data))
        msg = uart.encode_message(data)
        print('Encoded Message: ', ''.join('{:02x}'.format(x) for x in msg))
        self.assertEqual(msg, b'\x7e\x7d\x5e\x7d\x5d\x02\x03\x04\x04\x7e', 'Encoded message not equal, test failed')
        data = uart.decode_message(msg)
        print('Decoded Message: ', ''.join('{:02x}'.format(x) for x in data))
        self.assertEqual(data, data_orig, 'Data not equal, test failed')

    def test_checksum(self):
        uart = UartMessageHandler('COM22', 115200)
        # Test the checksum function
        data = b'\x01\x02\x03\x04'
        checksum = uart.calculate_checksum(data)
        self.assertEqual(checksum, b'\x0a', 'Checksum not equal, test failed')
        data = b'\xff\x01\x02\x03\x04'
        checksum = uart.calculate_checksum(data)
        self.assertEqual(checksum, b'\x09', 'Checksum not equal, test failed')

if __name__ == '__main__':
    unittest.main()