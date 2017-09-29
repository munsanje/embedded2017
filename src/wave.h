/*
 * wave.c
 *
 *  Created on: Sep 29, 2017
 *      Author: Munsanje Mweene
 */

#ifndef _WAVE_H_
#define _WAVE_H_

#include "stdio.h"
#include "stdint.h"

typedef struct WAVE_HEADER {

  // RIFF Chunk Descriptor
  uint8_t riff[4];
  uint32_t chunk_size;
  uint8_t wave[4];

  // fmt sub-chunk
  uint8_t fmt[4];
  uint32_t sub_chunk1_size;
  uint16_t audio_format;
  uint16_t num_channels;
  uint32_t sample_rate;
  uint32_t byte_rate;
  uint16_t block_align;
  uint16_t bits_per_sample;

  // data sub-chunk
  uint8_t sub_chunk2_id[4];
  uint32_t sub_chunk2_size;

} wave_header;

typedef struct WAVE_FILE {
	wave_header header;
	uint8_t *audio_data;  // pointer to start of audio data
} wave_file;

/*Read in header from specified wave file*/
int read_header(wave_header*, FILE*);


/*Read specified wave file*/
wave_file read_wav(const char*);









#endif
