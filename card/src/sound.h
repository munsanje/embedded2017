/*
 * sound.h
 *
 *  Created on: 28 Oct 2017
 *      Author: sanje
 */

#ifndef SOUND_H_
#define SOUND_H_

//Includes
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "codec.h"

//Defines
#define LED_BLUE_ON   GPIOD->BSRRL = GPIO_Pin_15;
#define LED_BLUE_OFF  GPIOD->BSRRH = GPIO_Pin_15;

#define NOTEFREQUENCY 0.015		//frequency of saw wave: f0 = 0.5 * NOTEFREQUENCY * 48000 (=sample rate)
#define NOTEAMPLITUDE 500.0		//amplitude of the saw wave


typedef struct {
	float tabs[8];
	float params[8];
	uint8_t currIndex;
} fir_8;


// struct to initialize GPIO pins
GPIO_InitTypeDef GPIO_InitStructure;


void playBuffer(const uint16_t* buffer, uint32_t buffer_size);

void initAudio();

float updateFilter(fir_8* theFilter, float newValue);

void initFilter(fir_8* theFilter);

uint16_t  amplitudeTodB(uint16_t amp);

uint16_t  dBToamplitude(uint16_t  dB);


#endif /* SOUND_H_ */
