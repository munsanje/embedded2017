#include "FreeRTOS.h"
#include "task.h"

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#include "ff.h"
#include "sound.h"
#include "wav.h"

#include "buttons.h"


static uint16_t buffer[11025];
uint16_t bufferP[11025];
static uint16_t buffer_size = 11025;
char* files[3][8]= {{"mA.wav", "mCh.wav", "mCl.wav", "mDh.wav", "mDl.wav", "mFh.wav", "mFl.wav", "mG.wav"},
							 {"pA.wav", "pB.wav", "pC.wav", "pD.wav","pE.wav", "pF.wav", "pG.wav", "pG#.wav"}
						   {"xA.wav", "xB.wav", "xC.wav", "xD.wav", "xE.wav", "xG.wav", "xG#.wav"}};

uint8_t instrument=1;

void card_main(void* p) {

	button_type button;
	PBInit("C0", &button);

	static FATFS FatFs;
	static FIL fp;
	FRESULT res = init_sd(&FatFs);  // initialize SD card
	const char* filename = "mA.wav";

	for(int z=0;z<11025;z++){
		bufferP[z]=0;
	}

  res = read_wav_file(&fp, filename, buffer, buffer_size);  // load in PCM data

	while(PBGetState(&button) == 1);  // wait for button to be pressed

	if(res == FR_OK) {
		playBuffer(buffer, buffer_size);
	}
	deinit_sd();  // de-initialize SD card

    for (;;) {
    }
    vTaskDelete(NULL);
}
