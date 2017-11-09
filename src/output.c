#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "task.h"

#include "ziki.h"
#include "patterns.h"

#include "codec.h"
#include "piano.h"


#define LED_OFF 0
#define LED_ON 1
#define LED_CURSOR 2
#define LED_FREQ 20

uint8_t CURSOR_COUNT = 0;

uint32_t prev_Gnd[8] = {0}; //holds previous ground vals

void play(uint8_t pattern[8][8]);
void render(uint16_t pattern[9][8]);

void box_animation();

void setup_leds();
void setup_sound();

void output_main(void* p) {
    setup_leds();
    setup_sound();

    uint16_t selected[9][8] = {0};
    uint16_t show[9][8] = {0};

    uint16_t coords;
    int8_t x, y;
    uint8_t save;
    uint8_t prevSave;

    uint8_t pattern[8][8] = {
        {1,0,0,0,0,0,0,0},
        {0,1,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,0},
        {0,0,0,1,0,0,0,0},
        {0,0,0,0,1,0,0,0},
        {0,0,0,0,0,1,0,0},
        {0,0,0,0,0,0,1,0},
        {0,0,0,0,0,0,0,1},
    };

    while (1) {
        xQueueReceive(Q_HANDLE_INPUT_OUTPUT, &coords, 1);
        x = 7-(coords >> 3);
        y = (0b111 & coords)+1;

        save = (coords >> 6) & 1;
        if(save == 1 && prevSave == 0){
            selected[y+1][x] = 1 - selected[y+1][x];
            box_animation();
        }
        prevSave = save;

        for (uint8_t i = 0; i < 9; i++) {
            for (uint8_t j = 0; j < 8; j++) {
                show[i][j] = selected[i][j];
            }
        }
        show[y][x] = LED_CURSOR;
        render(show);
    }
}

void play(uint8_t pattern[8][8]) {
    uint8_t count = 0;
	for (uint16_t i = 0; i < COL_SIZE; i++) {
        for (uint16_t j = 0; j < PIANO_SIZE*2; j++) { // size*2 to update L+R

            uint16_t wave = 0;
            for (uint8_t k = 0; k < COL_SIZE; k++) {
                wave += (piano[k][j/2]/8) * pattern[k][i]; // j/2 to update L+R
            }

            // wait for flag to clear then send
            while (!SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE)) {}
            SPI_I2S_SendData(CODEC_I2S, wave);
        }
	}
}

void render(uint16_t pattern[9][8]) {
    //blink
    CURSOR_COUNT = (CURSOR_COUNT+1) % (LED_FREQ+1);
    for(uint8_t i = 0 ; i < 9 ; i++){
          for(uint8_t j = 0 ; j < 8 ; j++ ){
                if(pattern[i][j] == LED_CURSOR){
                      pattern[i][j] = CURSOR_COUNT>(LED_FREQ/2);
                }
          }
    }

    //multiplex
    for(uint16_t i=0; i<8 ; i++){
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

void box_animation(){
    for(uint16_t i = 0 ; i < 3 ; i++){render(box1);}
    for(uint16_t i = 0 ; i < 3 ; i++){render(box2);}
    for(uint16_t i = 0 ; i < 3 ; i++){render(box3);}
    for(uint16_t i = 0 ; i < 3 ; i++){render(box4);}
}

void setup_leds() {
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
