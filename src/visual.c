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

void visual_main(void* p) {
    configure_pins();

    uint8_t selected[4][4] = {{1,0,0,1},{0,0,0,0},{0,0,1,0},{0,0,0,0}};
    uint8_t show[4][4] = {{1,0,0,0},{1,0,0,0},{0,0,0,0},{0,0,0,0}};

    uint8_t x, y, coords;

    while (1) {
        xQueueReceive(Global_Queue_Handle, &coords, 1);

        x = coords >> 2;
        y = 0b11 & coords;

        for (uint8_t i = 0; i < 4; i++) {
            for (uint8_t j = 0; j < 4; j++) {
                show[i][j] = selected[i][j];
            }
        }
        show[x][y] = LED_CURSOR;

        render(show);
    }
}

void configure_pins() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
                                | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
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