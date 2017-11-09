#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "task.h"

#include "ziki.h"

#include "codec.h"
#include "piano.h"


#define LED_OFF 0
#define LED_ON 1
#define LED_CURSOR 2

#define LED_FREQ 6

uint8_t CURSOR_COUNT = 0;

void render(uint8_t pattern[9][8]);

void play(uint8_t pattern[8][8]);

void setup_leds();
void setup_sound();

void output_main(void* p) {
    setup_leds();
    setup_sound();

    uint8_t selected[8][8] = {
        {1,0,0,0,0,0,0,0},
        {0,1,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,0},
        {0,0,0,1,0,0,0,0},
        {0,0,0,0,1,0,0,0},
        {0,0,0,0,0,1,0,0},
        {0,0,0,0,0,0,1,0},
        {0,0,0,0,0,0,0,1},
    };

    /*uint8_t x, y, input, coords, save;*/
    while (1) {
        /*xQueueReceive(Q_HANDLE_INPUT_OUTPUT, &coords, 1);*/

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

        play(selected);
        render(selected);
    }

}

 
void render(uint8_t pattern[9][8]) {
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
}

void play(uint8_t pattern[8][8]) {
    uint8_t count = 0;
	uint16_t i = 0, j = 0, wave = 0;
	
	for (; j < COL_SIZE;) {
		if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE)) {
            SPI_I2S_SendData(CODEC_I2S, wave);

            // only update on every second sample to insure that L & R ch. have the same sample value
            if (count & 1) {
                if ((i < PIANO_SIZE) && (j < COL_SIZE)) {
                    wave = 0;
                    for (uint8_t k = 0; k < COL_SIZE; k++) {
                        wave += (piano[COL_SIZE-1-k][i]/8) * pattern[k][j];
                    }
                    i++;
                } else {
                    i = 0;
                    j++;
                }

            }
            count++;
        }
	}
}

void setup_leds() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
                                | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 |  GPIO_Pin_8 |  GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11
                                | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void setup_sound() {
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	codec_init();
	codec_ctrl_init();
	I2S_Cmd(CODEC_I2S, ENABLE);
}
