#ifndef ZIKI_H
#define ZIKI_H

/* Handles all tasks related to user-input; for instance
 * reading pots using ADC */
void uinput_main(void* p);

/* Handles LED display */
void output_main(void* p);

/* Synthesizes Audio */
void synth_main(void* p);

#endif /* ZIKI_H */
