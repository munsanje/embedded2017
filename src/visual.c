#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "task.h"

#include "ziki.h"


#define LED_OFF 0
#define LED_ON 1
#define LED_CURSOR 2

#define LED_FREQ 6

uint8_t CURSOR_COUNT = 0;

void render(uint8_t pattern[4][4]);

void configure_pins();

void write_gpio_vdd(uint8_t bits);
void write_gpio_gnd(uint16_t bits);

void visual_main(void* p) {
    configure_pins();

    /*uint8_t selected[4][4] = {{1,0,0,1},{0,0,0,0},{0,0,1,0},{0,0,0,0}};*/
    /*uint8_t show[4][4] = {{1,0,0,0},{1,0,0,0},{0,0,0,0},{0,0,0,0}};*/

    /*uint8_t x, y, input, coords, save;*/

    /*while (1) {*/
        /*xQueueReceive(Global_Queue_Handle, &input, 1);*/


        /*coords = 0b1111 & input;*/
        /*x = coords >> 2;*/
        /*y = 0b11 & coords;*/

        /*save = (0b10000 & input) >> 4;*/
        /*if (save) {*/
            /*selected[x][y] = ~selected[x][y];*/
        /*}*/

        /*for (uint8_t i = 0; i < 4; i++) {*/
            /*for (uint8_t j = 0; j < 4; j++) {*/
                /*show[i][j] = selected[i][j];*/
            /*}*/
        /*}*/
        /*show[x][y] = LED_CURSOR;*/

        /*render(show);*/
    /*}*/
    GPIOB->ODR = 0b1010100000000000;
    GPIOD->ODR = 0b0000000000000000;
    GPIOE->ODR = 0b1010101010000000;
}


void write_gpio_vdd(uint8_t bits) {
}

void write_gpio_gnd(uint16_t bits);

void configure_pins() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
                                | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 |  GPIO_Pin_8 |  GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11
                                | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void render(uint8_t pattern[4][4]) {
    // increment count appropriately
    CURSOR_COUNT = (CURSOR_COUNT + 1) % (LED_FREQ + 1);

    // determine cursors
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            if (pattern[i][j] == LED_CURSOR) {
                pattern[i][j] = (CURSOR_COUNT > LED_FREQ/2);
            }
        }
    }

    uint8_t row[4] = {
        0b10001111,
        0b01001111,
        0b00101111,
        0b00011111
    };

    // compute rows
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t col = 0;
        for (uint8_t j = 0; j < 4; j++) {
            col += (pattern[j][i] << (3-j));
        }
        row[i] ^= col;
    }

    // write to LEDs
    for (uint32_t i = 0; i < 100000; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            GPIOA->ODR = row[j];
        }
    }
}
