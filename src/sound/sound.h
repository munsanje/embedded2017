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

void initAudio();

void playBuffer(const uint16_t* buffer, uint32_t buffer_size);

void addWaveForms(uint16_t* waveform_1, uint16_t* waveform_2, uint32_t buffer_size);

uint16_t amplitudeTodB(uint16_t amp);

uint16_t dBToamplitude(uint16_t dB);

#endif /* SOUND_H_ */
