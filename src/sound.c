#include "FreeRTOS.h"
#include "task.h"

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "codec.h"
#include "piano.h"

void setup_pins();

#define COL_SIZE 4
#define PIANO_SIZE 11025


void sound_main(void* p) {
    setup_pins();

	codec_init();
	codec_ctrl_init();

	I2S_Cmd(CODEC_I2S, ENABLE);

    uint16_t selection[4][4] = {
       {1,0,0,0},
       {0,1,0,0},
       {0,0,1,0},
       {0,0,0,1}
    };

    uint8_t downscale = 4;

    uint8_t sampleCounter = 0;
	uint16_t i = 0, j = 0;
	uint16_t sawWave = 0;
	for (;;) {
		if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE)) {
            SPI_I2S_SendData(CODEC_I2S, sawWave);

            // only update on every second sample to insure that L & R ch. have the same sample value
            if (sampleCounter & 0x00000001) {

                if ((i < PIANO_SIZE) && (j < COL_SIZE)) {
                    sawWave = (pA[i]/downscale) * selection[0][j]
                            + (pB[i]/downscale) * selection[1][j]
                            + (pC[i]/downscale) * selection[2][j]
                            + (pD[i]/downscale) * selection[3][j];
                    i++;
                } else {
                    i = 0;
                    j++;
                    if (j >= 4){
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
