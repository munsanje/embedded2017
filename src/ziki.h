#ifndef ZIKI_H
#define ZIKI_H

/* Handles all tasks related to user-input; for instance
 * reading pots using ADC */
void input_main(void* p);

/* Handles LED display */
void visual_main(void* p);

/* Handles Sound output */
void sound_main(void* p);

/* Synthesizes Audio */
void synth_main(void* p);

#endif /* ZIKI_H */
