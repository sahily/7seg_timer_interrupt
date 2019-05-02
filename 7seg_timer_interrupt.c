/*
 * 7seg_timer_interrupt.c
 *
 * Created: 4/8/2019 1:13:05 PM
 * Author : Sahil S. Mahajan
 * Description: Interrupt based driver for 7-segment display
 *
 */

#include <REG51F.H>

void init(void);
void configureTimer0(void);
void enableInterrupt(void);
void resetTimer0(void);
void display(void);

sbit sl1 = P2^7;
sbit sl2 = P2^6;
sbit sl3 = P2^5;
sbit sl4 = P2^4;

unsigned int numbersToDisplay[4] = {3,6,9,2};   //enter 4 numbers here

unsigned int numberList[10] = {
                                  0xFC,   //0         
                                  0x60,   //1
                                  0xDA,   //2 
                                  0xF2,   //3
                                  0x66,   //4
                                  0xB6,   //5
                                  0xBE,   //6
                                  0xE0,   //7
                                  0xFE,   //8
                                  0xF6    //9
                              };
                            
unsigned int digitSelector[4][4] = {
                                      0,1,1,1,   //select 1st digit on 7seg display
                                      1,0,1,1,   //select 2nd digit on 7seg display
                                      1,1,0,1,   //select 3rd digit on 7seg display
                                      1,1,1,0    //select 4th digit on 7seg display
                                   };                 

unsigned int x;         //used with digitSelctor variable in display function to select digit




void main()
{
    init();
    configureTimer0();
    enableInterrupt();
    while(1);
}


/* -----------------
 * Function: init
 * -----------------
 *
 * clears display, initializes variables, and configures timer mode
 *
 */

void init(void)
{
    P0 = 0x00;      //clear display
    x = 0;
    TMOD = 0x01;    //use Timer 0 in 16-bit Timer operating mode  
}


/* -----------------
 * Function: configureTimer0
 * -----------------
 *
 * sets up Timer 0 and enables timer interrupt
 *
 */

void configureTimer0(void)
{
    resetTimer0();          //load timer0 start value
    TR0 = 1;                //start Timer 0
    ET0 = 1;                //enable Timer 0 overflow interrupt
}


/* -----------------
 * Function: enableInterrupt
 * -----------------
 *
 * enables all interrupts that has their individual interrupt bit enabled
 *
 */

void enableInterrupt(void)
{
    EA = 1;
}


/* -----------------
 * Function: resetTimer0
 * -----------------
 *
 * loads Timer 0's 16 bit count register with start value
 *
 */

void resetTimer0(void)
{
    TH0 = 0xEF;
    TL0 = 0xCD;         //set to 4.5 milliseconds  
}


/* -----------------
 * Function: display
 * -----------------
 *
 * selects one digit on the 7-segment display at a time and displays a number on the selected digit
 *
 */

void display(void)
{

    P0 = 0x00;

    sl1 = digitSelector[x][0];
    sl2 = digitSelector[x][1];
    sl3 = digitSelector[x][2];
    sl4 = digitSelector[x][3];   /* Digit is selected when its pin is reset.        */ 
                                 /* Only one of these is selected in each iteration */

    P0 = numberList[numbersToDisplay[x]];

    x++;

    if(x == 4)
    {
        x = 0;
    }

}



/* --------------------------------------
 * Interrupt Service Routine: Timer 0 Interrupt
 * --------------------------------------
 *
 * Runs each time timer 0 interrupt is generated 
 *
 * resets Timer 0's count register to start value, and calls display function
 *
 */

void isr_t0() interrupt 1
{
    resetTimer0();
    display();
}