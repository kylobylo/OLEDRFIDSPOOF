#pragma once

#include "Defines.h"
#include "RFID.hpp"

#define NFCScreenNum 1
#define readCardScreenNum 2
#define fileNameSelect 3
#define nullScreenNum 0
extern "C" {
    #include "ssd1351.h"
}

class shodan {

    public:

        static void displayRefresh(uint8_t screenNumber = screen, uint8_t optionNum = option);

        static void drawNFCSelect();

        static void drawReadCard(uint8_t optionNum, bool newData = false);

        static std::string hexify(unsigned int n);

        static uint8_t selectButtonPress(uint8_t optionNum);


        //make color
        static uint16_t white;
        static uint16_t blue;
        static uint16_t red;
        static uint16_t green;

    private:

        //Internal reference for what screen we are displaying.
        static uint8_t screen;

        static uint8_t option;

        static std::string uidstring;

        static uint8_t oldScreen;



};