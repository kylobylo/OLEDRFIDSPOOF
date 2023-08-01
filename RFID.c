#include "RFID.h"


uint8_t command;
time_t currentTime;


int8_t writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
    printf("Writing Command\n");
    command = header[0];
    uint8_t xBuf[8 + hlen + blen]; //replace with queue
    
    xBuf[0] = PN532_PREAMBLE;
    xBuf[1] = PN532_STARTCODE1;
    xBuf[2] = PN532_STARTCODE2;
    
    uint8_t length = hlen + blen + 1;   // length of data field: TFI + DATA
    xBuf[3] = length;
    xBuf[4] = ~length + 1;                 // checksum of length
    
    xBuf[5] = PN532_HOSTTOPN532;
    uint8_t sum = PN532_HOSTTOPN532;    // sum of TFI + DATA
    
    
    printf("write: ");
       
    uint8_t i = 0;

    while(i < hlen) {
        //if (i2c_write_blocking(i2cPORT, addr, header[i], 1, true)) {
            xBuf[6+i] = header[i];
            sum += header[i];
            
            printf("Writing header: %X \n", header[i]);
        /*} else {
            printf("\nToo many data to send, I2C doesn't support such a big packet\n");     // I2C max packet: 32 bytes
            return PN532_INVALID_FRAME;
        }*/
        i++;
    }

    uint8_t j = 0;

    while( j < blen) {
        //if (i2c_write_blocking(i2cPORT, addr,body[i], 1, true)) {
            xBuf[6+i+j];
            sum += body[j];
            
            printf("Writing body: %X \n", body[j]);
       /*  } else {
            printf("\nToo many data to send, I2C doesn't support such a big packet\n");     // I2C max packet: 32 bytes
            return PN532_INVALID_FRAME;
        } */
        j++;
    }
  
    uint8_t checksum = ~sum + 1;            // checksum of TFI + DATA
    xBuf[7 + i + j] = checksum;
    xBuf[8 + i + j] = PN532_POSTAMBLE;

    int status = i2c_write_blocking(i2cPORT, addr, xBuf, sizeof(xBuf), false);
    printf("Write Status: %d\n", status);


    printf("xBuf Size: %d \n", sizeof(xBuf));


    return 0;//readAckFrame();
}

int8_t readAckFrame()
{
    const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};
    uint8_t ackBuf[sizeof(PN532_ACK)];
    uint8_t xBuf[sizeof(PN532_ACK)+1];
    
    currentTime = clock() / CLOCKS_PER_SEC;
    printf("wait for ack at : ");
    printf(ctime(&currentTime));
    printf('\n');
    
    uint16_t time = 0;
    do {

        int status = i2c_read_blocking(i2cPORT, addr, xBuf, sizeof(PN532_ACK) + 1, false);
        if (status) {
            printf("Read Status: %d \n", status);
            printf("ACK xbuf: %X \n", xBuf[0]);
            if (xBuf[0] & 1) {  // check first byte --- status
                break;         // PN532 is ready
            }
        }

        sleep_ms(1);
        time++;
        if (time > PN532_ACK_WAIT_TIME) {
            printf("Time out when waiting for ACK\n");
            return PN532_TIMEOUT;
        }
    } while (1); 

    currentTime = clock() / CLOCKS_PER_SEC;
    
    printf("ready at : ");
    printf(ctime(&currentTime));
    printf('\n');
    

    for (uint8_t i = 0; i < sizeof(PN532_ACK); i++) {
        ackBuf[i] = xBuf[i + 1];
    }
    
    if (memcmp(ackBuf, PN532_ACK, sizeof(PN532_ACK))) {
        printf("Invalid ACK\n");
        return PN532_INVALID_ACK;
    }
    
    return 0;
}

int16_t getResponseLength(uint8_t buf[], uint8_t len, uint16_t timeout) {
    const uint8_t PN532_NACK[] = {0, 0, 0xFF, 0xFF, 0, 0};
    uint16_t time = 0;

    //Extra Buffer
    uint8_t xBuf[6];

    do {
        if (i2c_read_blocking(i2cPORT, addr, xBuf, 6, false)) {
            for (int l = 0; l < 6; l++) {
                printf("Recieved xbuf[%d], read as: %X\n", l, xBuf[l]);
            }
                if (xBuf[0] & 1) {  // check first byte --- status
                    break;         // PN532 is ready
                }
        }

        sleep_ms(1);
        time++;
        if ((0 != timeout) && (time > timeout)) {
            return -1;
        }
    } while (1); 
    
    if (0x00 != xBuf[1]      ||       // PREAMBLE
            0x00 != xBuf[2]  ||       // STARTCODE1
            0xFF != xBuf[3]           // STARTCODE2
        ) {
        
        return PN532_INVALID_FRAME;
    }
    
    uint8_t length = xBuf[4];

    // request for last respond msg again
    for (uint16_t i = 0; i < sizeof(PN532_NACK); ++i) {
      i2c_write_blocking(i2cPORT, addr, PN532_NACK[i], sizeof(PN532_NACK), true);
    }

    return length;
}

int16_t readResponse(uint8_t buf[], uint8_t len, uint16_t timeout)
{
    uint16_t time = 0;
    int16_t length;
    

    length = getResponseLength(buf, len, timeout);

    printf("Recieving length of %d bytes\n", length);

    uint8_t xBuf[ 6 + length + 2];

    // [RDY] 00 00 FF LEN LCS (TFI PD0 ... PDn) DCS 00
    do {
        if (i2c_read_blocking( i2cPORT, addr, xBuf, 6 + length + 2, false)) {
            if (xBuf[0] & 1) {  // check first byte --- status
                break;         // PN532 is ready
            }
        }

        sleep_ms(1);
        time++;
        if ((0 != timeout) && (time > timeout)) {
            return -1;
        }
    } while (1); 
    
    if (0x00 != xBuf[1]      ||       // PREAMBLE
            0x00 != xBuf[2]  ||       // STARTCODE1
            0xFF != xBuf[3]           // STARTCODE2
        ) {
        
        return PN532_INVALID_FRAME;
    }
    
    length = xBuf[4];

    if (0 != (uint8_t)(length + xBuf[5])) {   // checksum of length
        return PN532_INVALID_FRAME;
    }
    
    uint8_t cmd = command + 1;               // response command
    if (PN532_PN532TOHOST != xBuf[6] || (cmd) != xBuf[7]) {
        return PN532_INVALID_FRAME;
    }
    
    length -= 2;
    if (length > len) {
        return PN532_NO_SPACE;  // not enough space
    }
    
    printf("read:  ");
    printf("%X" ,cmd);
    
    uint8_t sum = PN532_PN532TOHOST + cmd;
    for (uint8_t i = 0; i < length; i++) {
        buf[i] = xBuf[8];
        sum += buf[i];
        
        printf("%X", buf[i]);
    }
    printf('\n');
    
    uint8_t checksum = xBuf[9];
    if (0 != (uint8_t)(sum + checksum)) {
        printf("checksum is not ok\n");
        return PN532_INVALID_FRAME;
    }
    
    return length;
}


