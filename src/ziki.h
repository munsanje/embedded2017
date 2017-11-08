#include "queue.h"

#ifndef ZIKI_H
#define ZIKI_H

xQueueHandle Q_HANDLE_INPUT_OUTPUT;

/* Handles all tasks related to user-input; for instance
 * reading pots using ADC */
void input_main(void* p);

/* Handles LED display */
void output_main(void* p);

#endif /* ZIKI_H */
