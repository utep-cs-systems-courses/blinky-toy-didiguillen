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

#define BLINK1 250
#define BLINK2 125
#define BLINK3 60
#define BLINK4 30
#define BLINK5 15

extern int playBH;
extern int blinks;
extern int playSus;
extern int led;

void blink_state(int);

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

void led_init()
{
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;
}

void switch_interrupt_handler()
{
  char p1val = P1IN;
  P1IES |= (p1val & SW1);
  P1IES &= (p1val | ~SW1);

  char p2val = P2IN;
  P2IES |= (p2val & TOP_SWITCH);
  P2IES &= (p2val | ~TOP_SWITCH);
}

void blink_state(int num)
{
  switch(num){
  case 0:
    blinks = BLINK1;
    break;
  case 1:
    blinks = BLINK2;
    break;
  case 2:
    blinks = BLINK3;
    break;
  case 3:
    blinks = BLINK4;
    break;
  default:
    blinks = BLINK5;
    break;
  }
}

void __interrupt_vec(PORT2_VECTOR) Port_2()
{
  if(P2IFG & SW2){
    P2IFG &= ~SW2;
    blink_state(1);
    playBH = 1;
    led = 0;
    switch_interrupt_handler();
  }
  else if(P2IFG & SW3){
    P2IFG &= ~SW3;
    blink_state(2);
    led = 1;
    switch_interrupt_handler();
  }
  else if(P2IFG & SW4){
    P2IFG &= ~SW4;
    blink_state(3);
    led = 0;
    switch_interrupt_handler();
  }
  else if(P2IFG & SW5){
    P2IFG &= ~SW5;
    blink_state(4);
    playSus = 1;
    led = 1;
    switch_interrupt_handler();
  }
}

void __interrupt_vec(PORT1_VECTOR) Port_1()
{
  if(P1IFG & SW1){
    P1IFG &= ~SW1;
    blink_state(0);
    led = 1;
    switch_interrupt_handler();
  }
}

void toggle_green()
{
  if(P1OUT & LED_GREEN)
    P1OUT &= ~LED_GREEN;
  else
    P1OUT |= LED_GREEN;
}

void toggle_red()
{
  if(P1OUT & LED_RED)
    P1OUT &= ~LED_RED;
  else
    P1OUT |= LED_RED;
}
