#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "task.h"

#include "ziki.h"

#define LED_OFF 0
#define LED_ON 1
#define LED_CURSOR 2
#define LED_FREQ 6

uint32_t prev_Gnd[8] = {0,0,0,0,0,0,0,0}; //holds previous ground vals

uint8_t CURSOR_COUNT = 0;

void render(uint16_t pattern[9][8]);

void configure_pins();

void visual_main(void* p) {
    configure_pins();

    uint8_t selected[9][8] = {{0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0},
                              {0,0,0,1,0,0,0,0},
                              {0,0,0,0,0,0,0,0},
                              {0,0,0,1,0,0,0,0},
                              {0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0}};

    uint8_t show[9][8] =     {{1,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0}};

    while (1) {
        render(selected);
    }
}

void configure_pins() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
                                | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10
                                | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void render(uint16_t pattern[9][8]) {
    //multitplex
    for(uint8_t i=0; i<8 ; i++){
        GPIOA->ODR = 1<<(7-i);
        GPIOE->ODR = prev_Gnd[i];
        for(uint16_t i=0; i<10000 ; i++){}
    }

    //update our prev
    for (uint8_t i = 0; i < 8; i++) {
        uint16_t col_sum = 0;
        for(uint8_t j = 0; j < 9  ; j++) {
            if(pattern[j][i] == 1){}
            else{
                col_sum += (1 << (15-j));
            }
        }
        prev_Gnd[i] = col_sum;
    }
}
