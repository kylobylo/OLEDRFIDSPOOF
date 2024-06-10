#pragma once

#include "Defines.h"



static int addr =  (0x24);


//const uint8_t *body = 0, uint8_t blen = 0
class PN532 {
    public:
    
        static int8_t writeCommand(uint8_t (*header)[64], uint8_t hlen, const uint8_t *body, uint8_t blen); //Default for Body and Blen is 0

        static int8_t readAckFrame();

        static int16_t readResponse(uint8_t (*buf)[64], uint8_t len, uint16_t timeout);

        static uint8_t pn532_packetbuffer[64];

        static bool SAMConfig();  

        static bool getStatus(bool msg);

        static void dumpBuffer();

        static uint8_t mifareclassic_ReadDataBlock (uint8_t blockNumber, uint8_t *data);

        static uint8_t mifareultralight_ReadPage (uint8_t page, uint8_t *buffer);

        static void clearBuffer();

        static uint8_t mifareclassic_WriteDataBlock (uint8_t blockNumber, uint8_t *data);

        static bool readPassiveTargetID(uint8_t (*buf)[64], uint8_t cardbaudrate, uint8_t (*uid)[7], uint8_t *uidLength, uint16_t timeout);

        static void RFID_init();

        static void readMifare();

        static int16_t getResponseLength(uint8_t (*buf)[64], uint8_t len, uint16_t timeout);

        static bool to_hex(char* dest, size_t dest_len, const uint8_t* values, size_t val_len);

        static uint8_t mifareclassic_AuthenticateBlock (uint8_t (*buf)[64], uint8_t *uid, uint8_t uidLen, uint32_t blockNumber, uint8_t keyNumber, uint8_t *keyData);

        static uint8_t _uid[7];  // ISO14443A uid
        static uint8_t _uidLen;  // uid len
        static uint8_t _key[6];  // Mifare Classic key
        static uint8_t inListedTag; // Tg number of inlisted tag.
        static uint8_t _felicaIDm[8]; // FeliCa IDm (NFCID2)
        static uint8_t _felicaPMm[8]; // FeliCa PMm (PAD)




        char c;
        static uint8_t uidLength;
        static uint8_t uid[7];  // Buffer to store the returned UID


};