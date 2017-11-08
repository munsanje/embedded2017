#include "FreeRTOS.h"
#include "task.h"

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "codec.h"
#include "piano.h"

#define DOWNSCALE 8

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

    play(selected);
}

void play(uint8_t pattern[8][8]) {
    uint8_t sampleCounter = 0;
	uint16_t i = 0, j = 0;
	uint16_t sawWave = 0;
	for (;;) {
		if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE)) {
            SPI_I2S_SendData(CODEC_I2S, sawWave);

            // only update on every second sample to insure that L & R ch. have the same sample value
            if (sampleCounter & 0x00000001) {

                if ((i < PIANO_SIZE) && (j < COL_SIZE)) {
                    sawWave = 0;
                    for (uint8_t k = 0; k < COL_SIZE; k++) {
                        sawWave += (piano[k][i]/DOWNSCALE) * pattern[k][j];
                    }

                    i++;
                } else {
                    i = 0;
                    j++;
                    if (j >= COL_SIZE){
                        j=0;
                    }
                }

            }
            sampleCounter++;
        }

	}

    vTaskDelete(NULL);
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
