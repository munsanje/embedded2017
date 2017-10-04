#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include <math.h>

int output_main(void)
{
    int i = 0;
    uint32_t output_value;
    int32_t count = 0;

    //Input Values:

    uint8_t Y[4][4] = {{1,0,0,1},{1,0,0,1},{0,1,1,0},{0,1,1,0}};
    uint8_t X[4][4] = {{1,0,0,1},{0,1,1,0},{0,1,1,0},{1,0,0,1}};
    uint8_t O[4][4] = {{0,1,1,0},{1,0,0,1},{1,0,0,1},{0,1,1,0}};
    uint8_t U[4][4] = {{1,0,0,1},{1,0,0,1},{1,0,0,1},{0,1,1,0}};

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |
        GPIO_Pin_1 |
        GPIO_Pin_2 |
        GPIO_Pin_3 |
        GPIO_Pin_4 |
        GPIO_Pin_5 |
        GPIO_Pin_6 |
        GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    /* Private functions */
    void delay_ms(uint32_t milli);
    void render(uint8_t w , uint8_t x , uint8_t y, uint8_t z);

    /* Infinite loop */
    while (1)
    {
        /*write a diagonal to board.
          Done by writing to GPIOB_ODR
          */

        /*
        //r1 c1
        output_value = 0b00001001; //first bits are columns, and setting volts, second two bits are ground
        GPIOB->ODR = output_value;
        delay_ms(1000); //delay for 1 second

        //r1 c2
        output_value = 0b00000101;
        GPIOB->ODR = output_value;
        delay_ms(1000); //delay for 1 second

        //r2 c1
        output_value = 0b00001010;
        GPIOB->ODR = output_value;
        delay_ms(1000); //delay for 1 second

        //r2 c2
        output_value = 0b00000110;
        GPIOB->ODR = output_value;
        delay_ms(1000); //delay for 1 second

        //r1 c1 && r1 c2
        output_value = 0b00001101;
        GPIOB->ODR = output_value;
        delay_ms(1000); //delay for 1 second

        //r1 c1 && r2 c1
        output_value = 0b00001000;
        GPIOB->ODR = output_value;
        delay_ms(1000); //delay for 1 second

        for(i=0; i<1000; i++){
        if(i%2==0){
        output_value = 0b00001001; //r1 c1
        GPIOB->ODR = output_value;
        delay_ms(2);
        }
        else{
        output_value = 0b00000110; //r2 c2
        GPIOB->ODR = output_value;
        delay_ms(2);
        }
        }

        for(i=0; i<1000; i++){
        if(i%2==0){
        output_value = 0b00000101; //r1 c2
        GPIOB->ODR = output_value;
        delay_ms(2);
        }
        else{
        output_value = 0b00001010; //r2 c1
        GPIOB->ODR = output_value;
        delay_ms(2);
        }

        }
        */


        //REUSABLE

        uint32_t i;
        //Extract Col Sums
        uint8_t Ycol1=240 , Ocol1=240 , Ucol1=240;
        uint8_t Ycol2=240 , Ocol2=240 , Ucol2=240;
        uint8_t Ycol3=240 , Ocol3=240 , Ucol3=240;
        uint8_t Ycol4=240 , Ocol4=240 , Ucol4=240;
        for(i=0 ; i<4 ; i++){
            Ycol1 += (Y[i][0] * pow(2,(3-i)));
            Ycol2 += (Y[i][1] * pow(2,(3-i)));
            Ycol3 += (Y[i][2] * pow(2,(3-i)));
            Ycol4 += (Y[i][3] * pow(2,(3-i)));

            Ocol1 += (O[i][0] * pow(2,(3-i)));
            Ocol2 += (O[i][1] * pow(2,(3-i)));
            Ocol3 += (O[i][2] * pow(2,(3-i)));
            Ocol4 += (O[i][3] * pow(2,(3-i)));

            Ucol1 += (U[i][0] * pow(2,(3-i)));
            Ucol2 += (U[i][1] * pow(2,(3-i)));
            Ucol3 += (U[i][2] * pow(2,(3-i)));
            Ucol4 += (U[i][3] * pow(2,(3-i)));
        }

        //render the matrix for a predefined period
        uint8_t v1 = Ycol1-240;
        uint8_t v2 = Ycol2-240;
        uint8_t v3 = Ycol3-240;
        uint8_t v4 = Ycol4-240;

        i=0;
        while(i<1000){
            render(Ycol1 , Ycol2 , Ycol3 , Ycol4);
            i++;
        }
        i=0;
        while(i<1000){
            render(Ocol1 , Ocol2 , Ocol3 , Ocol4);
            i++;
        }
        i=0;
        while(i<1000){
            render(Ucol1 , Ucol2 , Ucol3 , Ucol4);
            i++;
        }

        /*
        //XOXOXOXOX
        uint8_t Xcol1=240 , Ocol1=240;
        uint8_t Xcol2=240 , Ocol2=240;
        uint8_t Xcol3=240 , Ocol3=240;
        uint8_t Xcol4=240 , Ocol4=240;

        for(i=0 ; i<4 ; i++){
        Xcol1 += (X[i][0] * pow(2,(3-i)));
        Xcol2 += (X[i][1] * pow(2,(3-i)));
        Xcol3 += (X[i][2] * pow(2,(3-i)));
        Xcol4 += (X[i][3] * pow(2,(3-i)));

        Ocol1 += (O[i][0] * pow(2,(3-i)));
        Ocol2 += (O[i][1] * pow(2,(3-i)));
        Ocol3 += (O[i][2] * pow(2,(3-i)));
        Ocol4 += (O[i][3] * pow(2,(3-i)));
        }

        i=0;
        while(i<1000){
        render(Xcol1 , Xcol2 , Xcol3 , Xcol4);
        i++;
        }
        i=0;
        while(i<1000){
        render(Ocol1 , Ocol2 , Ocol3 , Ocol4);
        i++;
        }

        /*
        //render the matrix
        //Multiplexing
        uint32_t a = 0b10000000;
        uint32_t b = 0b01000000;
        uint32_t c = 0b00100000;
        uint32_t d = 0b00010000;

        uint32_t w = 0b00000000;
        uint32_t x = 0b00000000;
        uint32_t y = 0b00000000;
        uint32_t z = 0b00000000;

        w = w | ~col1;
        x = x | ~col2;
        y = y | ~col3;
        z = z | ~col4;

        //Concatenate
        a = a | w;
        b = b | x;
        c = c | y;
        d = d | z;

        uint32_t j;
        //write to LEDs
        for(j=0; j<4 ; j++){
        if(j==0){
        GPIOA->ODR = a;
        }
        if(j==1){
        GPIOA->ODR = b;
        }
        if(j==2){
        GPIOA->ODR = c;
        }
        if(j==3){
        GPIOA->ODR = d;
        }
        delay_ms(5);
        }
        */

        /*
        // HARD CODED
        int j;
        count=0;

        // Pattern Octagon
        while(count < 1000){
        for(j=0; j<4 ; j++){
        if(j==0){
        output_value = 0b10001001; //first bits are columns, and setting volts, second two bits are ground
        GPIOA->ODR = output_value;
        delay_ms(5);
        }
        if(j==1){
        output_value = 0b01000110;
        GPIOA->ODR = output_value;
        delay_ms(5);
        }
        if(j==2){
        output_value = 0b00100110;
        GPIOA->ODR = output_value;
        delay_ms(5);
        }
        if(j==3){
        output_value = 0b00011001; //first bits are columns, and setting volts, second two bits are ground
        GPIOA->ODR = output_value;
        delay_ms(5);
        }
        }
        count++;
        }
        count=0;
        while(count < 1000){
        for(j=0; j<4 ; j++){
        if(j==0){
        output_value = 0b10000110; //first bits are columns, and setting volts, second two bits are ground
        GPIOA->ODR = output_value;
        delay_ms(1);
        }
        if(j==1){
        output_value = 0b01001001;
        GPIOA->ODR = output_value;
        delay_ms(1);
        }
        if(j==2){
        output_value = 0b00101001;
        GPIOA->ODR = output_value;
        delay_ms(1);
        }
        if(j==3){
        output_value = 0b00010110; //first bits are columns, and setting volts, second two bits are ground
        GPIOA->ODR = output_value;
        delay_ms(1);
        }
        }
        count++;
        }

*/

    } // end while(1)
} // end main()


