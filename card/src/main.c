/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 8.0.0   2017-10-25

The MIT License (MIT)
Copyright (c) 2009-2017 Atollic AB

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <stdio.h>
#include <string.h>

#include "ff.h"
#include "sound.h"
#include "wav.h"

/* Private macro */
#define LED_ORANGE LED3
#define LED_GREEN LED4
#define LED_RED LED5
#define LED_BLUE LED6


/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
FATFS FatFs;
FIL fp;
int main(void)
{
//
	SystemInit();
	STM_EVAL_LEDInit(LED_GREEN);
	STM_EVAL_LEDInit(LED_RED);
	STM_EVAL_LEDInit(LED_BLUE);
	STM_EVAL_LEDInit(LED_ORANGE);

	FRESULT res = init_sd(&FatFs);
	const char* filename = "cartoon012.wav";
	uint32_t buffer_size = get_wav_size(&fp, filename);
	uint16_t buffer[buffer_size];
    res = read_wav_file(&fp, filename, buffer, buffer_size);  // load in PCM data
	if(res == FR_OK) {
		STM_EVAL_LEDOn(LED_GREEN);
		playBuffer(buffer, buffer_size);
		STM_EVAL_LEDOff(LED_GREEN);
	}
	deinit_sd();
    STM_EVAL_LEDOn(LED_RED);

	/* Infinite loop */
	while (1) {}
}

/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
