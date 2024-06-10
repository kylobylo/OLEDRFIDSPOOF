#include "shodan.hpp"

uint8_t shodan::screen = 1;
uint8_t shodan::option = 0;

uint8_t shodan::oldScreen = 1;

std::string shodan::uidstring = "\0";

uint16_t shodan::white = ssd1351_make_colour(0xff, 0xff, 0xff);
uint16_t shodan::green = ssd1351_make_colour(0x00, 0xff, 0x00);
uint16_t shodan::blue = ssd1351_make_colour(0x00, 0x00, 0xff);
uint16_t shodan::red = ssd1351_make_colour(0xff, 0x00, 0x00);

void shodan::displayRefresh(uint8_t screenNumber, uint8_t optionNum) {
    ssd1351_clear();
    bool newData = false;
    if(screenNumber != 0) {
        screen = screenNumber;
    } else if(screen != oldScreen) {
        newData = true;
        oldScreen = screen;
    }
    option = optionNum;
    switch(screen) {
        case NFCScreenNum:
            printf("Drawing NFC Select screen\n");
            drawNFCSelect();
            break;
        case readCardScreenNum:
            printf("Printing out UID\n");
            drawReadCard(optionNum, newData);
            break;
        default:
            printf("Error, invalid screen number!\n");
    }

}

void shodan::drawReadCard(uint8_t optionNum, bool newData) {
        ssd1351_write_text(BUFFER_TYPE_DISPLAY, "Read:", (point_t){10,10}, green, FONT_LARGE, true, false);
        ssd1351_draw_line(BUFFER_TYPE_DISPLAY, white, (point_t){7, 30}, (point_t){119, 30});

        shodan::screen = readCardScreenNum;
        std::stringstream uidstringstream;

        if(newData){
            char digits[4];
            uidstringstream << "UID: ";
            for(int i = 0; i < PN532::uidLength; i++){
                //PN532::to_hex(digits, sizeof(digits), &PN532::uid[i], sizeof(PN532::uid[i]) );
                digits[i] = PN532::uid[i];
                printf("Printing string chars: %s\n", hexify(digits[i]));
                uidstringstream << hexify(digits[i]);
            }
        
            uidstring = uidstringstream.str();
        }
        printf("uidstring: %s \n", uidstring.c_str());
        uint8_t length = uidstring.length();
        char* char_array = new char[length + 1];
        char* save = "Save";
        char* load = "Load";
        // copying the contents of the 
        // string to char array 
        strcpy(char_array, uidstring.c_str()); 
        ssd1351_write_text(BUFFER_TYPE_DISPLAY, char_array, (point_t){10,40}, white, FONT_IBM, true, false);
        ssd1351_write_text(BUFFER_TYPE_DISPLAY, save, (point_t){10,70}, white, FONT_IBM, true, false);
        ssd1351_write_text(BUFFER_TYPE_DISPLAY, load, (point_t){10,85}, white, FONT_IBM, true, false);
        switch (optionNum) {
            //Save
            case 0:
                ssd1351_draw_line(BUFFER_TYPE_DISPLAY, white, (point_t){10, 78}, (point_t){120,78});
                break;
            //Load
            case 1:
                ssd1351_draw_line(BUFFER_TYPE_DISPLAY, white, (point_t){10, 93}, (point_t){120,93});
                break;
            default:
                break;

        }

}

uint8_t shodan::selectButtonPress(uint8_t optionNum) {

    switch(screen) {
        case NFCScreenNum:
            switch (optionNum)
            {
                case 0:
                    PN532::readMifare();
                    displayRefresh(readCardScreenNum, optionNum);
                    break;
                case 1:
                    
                    break;
                default:
                    break;
            }
            break;
        case readCardScreenNum:
            switch(optionNum) {
                case 0:
                    
                    displayRefresh(fileNameSelect, optionNum);
            }
            break;
        default:
            break;

        
    }
    return(0);
}

void shodan::drawNFCSelect() {
    ssd1351_write_text(BUFFER_TYPE_DISPLAY, "NFC Func:", (point_t){10,10}, white, FONT_LARGE, true, false);
    ssd1351_write_text(BUFFER_TYPE_DISPLAY, "Read Tag", (point_t){10, 38}, white, FONT_IBM, true, false);
    ssd1351_write_text(BUFFER_TYPE_DISPLAY, "Write Tag", (point_t){10, 50}, white, FONT_IBM, true, false);
    ssd1351_draw_line(BUFFER_TYPE_DISPLAY, white, (point_t){7, 30}, (point_t){119, 30});
}


std::string shodan::hexify(unsigned int n)
{
  std::string res;

  do
  {
    res += "0123456789ABCDEF"[n % 16];
    n >>= 4;
  } while(n);

  return std::string(res.rbegin(), res.rend());
}