#include "FreeRTOS.h"
#include "task.h"

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#include "ff.h"
#include "sound.h"
#include "wav.h"

void card_main(void* p) {
	FATFS FatFs;
	FIL fp;
	int count =0;
	SystemInit();
	FRESULT res = init_sd(&FatFs);  // initialize SD card
	const char* filename = "pG.wav"; //mA.wav";
	const char* files[] = {"pG.wav","mCh.wav"};
	uint16_t bufferP[11025];
	for(int z=0;z<11025;z++){
		bufferP[z]=0;
	}
	uint16_t buffer[11025];
	for(int i=0;i<2;i++){
		res = open_file(&fp, files[i], FA_READ | FA_OPEN_ALWAYS);
			int k = 0; //
		res = read_wav_file(&fp, files[i], buffer, 11025);
		for(int k=0;k<11025;k++){
			bufferP[k]=bufferP[k]+buffer[k];
		}

	}
	deinit_sd();  // de-initialize SD card

	playBuffer(bufferP, 11025);

    for (;;) {
    }
    vTaskDelete(NULL);
}
