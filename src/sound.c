#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "ziki.h"

#include "codec.h"
#include "piano.h"

void setup_pins();

void play(uint8_t pattern[8][8]);

void sound_main(void* p) {
    setup_pins();

	codec_init();
	codec_ctrl_init();

	I2S_Cmd(CODEC_I2S, ENABLE);

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

    uint8_t selected_pack[8] = {0};
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            selected_pack[i] += selected[i][j] << j;
        }
    }

    uint8_t show[8][8] = {0};

    // unpack
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            show[i][j] = (selected_pack[i] >> j) & 1;
        }
    }

    while(1) {
        play(selected);
    }

    vTaskDelete(NULL);
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
                        wave += (piano[k][i]/8) * pattern[k][j];
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

void setup_pins() {
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
}
