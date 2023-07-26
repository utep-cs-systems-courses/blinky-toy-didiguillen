#include <msp430.h>
#include "libTimer.h"

#define LED_RED BIT0               // P1.0
#define LED_GREEN BIT6             // P1.6
#define LEDS (LED_RED | LED_GREEN)

#define SW1 BIT3		/* switch1 is p1.3 */
#define SWITCHBOT SW1		/* only 1 switch on this board */
#define SW2 BIT0
#define SWITCHONE SW2
#define SW3 BIT1
#define SWITCHTWO SW3
#define SW4 BIT2
#define SWITCHTHREE SW4
#define SW5 BIT3
#define SWITCHFOUR SW5
#define SWITCHTOP (SWITCHONE | SWITCHTWO | SWITCHTHREE | SWITCHFOUR)

#define BLINK1 0
#define BLINK2 1
#define BLINK3 2
#define BLINK4 3
#define BLINK5 5
void blink_state(int);
void switch_interrupt_handler_2();

void switch_init() {
  P1REN |= SWITCHBOT;		/* enables resistors for switches */
  P1IE |= SWITCHBOT;		/* enable interrupts from switches */
  P1OUT |= SWITCHBOT;		/* pull-ups for switches */
  P1DIR &= ~SWITCHBOT;		/* set switches' bits for input */
  P2REN |= SWITCHTOP;
  P2IE |= SWITCHTOP;
  P2OUT |= SWITCHTOP;
  P2DIR &= ~SWITCHTOP;
}

void led_init() {
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;		/* leds initially off */
}

void wdt_init() {
  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
}

void main(void) 
{  
  switch_init();
  led_init();
  wdt_init();
    
  or_sr(0x18);  // CPU off, GIE on
} 

static int buttonDown;

void
switch_interrupt_handler()
{
  char p1val = P1IN;		/* switch is in P1 */

/* update switch interrupt sense to detect changes from current buttons */
  P1IES |= (p1val & SWITCHBOT);	/* if switch up, sense down */
  P1IES &= (p1val | ~SWITCHBOT);	/* if switch down, sense up */

  if (p1val & SW1) {		/* button up */
    P1OUT &= ~LED_GREEN;
    buttonDown = 0;
  } else {			/* button down */
    P1OUT |= LED_GREEN;
    buttonDown = 1;
  }
}
void switch_interrupt_handler_2(){
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHTOP);
  P2IES &= (p2val | ~SWITCHTOP);

  if(p2val & SW2){
    P1OUT &= ~LED_GREEN;
    buttonDown = 0;
  }
  else {
    P1OUT |= LED_GREEN;
    buttonDown = 1;
  }
  
}

char state = BLINK1;

void blink_state(int num)
{
  switch(num){
  case 1:
    state = BLINK1;
    break;
  case 2:
    state = BLINK2;
    break;
  case 3:
    state = BLINK3;
    break;
  case 4:
    state = BLINK4;
    break;
  default:
    state = BLINK5;
  }
}
/* Switch on P1 (S2) */
void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if (P1IFG & SWITCHBOT) {	      /* did a button cause this interrupt? */
    P1IFG &= ~SWITCHBOT;		      /* clear pending sw interrupts */
    //countpress++;
    blink_state(0);
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHONE){
    P2IFG &= ~SWITCHONE;
    blink_state(1);
    switch_interrupt_handler_2();
  }
  else if (P2IFG & SWITCHTWO){
    P2IFG &= ~SWITCHTWO;
    blink_state(2);
  }
  else if (P2IFG & SWITCHTHREE){
    P2IFG &= ~SWITCHTHREE;
    blink_state(3);
  }
  else if(P2IFG & SWITCHFOUR){
    P2IFG &= ~SWITCHFOUR;
    blink_state(4);
  }
}

void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  static int blink_count = 0;
  if(state == BLINK1){
    switch(blink_count){
    case 6:
      blink_count = 0;
      P1OUT |= LED_RED;
      break;
    default:
      blink_count++;
      if(!buttonDown) P1OUT &= ~LED_RED;
    }
  }
  else if(state == BLINK2){
    switch(blink_count){
    case 12:
      blink_count = 0;
      P1OUT |= LED_RED;
      break;
    default:
      blink_count++;
      if(!buttonDown) P1OUT &= ~LED_RED;
    }
  }
  else if(state == BLINK3){
    switch(blink_count){
    case 18:
      blink_count = 0;
      P1OUT |= LED_RED;
      break;
    default:
      blink_count++;
      if(!buttonDown) P1OUT &= ~LED_RED;
    }
  }
  else if(state == BLINK4){
    switch(blink_count){
    case 24:
      blink_count = 0;
      P1OUT |= LED_RED;
      break;
    default:
      blink_count++;
      if(!buttonDown) P1OUT &= ~LED_RED;
    }
  }
  else{ //doesn't work, ask at coaching session
    switch(blink_count){
    case 30: 
      blink_count = 0;
      P1OUT |= LED_RED;
      break;
    default:
      blink_count++;
      if (!buttonDown) P1OUT &= ~LED_RED; /* don't blink off if button is down */
    }
  }
} 
