#include "RFID.hpp"


uint8_t command;
time_t currentTime;

uint8_t PN532::pn532_packetbuffer[64];
uint8_t PN532::_uidLen;
uint8_t PN532::uidLength;
uint8_t PN532::_key[6] = { 0, 0, 0, 0, 0, 0};
uint8_t PN532::_uid[7] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t PN532::_felicaIDm[8];
uint8_t PN532::_felicaPMm[8];


uint8_t PN532::uid[7] = { 0, 0, 0, 0, 0, 0, 0 };




bool PN532::to_hex(char *dest, size_t dest_len, const uint8_t* values, size_t val_len) {
    static const char hex_table[] = "0123456789ABCDEF";
    if(dest_len < (val_len*2+1)) /* check that dest is large enough */
        return false;
    while(val_len--) {
        /* shift down the top nibble and pick a char from the hex_table */
        *dest++ = hex_table[*values >> 4];
        /* extract the bottom nibble and pick a char from the hex_table */
        *dest++ = hex_table[*values++ & 0xF];
    }
    *dest = 0;
    return true;
}

int8_t PN532::writeCommand(uint8_t (*header)[64], uint8_t hlen, const uint8_t *body, uint8_t blen)
{
const uint LED_PIN = 25;
uint8_t pn532_packetbuffer[64];


char c;
uint8_t uidLength;
uint8_t uid[7] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID

    printf("Writing Command\n");
    command = **header;
    uint8_t xBuf[8 + hlen + blen]; //replace with queue
    
    xBuf[0] = PN532_PREAMBLE;
    //printf("xBuf[0] = 0x%X\n", xBuf[0]);
    xBuf[1] = PN532_STARTCODE1;
    //printf("xBuf[1] = 0x%X\n", xBuf[1]);
    xBuf[2] = PN532_STARTCODE2;
    //printf("xBuf[2] = 0x%X\n", xBuf[2]);
    
    uint8_t length = hlen + blen + 1;   // length of data field: TFI + DATA
    xBuf[3] = length;
    //printf("xBuf[3] = 0x%X\n", xBuf[3]);
    xBuf[4] = ~length + 1;              // checksum of length
    //printf("xBuf[4] = 0x%X\n", xBuf[4]);
    
    xBuf[5] = PN532_HOSTTOPN532;
    //printf("xBuf[5] = 0x%X\n", xBuf[5]);
    uint8_t sum = PN532_HOSTTOPN532;    // sum of TFI + DATA
    
    
    printf("write: ");
       
    uint8_t i = 0;

    uint8_t* ptr;

    while(i < hlen) {
            ptr = *header+i;
        //if (i2c_write_blocking(i2cPORT, addr, header[i], 1, true)) {
            xBuf[6+i] = *ptr;
            printf("xBuf[%d] = 0x%X\n", 6+i, xBuf[6+i]);
            sum += *ptr;
            
            printf("Writing header: %X \n", *ptr);
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
    printf("Got commands\n");

    /*uint8_t firmCommand[9];
    firmCommand[0] = 0x00; 
    firmCommand[1] = 0x00;
    firmCommand[2] = 0xFF;
    firmCommand[3] = 0x02;
    firmCommand[4] = 0xFE;
    firmCommand[5] = 0xD4;
    firmCommand[6] = 0x02; 
    firmCommand[7] = 0x2A;
    firmCommand[8] = 0x00;*/
  
    //printf("Sum: 0x%X \n",sum);
    uint8_t checksum = ~sum + 1;            // checksum of TFI + DATA
    //printf("Checksum: 0x%x\n", checksum);
    xBuf[6 + i + j] = checksum;
    //printf("xBuf[%d] = 0x%X\n", 6+i+j,xBuf[7]);
    xBuf[7 + i + j] = PN532_POSTAMBLE;
    //printf("xBuf[%d] = 0x%X\n", 7+i+j, xBuf[8]);

    //printf("xBuf Size: %d \n", sizeof(xBuf));

    //printf("xBuf Contents:\n");
    for(int o = 0; o<sizeof(xBuf); o++) {
        printf("xBuf[%d] = %X\n", o, xBuf[o]);
    }

    int status = i2c_write_blocking(i2cPORT, addr, xBuf, sizeof(xBuf), false);
    printf("Write Status: %d\n", status);





    return readAckFrame();
}

void PN532::clearBuffer() {
    for(int i = 0; i < sizeof(pn532_packetbuffer); i++) {
        pn532_packetbuffer[i] = 0;
    }
}


bool PN532::getStatus(bool msg) {
    
    clearBuffer();

    uint8_t xBuf[64];
    xBuf[0] = 4;
    bool status = writeCommand(&xBuf, 1, 0, 0);
    if(status) {
        return false;
    }
    status = readResponse(&pn532_packetbuffer, sizeof(*pn532_packetbuffer), 1000);
    if(msg){
        dumpBuffer();
    }
    if(status) {
        return false;
    }



    return true;
}

void PN532::dumpBuffer() {

    for(int i = 0; i < sizeof(pn532_packetbuffer); i++) {
        printf("PN532_PacketBuffer[%d]: 0x%X\n", i, *(pn532_packetbuffer+i));
    }
}


int8_t PN532::readAckFrame()
{
    const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};
    uint8_t ackBuf[sizeof(PN532_ACK)];
    uint8_t xBuf[sizeof(PN532_ACK)+1];
    
    currentTime = clock() / CLOCKS_PER_SEC;
    printf("wait for ack at : ");
    printf(ctime(&currentTime));
    printf("\n");
    
    uint16_t time = 0;
    int status = 0;

    for(int i = 0; i<PN532_ACK_WAIT_TIME; i++) {
        status = i2c_read_blocking(i2cPORT, addr, xBuf, sizeof(PN532_ACK) + 1, false);
        if(xBuf[0] & 1) {
            break;
        }
    }
    if (status == -1) {
        printf("ERROR: Timeout waiting for ack");
    }

    currentTime = clock() / CLOCKS_PER_SEC;
    
    printf("ready at : ");
    printf(ctime(&currentTime));
    printf("\n");
    

    for (uint8_t i = 0; i < sizeof(PN532_ACK); i++) {
        ackBuf[i] = xBuf[i + 1];
    }
    
    if (memcmp(ackBuf, PN532_ACK, sizeof(PN532_ACK))) {
        printf("Invalid ACK\n");
        return PN532_INVALID_ACK;
    }
    
    return 0;
} 


