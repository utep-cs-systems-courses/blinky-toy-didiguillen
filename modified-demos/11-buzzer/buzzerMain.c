#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"

int main() {
    configureClocks();
 
    buzzer_init();
    //	/* start buzzing!!! 2MHz/1000 = 2kHz*/
    //Buddy Holly notes:
    //9631, 5727, 6428, 7645, 9631, 8581, 7645, 8581, 9631, 11453, 12857  
    enableWDTInterrupts();
    or_sr(0x18);          // CPU off, GIE on
}
int quarterNoteLength = 125;
int eighthNoteLength = 63;
int seconds = 0;
int notes[]= {9361, 5727, 6428, 7645, 9631, 8581, 7645, 8581, 9631, 11453, 12857};
int noteLength[] = {1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1};
int i = 0;

void __interrupt_vec(WDT_VECTOR) WDT()
{
  //if(i = 0){
  //buzzer_set_period(9361);
  //}
  seconds++;
  if(i > 10)
    i = 0;
  if(noteLength[i]){
    if(seconds >= quarterNoteLength){
      seconds = 0;
      i++;
      buzzer_set_period(notes[i]);
    }
  }
  else{
    if(seconds >= eighthNoteLength){
      seconds = 0;
      i++;
      buzzer_set_period(notes[i]);
    }
  }
  
}
