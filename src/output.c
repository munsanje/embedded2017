#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "ziki.h"


void render(uint8_t pattern[4][4],uint8_t coords);

void configure_pins();

uint8_t x_global, y_global, count = 0;

void output_main(void* p) {
    configure_pins();

    uint8_t a[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    uint8_t b[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

    uint8_t x,y,coords,save = 0;

    uint8_t count2 = 0;
    while (1) {
        x_global, y_global = 0;

        xQueueReceive(Global_Queue_Handle, &coords, 1);
        //xQueueReceive(Global_Queue_Handle, &save, 1);

        x = coords >> 2;
        y = 0b11 & coords;

        /*
        if(save == 1){
              a[x][y] = ~b[x][y];
        }
        */
        b[x][y] = 1;
        for (uint8_t i = 0; i < 4; i++) {
            for (uint8_t j = 0; j < 4; j++) {
                a[i][j] = b[i][j];
            }
        }

        a[x][y] = 1;
        render(a,coords);
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

void render(uint8_t pattern[4][4],uint8_t coords) {
    // multiplexing masks
    x_global = coords >> 2;
    y_global = 0b11 & coords;

    //test
    if(count < 2){
        pattern[x_global][y_global] = 0;
        count++;
    }
    else{
        count = 0;
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
    count++;
}
