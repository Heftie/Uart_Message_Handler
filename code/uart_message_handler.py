import serial

# This class is used to handle UART Messages
#      The message is in the format:
#      0x7E <command> <data> <chk> 0x7E
#      The command is a single byte
#      The data is a variable length
#      The chk is a single byte (checksum over the command and data)

# The protocol has a flow control mechanism
#      The sender sends a message and waits for a response message from the receiver
#      If the sender does not receive a response message within a timeout period, it resends the message
#      The receiver sends a response message after receiving a message
#      The response message is in the format:
#      0x7E <response_cmd> <response code> <chk> 0x7E
#      The response_cmd is 0xF0
#      The response code is a single byte
#        0x00: OK
#        0x01: Request Error
#        0x02: Channel Error
#        0x03: Parameter Error
#        0x04: Buffer Error
#        0x05: Delayed
#        0x06: Data Invalid
#        0x07: Data Error
#        0x08: Checksum Error
#
#      The flow control mechanism only automtically resends the message if the response code is 0x08 (Checksum Error)


class UartMessageHandler:
    def __init__(self, port, baudrate):
        self.port = port
        self.baudrate = baudrate
        self.ser = serial.Serial(port, baudrate)
        self.ser.timeout = 1
        self.ser.flushInput()
        self.ser.flushOutput()
        self.number_of_retries = 3

    def open(self):
        self.serial.open()

    def close(self):
        self.serial.close()

    def send_message(self, msg : bytes) -> bytes:
        '''Send a message to the UART and wait for the response message. 
        If the response message is not received within a timeout period, resend the message.'''
        msg += self.calculate_checksum(msg)
        msg = self.encode_message(msg)
        for i in range(self.number_of_retries):
            self.ser.write(msg)
            response = self.receive_message()
            response = self.decode_message(response)
            # check if response is valid checksum is correct and no checksum error on receiver side
            if response != b'' or response[1] == 0x08: 
                return response
        return b''
        
    def receive_message(self) -> bytes:
        '''read data from serial port until "0x7E" is received twice (start and end of message).
        Return the message or in case of timeout return b''.'''
        data = b''
        loop = True
        sofeof_cnt = 0
        while loop:
            received = self.ser.read(1)
            #print (received)
            if received == b'':
                return b''
            data += received
            if received == b'\x7E':
                sofeof_cnt += 1
            if sofeof_cnt >= 2:
                loop = False


    def encode_message(self, data : bytes) -> bytes:
        '''Encode the data, add start and end of frame and return the message.'''
        # parse the byte data 
        # 0x7E -> 0x7D 0x5E
        # 0x7D -> 0x7D 0x5D
        msg = b''
        for i in data:
            i = bytes([i])
            if i == b'\x7E':
                msg += b'\x7D\x5E'
            elif i == b'\x7D':
                msg += b'\x7D\x5D'
            else:
                msg += i
        # add start and end of frame
        msg = b'\x7E' + msg + b'\x7E'
        return msg
    
    def decode_message(self, msg : bytes) -> bytes:
        '''Decode the message, remove start and end of frame and return the data. 
        If the message is invalid return an empty byte string.'''
        # check if message has start and end 
        # if so remove the start and end bytes
        # else return error
        if msg[0] != 0x7E or msg[-1] != 0x7E:
            return b''
        msg = msg[1:-1]
        # parse the byte data 
        # 0x7D 0x5E -> 0x7E
        # 0x7D 0x5D -> 0x7D
        data = b''
        i = 0
        while i < len(msg):
            if msg[i] == 0x7D:
                if msg[i+1] == 0x5E:
                    data += b'\x7E'
                elif msg[i+1] == 0x5D:
                    data += b'\x7D'
                i += 1
            else:
                data += bytes([msg[i]])
            i += 1
        return data
    
    def calculate_checksum(self, data : bytes) -> bytes:
        '''Calculate the checksum of the data and return uint8'''
        chksum = 0
        for item in data:
            chksum += int(item)
        chksum = chksum & 0xFF
        return bytes([chksum])

