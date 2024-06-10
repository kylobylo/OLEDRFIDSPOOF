

#include "Defines.h"
#include "RFID.hpp"
#include "shodan.hpp"
#define upPin 18
#define downPin 19
#define selectPin 20




const uint LED_PIN = 25;



int main() {

    u_int8_t inputDetected = 0;

    uint8_t optionNum = 0;


    stdio_init_all();

    sleep_ms(1000);

    printf("Starting up!\n");
    sleep_ms(1000);


    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));
    bi_decl(bi_2pins_with_func(i2cSDA, i2cSCL, GPIO_FUNC_I2C));


    i2c_init(i2cPORT, 100000);

    gpio_set_function(i2cSDA, GPIO_FUNC_I2C);
    gpio_set_function(i2cSCL, GPIO_FUNC_I2C);
   // gpio_pull_up(i2cSDA);
    //gpio_pull_up(i2cSCL);

    PN532::RFID_init();
    PN532::getStatus(true);
    PN532::SAMConfig();



    gpio_init(upPin);
    gpio_init(downPin);
    gpio_init(selectPin);
    gpio_set_dir(upPin, GPIO_IN);
    gpio_set_dir(downPin, GPIO_IN);
    gpio_set_dir(selectPin, GPIO_IN);




    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);
    ssd1351_init();
    gpio_put(LED_PIN, 0);


    shodan::displayRefresh(1);
    ssd1351_refresh();



    
    while(1) {
        while(inputDetected == 0){
            if(gpio_get(upPin)) {
                inputDetected = 1;
            }
            if(gpio_get(downPin)) {
                inputDetected = 2;
            }
            if(gpio_get(selectPin)) {
                inputDetected = 3;
            }
        }

        if(inputDetected == 1 && optionNum) {
            gpio_put(LED_PIN, 1);
            printf("Up Input Detected\n");
            optionNum--;
        }
        
        if(inputDetected == 2 && !optionNum) {
            gpio_put(LED_PIN, 0);
            printf("Down Input Detected\n");
            optionNum++;
        }

        if(optionNum == 0){
            shodan::displayRefresh(nullScreenNum, optionNum);
            ssd1351_draw_line(BUFFER_TYPE_DISPLAY, shodan::white, (point_t){7, 46}, (point_t){119, 46});
        }
        if(optionNum == 1){
            shodan::displayRefresh(nullScreenNum, optionNum);
            ssd1351_draw_line(BUFFER_TYPE_DISPLAY, shodan::white, (point_t){7, 58}, (point_t){119, 58});
        }
        if(inputDetected == 3) {
            shodan::selectButtonPress(optionNum);
        }

        inputDetected = 0;
        ssd1351_refresh();
    
    }
    


}