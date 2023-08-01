#pragma once

#include "Defines.h"






//const uint8_t *body = 0, uint8_t blen = 0

int8_t writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen); //Default for Body and Blen is 0

int8_t readAckFrame();

int16_t readResponse(uint8_t buf[], uint8_t len, uint16_t timeout);



    
static int addr =  (0x48 >> 1);
