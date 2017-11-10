#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include <stdbool.h>
#include <math.h>

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

float TEMPO[8] = {1, 1.25, 1.5, 1.75, 2, 2.5, 3, 3.5};

uint32_t prev_gnd[8] = {0}; //holds previous ground vals

void play(uint8_t pattern[9][8], uint8_t tempo);
void render(uint8_t pattern[9][8]);

void box_animation();
void intro_animation();
void zeroin_animation(int8_t x, int8_t y);

void setup_leds();
void setup_sound();

void output_main(void* p) {
    setup_leds();
    setup_sound();

    uint8_t selected[9][8] = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,0,1},
        {0,0,0,0,1,0,1,0},
        {0,0,0,1,0,1,0,1},
        {0,0,1,0,1,0,1,0},
        {0,1,0,1,0,1,0,0},
        {1,0,1,0,1,0,0,0},
        {0,1,0,1,0,0,0,0},
        {1,0,1,0,0,0,0,0},
    };
    uint8_t show[9][8] = {0};

    uint16_t input_rcv;
    uint8_t instrument_sel, tempo, playback, save;
    int8_t x, y = 0;
    uint8_t prev_save, prev_inst, prev_tempo, prev_play;

    uint8_t tempo_index = 0;

    uint8_t inst_shift = 0;

    intro_animation();
    for (;;) {
        xQueueReceive(Q_HANDLE_INPUT_OUTPUT, &input_rcv, 1);
        instrument_sel = (input_rcv >> 9) & 1;
        tempo = (input_rcv >> 8) & 1;
        playback = (input_rcv >> 7) & 1;
        save = (input_rcv >> 6) & 1;
        x = 7-(input_rcv >> 3);
        y = (0b111 & input_rcv)+1;

        if(save == 1 && prev_save == 0){
            selected[y+1][x] = 1 - selected[y+1][x];
            box_animation(x, y+1);
        }
        prev_save = save;

        for (uint8_t i = 0; i < 9; i++) {
            for (uint8_t j = 0; j < 8; j++) {
                show[i][j] = selected[i][j];
            }
        }
        show[y][x] = LED_CURSOR;

        if (playback && prev_play == 0) {
            vTaskSuspend(HANDLE_INPUT);
            for (uint8_t i = 0; i < 3; i++) {
                play(selected, tempo_index);
            }
            setup_sound();
            vTaskResume(HANDLE_INPUT);
        } else {
            render(show);
        }
        prev_play = playback;

        if (tempo && instrument_sel) {
            for (uint8_t i=0; i < 8; i++) {
                for (uint8_t j=1; j < 9; j++) {
                    selected[j][i] = 0;
                }
            }
            continue; // don't change when pressing both 
        }

        if (tempo && prev_tempo == 0) {
            tempo_index = (tempo_index + 1) % 8;
        }
        for (uint8_t i=0; i < 8; i++) {
            selected[0][i] = (i == tempo_index) ? 1 : 0;
        }
        prev_tempo = tempo;


        GPIOC->ODR = ((1 << inst_shift) << 13);
        if (instrument_sel && prev_inst == 0) {
            inst_shift = (inst_shift + 1) % 3;
        }
        prev_inst = instrument_sel;
    }
}

void play(uint8_t pattern[9][8], uint8_t tempo) {
    uint8_t count = 0;
    uint16_t size = (uint16_t) ( ((float)PIANO_SIZE) / TEMPO[tempo] );
	for (uint16_t i = 0; i < COL_SIZE; i++) {
        GPIOE->ODR = 0x1FF << 7; 
        GPIOE->ODR = ~(1 << 15);
        for (uint8_t j = 0; j < 8; j++) {
            GPIOE->ODR &= ~((pattern[j+1][i] << (7-j)) << 7);
        }
        GPIOA->ODR = 1 << (7-i);
        GPIOC->ODR &= 0b1111111111110111;
        if ((7-i) == 4) {
            GPIOC->ODR |= 1 << 3;
        }
        for (uint16_t j = 0; j < size*2; j++) { // size*2 to update L+R
            uint16_t wave = 0;
            for (uint8_t k = 0; k < COL_SIZE; k++) {
                wave += (piano[k][j/2]/8) * pattern[k+1][7-i]; // j/2 to update L+R
            }

            // wait for flag to clear then send
            while (!SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE)) {}
            SPI_I2S_SendData(CODEC_I2S, wave);
        }
	}
}

void render(uint8_t pattern[9][8]) {
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
        GPIOA->ODR = 1 << (7-i);
        GPIOC->ODR &= 0b1111111111110111;
        if ((7-i) == 4) {
            GPIOC->ODR |= 1 << 3;
        }
        GPIOE->ODR = prev_gnd[i];
        for(uint16_t i=0; i<1000 ; i++){
            // audio clarity hack
            if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE)) {
                SPI_I2S_SendData(CODEC_I2S, 0);
            }
        }
    }

    //update our prev
    for (uint8_t i = 0; i < 8; i++) {
        uint16_t col_sum = 0;
        for(uint8_t j = 0; j < 9  ; j++) {
            if (pattern[j][i] == 1){}
            else {
                col_sum += (1 << (15-j));
            }
        }
        prev_gnd[i] = col_sum;
    }
}

void box_animation(){
    for(uint16_t i = 0 ; i < 3 ; i++){render(box1);}
    for(uint16_t i = 0 ; i < 3 ; i++){render(box2);}
    for(uint16_t i = 0 ; i < 3 ; i++){render(box3);}
    for(uint16_t i = 0 ; i < 3 ; i++){render(box4);}
}

void intro_animation(){
    for(uint16_t i = 0 ; i < 10 ; i++){render(box4);}
    for(uint16_t i = 0 ; i < 10 ; i++){render(box3);}
    for(uint16_t i = 0 ; i < 10 ; i++){render(box2);}
    for(uint16_t i = 0 ; i < 10 ; i++){render(box1);}
    for(uint16_t i = 0 ; i < 50 ; i++){}

    uint8_t sweep[9][8] = {0};
    uint8_t shift = 0;
    while(shift<32){
        for(uint8_t y=0 ; y<9 ; y++){
            for(uint8_t x=shift ; x<(shift+8) ; x++){
                sweep[y][x-shift] = pattern_ZIKI2[y][x];
            }
        }
        for(uint8_t i = 0 ; i < 10 ; i++){render(sweep);}
        shift++;
    }
    for(uint16_t i = 0 ; i < 50 ; i++){}
    for(uint16_t i = 0 ; i < 10 ; i++){render(box1);}
    for(uint16_t i = 0 ; i < 10 ; i++){render(box2);}
    for(uint16_t i = 0 ; i < 10 ; i++){render(box3);}
    for(uint16_t i = 0 ; i < 10 ; i++){render(box4);}
}

bool in_range(int8_t x, int8_t y) {
    return (x>0) && (y>0) && (x<=8) && (y<=9);
}

void zeroin_animation(int8_t x, int8_t y) {
    uint8_t blank[9][8] = {0};
    uint8_t pattern[9][8] = {0};

    pattern[y][x+1] = 1;
    pattern[y][x-1] = 1;
    pattern[y+1][x] = 1;
    pattern[y-1][x] = 1;

    for (uint8_t j = 0; j < 3; j++) {
        for (uint8_t i = 0; i < 7; i++) {
            render(pattern);
        }
        for (uint8_t i = 0; i < 7; i++) {
            render(blank);
        }
    }
}

void setup_leds() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);
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

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
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