/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */

/*
   void render(){
   int i,j;
   i = 0;
   j = 0;

   uint32_t a = 0b1000;
   uint32_t b = 0b0100;
   uint32_t c = 0b0010;
   uint32_t d = 0b0001;

   for(i=0 ; i<4 ; i++){
   a = (a << 1) | (i >> X[i][0]);
   b = (b << 1) | (i >> X[i][1]);
   c = (c << 1) | (i >> X[i][2]);
   d = (d << 1) | (i >> X[i][3]);
   }

   for(j=0; j<4 ; j++){
   if(j==0){
   GPIOA->ODR = a;
   }
   if(j==1){
   GPIOA->ODR = b;
   }
   if(j==2){
   GPIOA->ODR = c;
   }
   if(j==3){
   GPIOA->ODR = d;
   }
   }
   }
   */


void render(uint8_t c1 , uint8_t c2 , uint8_t c3, uint8_t c4) {
    uint32_t j;

    // multiplexing
    uint8_t a = 0b10000000;
    uint8_t b = 0b01000000;
    uint8_t c = 0b00100000;
    uint8_t d = 0b00010000;

    // concatenate
    uint8_t t1 = ~c1;
    uint8_t t2 = ~c2;
    uint8_t t3 = ~c3;
    uint8_t t4 = ~c4;

    a = a | ~c1;
    b = b | ~c2;
    c = c | ~c3;
    d = d | ~c4;

    // write to LEDs
    uint32_t cnt = 0;

    while (cnt < 1000) {
        for (j=0; j<4 ; j++) {
            if (j==0) {
                GPIOA->ODR = a;
            }
            if (j==1) {
                GPIOA->ODR = b;
            }
            if (j==2) {
                GPIOA->ODR = c;
            }
            if (j==3) {
                GPIOA->ODR = d;
            }
        }
        cnt++;
    }
}
