/*
 * sound.c
 *
 *  Created on: 28 Oct 2017
 *      Author: sanje
 */


//****************************************************
//
//   EXAMPLE OF USING CS42L22 on STM32F4-Discovery
//
//   author: A.Finkelmeyer
//   http://www.mind-dump.net/configuring-the-stm32f4-discovery-for-audio
//
//****************************************************

#include "sound.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "math.h"


void initAudio() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
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


void playBuffer(const uint16_t* buffer, uint32_t buffer_size) {
	initAudio();
	int i = 0;
	uint32_t sampleCounter = 0;
	uint16_t sample = 0;
	while(i < buffer_size) {
			if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE)) {
		  		SPI_I2S_SendData(CODEC_I2S, sample);
		  		//only update on every second sample to insure that L & R ch. have the same sample value
		  		if (sampleCounter & 0x00000001) {
			  			if (i < buffer_size){
				  				sample = buffer[i];
				  				i++;
			  			} //else
								// i = 0;
		  		}
		  		sampleCounter++;
	  	}

  }
}

/** Adds two wave forms, storing the sum in `waveform_1` */
void addWaveForms(uint16_t* waveform_1, uint16_t* waveform_2, uint32_t length) {
		for(int i = 0; i < length; i++) {
				waveform_1[i] = waveform_1[i] + waveform_2[i] - waveform_1[i]*waveform_2[i]/256;
		}

}

uint16_t amplitudeTodB(uint16_t amp){
	return (uint16_t)(20 * log10(amp));
}

uint16_t dBToamplitude(uint16_t dB){
	return (uint16_t)pow(10, dB/20);
}
