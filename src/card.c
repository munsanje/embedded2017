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
    uint8_t i, j;
    FRESULT res = 0;
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            if(selected[i][j]==1){
                res = read_wav_file(files[1][j], buffer, BUFFER_SIZE);
                playBuffer(buffer, BUFFER_SIZE);
            }
        }
    }
    for (;;) {
    }
    vTaskDelete(NULL);
}
