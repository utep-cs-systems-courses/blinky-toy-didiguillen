//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT |= LED_RED | LED_GREEN;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);		/* CPU off, GIE on */
}

int secondCount = 0;
int count = 0;

void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  if((++secondCount) == 4){ //made it blink so fast it looks dimmer
    secondCount = 0;
    if((++count) == 2){
      count = 0;
      P1OUT |= LED_GREEN;
      P1OUT &= ~LED_RED;
    }
    else{
      P1OUT &= ~LED_GREEN;
      P1OUT |= LED_RED;
    }
  }
} 

