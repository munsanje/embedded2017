#include "FreeRTOS.h"
#include "task.h"

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "codec.h"
#include "pA.h"


typedef struct {
	float tabs[8];
	float params[8];
	uint8_t currIndex;
} fir_8;

volatile uint32_t sampleCounter = 0;
volatile int16_t sample = 0;

float sawWave = 0.0;
float filteredSaw = 0.0;

void initFilter(fir_8* theFilter);

void setup_pins();

void sound_main(void* p) {
    setup_pins();

	codec_init();
	codec_ctrl_init();

	I2S_Cmd(CODEC_I2S, ENABLE);

    fir_8 filt;
	initFilter(&filt);


    for (int i = 0;;) {
        if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE)) {
            SPI_I2S_SendData(CODEC_I2S, sample);

                // only update on every second sample to insure that L & R ch. have the same sample value
                if (sampleCounter & 0x00000001)
                {
                    if (i<35564){
                        sawWave= gen[i];
                        i++;
                    } else {
                        i = 0;
                    }
                    sample = (sawWave);
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

void initFilter(fir_8* theFilter)
{
	uint8_t i;

	theFilter->currIndex = 0;

	for (i = 0; i < 8; i++) {
		theFilter->tabs[i] = 0.0;
    }

	theFilter->params[0] = 0.01;
	theFilter->params[1] = 0.05;
	theFilter->params[2] = 0.12;
	theFilter->params[3] = 0.32;
	theFilter->params[4] = 0.32;
	theFilter->params[5] = 0.12;
	theFilter->params[6] = 0.05;
	theFilter->params[7] = 0.01;
}
