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

    uint8_t selected[9][8] = {{0,0,1,1,1,1,0,0},
                               {0,1,0,0,0,0,1,0},
                               {1,0,0,1,1,0,0,1},
                               {1,0,1,0,0,1,0,1},
                               {1,0,1,0,0,1,0,1},
                               {1,0,1,0,0,1,0,1},
                               {1,0,0,1,1,0,0,1},
                               {0,1,0,0,0,0,1,0},
                               {0,0,1,1,1,1,0,0}};

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

    //GPIOA->ODR = 0b11110000;
    //GPIOE->ODR = 0b1010101010000000;

    /*
    for (uint32_t cnt = 0; cnt < 100000; cnt++) {
            for (uint8_t i = 0; i < 8; i++) {
                GPIOA->ODR = 1 << i;
                uint16_t sub = 0x1FF; // holds E7->E15
                for (uint8_t j = 0; j < 9; j++) {
                    sub ^= (pattern[j][i] << j);
                }
                GPIOE->ODR = sub << 7;
            }
    }
    */



    /*
    uint8_t vcc[8] = {0b10000000,
                      0b01000000,
                      0b00100000,
                      0b00010000,
                      0b00001000,
                      0b00000100,
                      0b00000010,
                      0b00000001};
  */
    //GPIOA->ODR = 0b10000000;
    //GPIOE->ODR = 0b0000100000000000;

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

    /*
    for(uint16_t loop ; loop <500 ; loop++){
        for(uint8_t i=0 ; i<8 ; i++){
            GPIOA->ODR = 0b1 << (7-i));
            uint16_t col_sum = 0x1FF; // holds E7->E15
            for(uint8_t j=0 ; j<9 ; i++){
                col_sum ^= (pattern[j][i] << j);
            }
            GPIOE->ODR = col_sum << 7;
        }
    }
    */
}
