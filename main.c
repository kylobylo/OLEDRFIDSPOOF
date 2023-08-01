

#include "Defines.h"
#include "RFID.h"


const uint LED_PIN = 25;
uint8_t pn532_packetbuffer[64];


char c;


void getStatus() {
    printf("Getting Status\n");
    i2c_read_blocking(i2cPORT, addr, pn532_packetbuffer[0], 1, false);
    printf("Status Reads: %X \n", pn532_packetbuffer[0]);
}

void RFID_init() {
    //Check connection
    sleep_ms(1000);


    uint32_t response;
    pn532_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;
    printf("About to try and write\n");
    sleep_ms(500);
    writeCommand(&pn532_packetbuffer[0], 1, 0, 0);
    int16_t status = readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer), 1000);
    if(status = -1) {
        printf("Timed out\n");
    }


    if(status >= 0) {
        response |= pn532_packetbuffer[0];
        response <<= 8;
        response |= pn532_packetbuffer[1];
        response <<= 8;
        response |= pn532_packetbuffer[2];
        response <<= 8;
        response |= pn532_packetbuffer[3];
    
        sleep_ms(5000);

        printf("Found chip PN5"); printf("%X", (response >> 24) & 0xFF); printf("\n");
        printf("Firmware ver. "); printf((response >> 16) & 0xFF);
        printf('.'); printf((response >> 8) & 0xFF); printf("\n");
        sleep_ms(250);

        //i2c_read_blocking(i2cPORT, addr, pn532_packetbuffer, 64, false);
       /* for(int i = 0; i < 64; i++) {
            printf("%X", pn532_packetbuffer[i]);
            printf("\n");
        }*/
    }
    

    //Set Max Retries
    pn532_packetbuffer[0] = PN532_COMMAND_RFCONFIGURATION;
    pn532_packetbuffer[1] = 5;    // Config item 5 (MaxRetries)
    pn532_packetbuffer[2] = 0xFF; // MxRtyATR (default = 0xFF)
    pn532_packetbuffer[3] = 0x01; // MxRtyPSL (default = 0x01)
    pn532_packetbuffer[4] = 0xFF;

    i2c_write_blocking(i2cPORT, addr, pn532_packetbuffer, 5, true);



}



int main() {

    sleep_ms(1000);

    stdio_init_all();

    printf("Starting up!\n");

    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));


    i2c_init(i2cPORT, 100000);

    gpio_set_function(i2cSDA, GPIO_FUNC_I2C);
    gpio_set_function(i2cSCL, GPIO_FUNC_I2C);
    gpio_pull_up(i2cSDA);
    gpio_pull_up(i2cSCL);

    RFID_init();
    getStatus();




    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);
    ssd1351_init();
    gpio_put(LED_PIN, 0);

    //make color
    uint16_t white = ssd1351_make_colour(0xff, 0xff, 0xff);
    ssd1351_clear();
    ssd1351_write_text(BUFFER_TYPE_DISPLAY, "TESTING", (point_t){10,10}, white, FONT_LARGE, true, false);
    ssd1351_write_text_centered(BUFFER_TYPE_DISPLAY, "Testing", (uint8_t){50}, white, FONT_IBM, true, false);
    ssd1351_draw_line(BUFFER_TYPE_DISPLAY, white, (point_t){7, 60}, (point_t){119, 60});
    ssd1351_refresh();
    printf("Done\n");
}