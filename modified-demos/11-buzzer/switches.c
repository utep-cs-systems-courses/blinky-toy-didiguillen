#include <msp430.h>
#include "libTimer.h"

#define LED_GREEN BIT0
#define LED_RED BIT6
#define LEDS (LED_RED | LED_GREEN)

#define SW1 BIT3 //button on bottom board
#define SW2 BIT0
#define SW3 BIT1
#define SW4 BIT2
#define SW5 BIT3
#define TOP_SWITCH (SW2 | SW3 | SW4 | SW5)
extern int playBH;

void switch_init()
{
  P1REN |= SW1;
  P1IE |= SW1;
  P1OUT |= SW1;
  P1DIR &= ~SW1;
  P2REN |= TOP_SWITCH;
  P2IE |= TOP_SWITCH;
  P2OUT |= TOP_SWITCH;
  P2DIR &= ~TOP_SWITCH;
}

void switch_interrupt_handler()
{
  char p1val = P1IN;
  P1IES |= (p1val & SW1);
  P1IES &= (p1val | ~SW1);

  char p2val = P2IN;
  P2IES |= (p2val & TOP_SWITCH);
  P2IES &= (p2val | ~TOP_SWITCH);

  if(p2val & SW2)
    playBH = 1;
  else
    playBH = 0;
}

void __interrupt_vec(PORT2_VECTOR) Port_2()
{
  if(P2IFG & SW2){
    P2IFG &= ~SW2;
    switch_interrupt_handler();
  }
}
