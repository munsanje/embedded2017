#include "FreeRTOS.h"
#include "task.h"

#include "codec.h"
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#include "ff.h"

#include "wav.h"
#include "sound.h"


static uint32_t BUFFER_SIZE = 11025;

static uint16_t buffer[11025] = {0};

static uint16_t bufferP[11025] = {0};
//
static uint8_t selected[8][8] = {
		{1,0,0,0,0,0,0,0},
		{1,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,0,0},
		{0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0},
		{0,0,0,0,0,1,0,0},
		{0,1,0,0,0,0,1,0},
		{0,0,0,1,0,0,0,1},
};

static uint8_t selected1[8][8] = {
		{1,0,0,0,0,0,0,0},
		{0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,0,0},
		{1,0,0,1,0,0,1,0},
		{0,0,0,0,1,0,0,0},
		{0,1,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0},
		{0,0,0,0,0,0,0,1},
};
static uint8_t selected2[8][8] = {
		{0,0,0,1,0,0,0,0},
		{0,1,0,0,0,0,0,0},
		{0,0,1,0,0,0,0,0},
		{0,0,0,1,0,0,0,0},
		{0,1,0,0,1,0,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0},
		{0,0,0,0,0,0,0,1},
};
static uint8_t selectedInstr[8][8] = {
		{1,0,0,0,0,0,0,0},
		{0,2,0,0,0,0,0,0},
		{0,0,3,0,0,0,0,0},
		{2,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0},
		{0,0,0,0,0,3,0,0},
		{0,0,0,0,0,0,2,0},
		{0,0,0,0,0,0,0,1}
};

static char* files[3][8]= {{"mA.wav", "mCh.wav", "mCl.wav", "mDh.wav", "mDl.wav", "mFh.wav", "mFl.wav", "mG.wav"},
							 {"pA.wav", "pB.wav", "pC.wav", "pD.wav","pE.wav", "pF.wav", "pG.wav", "pG#.wav"},
						   {"xA.wav", "xB.wav", "xC.wav", "xD.wav", "xE.wav", "xG.wav", "xG#.wav"}};

void card_main(void* p) {
	  uint8_t selectedInstrument = 0;
		static FATFS FatFs;
		static FIL fp;
		uint8_t i, j;
		FRESULT res = 0;
		read_wav_file("mA.wav", buffer, BUFFER_SIZE);
		read_wav_file("mCh.wav", bufferP, BUFFER_SIZE);
		addWaveForms(bufferP, buffer, BUFFER_SIZE);
		playBuffer(bufferP, BUFFER_SIZE);
		// for(i = 0; i < 8; i++){
		// 		for(j = 0; j < 8; j++){
    //
		// 				if(selected[i][j]==1){
		// 					res = read_wav_file(files[1][j], buffer, BUFFER_SIZE);
		// 					if(res == FR_OK){
		// 							for(int m=0;m<BUFFER_SIZE;m++){
		// 								bufferP[m]= bufferP[m]+ buffer[m]/10;
		// 							}
		// 					}
		// 				}
						// if(selected1[i][j]==1){
						// 	res = read_wav_file(files[1][j], buffer, BUFFER_SIZE);
						// 	if(res == FR_OK){
						// 	for(int m=0;m<BUFFER_SIZE;m++){
						// 		bufferP[m]= bufferP[m]+ buffer[m]/32;
						// 	}
						// }
            //
						// }
						// if(selected2[i][j]==1){
						// 	res = read_wav_file(files[2][j], buffer, BUFFER_SIZE);
						// 	if(res == FR_OK){
						// 	for(int m=0;m<BUFFER_SIZE;m++){
						// 		bufferP[m]= bufferP[m]+ buffer[m]/32;
						// 	}
						// }

						// }



		// 		}
		// 		if(res == FR_OK)
		// 			playBuffer(bufferP, BUFFER_SIZE);
		// 		for(int k = 0; k < BUFFER_SIZE; k++) {
		// 			bufferP[k] = 0;
		// 			buffer[k]=0;
		// 		}
	  // }
    for (;;) {
    }
    vTaskDelete(NULL);

}