bool PN532::SAMConfig() {
    uint8_t xBuf[64];
    xBuf[0] = PN532_COMMAND_SAMCONFIGURATION;
    xBuf[1] = 0x01; // normal mode;
    xBuf[2] = 0x14; // timeout 50ms * 20 = 1 second
    xBuf[3] = 0x01; // use IRQ pin!

    writeCommand(&xBuf, 4, 0, 0);

    return (0 < readResponse(&pn532_packetbuffer, sizeof(*pn532_packetbuffer), 1000));

};


bool PN532::readPassiveTargetID(uint8_t (*buf)[64], uint8_t cardbaudrate, uint8_t (*uid)[7], uint8_t *uidLength, uint16_t timeout) {
    uint8_t xBuf[64];
    xBuf[0] = PN532_COMMAND_INLISTPASSIVETARGET;
    xBuf[1] = 1;
    xBuf[2] = cardbaudrate;

    writeCommand(&xBuf, 3, 0, 0);

    if (readResponse(buf, sizeof(*buf), timeout) < 0) {
        return 0x0;
    }

    // check some basic stuff
    /* ISO14443A card response should be in the following format:

      byte            Description
      -------------   ------------------------------------------
      b0              Tags Found
      b1              Tag Number (only one used in this example)
      b2..3           SENS_RES
      b4              SEL_RES
      b5              NFCID Length
      b6..NFCIDLen    NFCID
    */

    if(**buf != 1) {
        return 0;
    }

    uint16_t sens_res = *(*buf+2);
    sens_res <<= 8;
    sens_res |= *(*buf+3);

    printf("ATQA: 0x%X  ", sens_res);
    printf("SAK: 0x%X\n", *(*buf+4));

    dumpBuffer();

    /* Card appears to be Mifare Classic */
    *uidLength = *(*buf+5);

    for(uint8_t i = 0; i < *(*buf+5); i++) {

        
        *(*uid+i) = *(*buf+(6+i));
        
    }

    return 1;


};






