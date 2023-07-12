//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);			/* CPU off, GIE on */
}

// global state vars that control blinking
int greenBlinkLimit = 0;  // duty cycle = 1/blinkLimit
int greenBlinkCount = 5;  // cycles 0...blinkLimit-1
int redBlinkLimit = 5;
int redBlinkCount = 0;
int greenSecondCount = 0;  // state var representing repeating time 0…1s
int redSecondCount = 0;

void green_blink(void){
  greenBlinkCount--;
  if(greenBlinkCount <= greenBlinkLimit){
    greenBlinkCount = 5;
    P1OUT |= LED_GREEN;
  } else
    P1OUT &= ~LED_GREEN;
}

void red_blink(void){
  redBlinkCount++;
  if(redBlinkCount >= redBlinkLimit){
    redBlinkCount = 0;
    P1OUT |= LED_RED;
  } else
    P1OUT &= ~LED_RED;
}

void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  // handle blinking 
  
  red_blink();
  green_blink();

  // measure a second
  greenSecondCount++;
  redSecondCount++;
  if (redSecondCount >= 125) {
    redSecondCount = 0;
    redBlinkLimit++;
    if(redBlinkLimit >= 8)
      redBlinkLimit = 0;
  }
  if (greenSecondCount >= 250) {  // once each second
    greenSecondCount = 0;
    greenBlinkLimit--;	     // reduce duty cycle
    if (greenBlinkLimit <= -8)     // but don't let duty cycle go below 1/7.
      greenBlinkLimit = 0;
  }
} 