int16_t PN532::getResponseLength(uint8_t (*buf)[64], uint8_t len, uint16_t timeout) {
    const uint8_t PN532_NACK[] = {0, 0, 0xFF, 0xFF, 0, 0};
    uint16_t time = 0;

    //Extra Buffer
    uint8_t xBuf[6];


    do {
        if (i2c_read_blocking(i2cPORT, addr, xBuf, 6, false)) {
           /* for (int l = 0; l < 6; l++) {
                printf("Recieved xbuf[%d](getResponseLength), read as: %X\n", l, xBuf[l]);
            } */
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
    i2c_write_blocking(i2cPORT, addr, PN532_NACK, sizeof(PN532_NACK), false);
    

    return length;
}

int16_t PN532::readResponse(uint8_t (*buf)[64], uint8_t len, uint16_t timeout)
{
    uint16_t time = 0;
    int16_t length;
    

    length = getResponseLength(buf, len, timeout);

    printf("Recieving length of %d bytes\n", length);

    uint8_t xBuf[ 6 + length + 2];

    // [RDY] 00 00 FF LEN LCS (TFI PD0 ... PDn) DCS 00
    do {
        if (i2c_read_blocking( i2cPORT, addr, xBuf, 6 + length + 2, false)) {
            /*for(int p = 0; p < sizeof(xBuf); p++) {
                printf("Recieved xBuf[%d], read as: %X\n",p, xBuf[p]);
            } */
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
    printf("%X " ,cmd);

    uint8_t *p;
    
    uint8_t sum = PN532_PN532TOHOST + cmd;
    for (uint8_t i = 0; i < length; i++) {
        p = *buf+i;
        *p = xBuf[8 + i];
        sum += *p;
        
        printf("%X ", *p);
    }
    printf("\n");
    
    uint8_t checksum = xBuf[9+length-1];
    if (0 != (uint8_t)(sum + checksum)) {
        printf("checksum (%d) is not ok\n",sum);
        return PN532_INVALID_FRAME;
    }
    
    return length;
}


uint8_t PN532::mifareclassic_AuthenticateBlock (uint8_t (*buf)[64], uint8_t *uid, uint8_t uidLen, uint32_t blockNumber, uint8_t keyNumber, uint8_t *keyData) {
    uint8_t i;

    // Hang on to the key and uid data
    memcpy (_key, keyData, 6);
    memcpy (_uid, uid, uidLen);
    _uidLen = uidLen;

    uint8_t xBuf[64];

    //Prepare the authentication command
    xBuf[0] = PN532_COMMAND_INDATAEXCHANGE; //Data Exchange Header
    xBuf[1] = 1; //Max Card Number
    xBuf[2] = (keyNumber) ? MIFARE_CMD_AUTH_B : MIFARE_CMD_AUTH_A;
    xBuf[3] = blockNumber; //Block Number (1k=0..63, 4k = 0..255)
    memcpy (xBuf + 4, _key, 6);
    for(i = 0; i < _uidLen; i++) {
        xBuf[10+i] = _uid[i];

    }
    if(writeCommand(&xBuf, 10+_uidLen, 0, 0)) {
        return 0;
    }
    readResponse(buf, sizeof(*buf), 1000);

    printf("buf0: 0x%X", *(*buf));
    printf(" buf1: 0x%X", *(*buf+1));
    printf(" buf0: 0x%X", *(*buf+2));
    printf("\n");


    if(**buf != 0x00) {
        printf("Authentification failed and returned: 0x%X\n", **buf);
        return 0;
    }
    return 1;
};





void PN532::RFID_init() {
    //Check connection
    sleep_ms(3000);


    uint32_t response;
    pn532_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;
    printf("About to try and write\n");
    sleep_ms(500);
    writeCommand(&pn532_packetbuffer, 1, 0, 0);
    printf("Size of pn532_packetbuffer: %d \n", sizeof(pn532_packetbuffer));
    int16_t status = readResponse(&pn532_packetbuffer, sizeof(pn532_packetbuffer), 1000);
    if(status == -1) {
        printf("Timed out\n");
    }
    if(status == -3) {
        printf("ERROR: PN532 Invalid Frame\n");
    }

    for(int loops = 0; loops < sizeof(pn532_packetbuffer); loops++) {
        printf("PN532_packetbuffer[%d] = %X \n", loops, pn532_packetbuffer[loops]);
    }

    if(status >= 0) {
        response |= pn532_packetbuffer[0];
        response <<= 8;
        response |= pn532_packetbuffer[1];
        response <<= 8;
        response |= pn532_packetbuffer[2];
        response <<= 8;
        response |= pn532_packetbuffer[3];

        printf("Found chip PN5"); printf("%X", (response >> 24) & 0xFF); printf("\n");
        printf("Firmware ver. "); printf("%d",(response >> 16) & 0xFF);
        printf("."); printf("%d",(response >> 8) & 0xFF); printf("\n");
        sleep_ms(250);

    }


    



};

void PN532::readMifare() {
    bool success = false;
    while(!success) {
        success = readPassiveTargetID(&pn532_packetbuffer, PN532_MIFARE_ISO14443A, &uid, &uidLength, 1000);
        sleep_ms(1000);
    }

    printf("Found an ISO1443A Card...\n");
    printf("\tUID Length:\t %d bytes\n", uidLength);
    printf("\tUID Value:\t %X %X %X %X\n",uid[0], uid[1], uid[2], uid[3]);

    if(uidLength == 4) {
        printf("Seems to be a Mifare Classic card (4 byte UID)\n");
        

        // Now we need to try to authenticate it for read/write access
        // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
        printf("Trying to authenticate block 4 with default KEYA value\n");
        uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        
        // Start with block 4 (the first block of sector 1) since sector 0
        // contains the manufacturer data and it's probably better just
        // to leave it alone unless you know what you're doing
        success = mifareclassic_AuthenticateBlock(&pn532_packetbuffer, uid, uidLength, 4, 0, keya);

        if(success) {
            //printf("Sector 1 (Blocks 4..7) has been authenticated\n");
            uint8_t data[16];// = { 'T', 'e', 's', 't', 'i', 'n', 'g', 0, 'w', 'r', 'i', 't', 'e', 0, 0, 0};
            /*success = this->mifareclassic_WriteDataBlock (4, data);
            if (success){
                printf("Wrote data to block 4\n");
            }*/
            success = mifareclassic_ReadDataBlock(4, data);

            //data read
            if(success) {
                printf("Reading Block 4:\n");
                for(int i = 0; i < sizeof(data); i++) {
                    printf("%X ", data[i]);
                }
                printf("\n");

            } else {
                printf("Unable to read requested block.  Try another.\n");
            }
        } else {
            printf("Authentication failed: Try another key.\n");
        }
    }
    if (uidLength == 7) {
        printf("Seems to be a Mifare Ultralight tag (7 byte UID)\n");
        printf("Reading page 4");
        uint8_t data[32];
        success = mifareultralight_ReadPage (4, data);
        if(success) {
            for(int i = 0; i < 4; i++) {
                printf("%X ", data[i]);
            }
            printf("\n");
        }
    }
    printf("Done\n"); 
};


uint8_t PN532::mifareclassic_WriteDataBlock (uint8_t blockNumber, uint8_t *data){
    /* Prepare the first command */
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 1;                      /* Card number */
    pn532_packetbuffer[2] = MIFARE_CMD_WRITE;       /* Mifare Write command = 0xA0 */
    pn532_packetbuffer[3] = blockNumber;            /* Block Number (0..63 for 1K, 0..255 for 4K) */
    memcpy (pn532_packetbuffer + 4, data, 16);

    //Send the command
    writeCommand(&pn532_packetbuffer, 20, 0, 0);

    return(0 < readResponse(&pn532_packetbuffer, sizeof(pn532_packetbuffer), 1000));
};


/**************************************************************************/
/*!
    Tries to read an entire 16-bytes data block at the specified block
    address.

    @param  blockNumber   The block number to authenticate.  (0..63 for
                          1KB cards, and 0..255 for 4KB cards).
    @param  data          Pointer to the byte array that will hold the
                          retrieved data (if any)

    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t PN532::mifareclassic_ReadDataBlock (uint8_t blockNumber, uint8_t *data) {
    printf("Trying to read 16 bytes from block %d", blockNumber);

        /* Prepare the command */
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 1;                      /* Card number */
    pn532_packetbuffer[2] = MIFARE_CMD_READ;        /* Mifare Read command = 0x30 */
    pn532_packetbuffer[3] = blockNumber;            /* Block Number (0..63 for 1K, 0..255 for 4K) */

    writeCommand(&pn532_packetbuffer, 4, 0, 0);
    readResponse(&pn532_packetbuffer, sizeof(pn532_packetbuffer), 1000);

    //check for error
    if (pn532_packetbuffer[0] != 0x00) {
        return 0;
    }

    /* Copy the 16 data bytes to the output buffer        */
    /* Block content starts at byte 9 of a valid response */
    memcpy (data, pn532_packetbuffer + 1, 16);

    return 1;
}


/***** Mifare Ultralight Functions ******/

/**************************************************************************/
/*!
    Tries to read an entire 4-bytes page at the specified address.

    @param  page        The page number (0..63 in most cases)
    @param  buffer      Pointer to the byte array that will hold the
                        retrieved data (if any)
*/
/**************************************************************************/
uint8_t PN532::mifareultralight_ReadPage (uint8_t page, uint8_t *buffer) {
    
    if (page >= 64) {
        printf("Page value out of range\n");
        return 0;
    }

    /* Prepare the command */
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 1;                   /* Card number */
    pn532_packetbuffer[2] = MIFARE_CMD_READ;     /* Mifare Read command = 0x30 */
    pn532_packetbuffer[3] = page;                /* Page Number (0..63 in most cases) */

    writeCommand(&pn532_packetbuffer, 4, 0, 0);

    readResponse(&pn532_packetbuffer, sizeof(pn532_packetbuffer), 1000);

    if (pn532_packetbuffer[0] == 0x00) {
        /* Copy the 4 data bytes to the output buffer         */
        /* Block content starts at byte 9 of a valid response */
        /* Note that the command actually reads 16 bytes or 4  */
        /* pages at a time ... we simply discard the last 12  */
        /* bytes                                               */
        memcpy (buffer, pn532_packetbuffer + 1, 4);
    } else {
        return 0;
    }
    return 1;
}