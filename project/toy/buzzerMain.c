#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"
int blinks = 0;
int led = 0;

int main() {
    configureClocks();
 
    buzzer_init();
    switch_init();
    led_init();
    //	/* start buzzing!!! 2MHz/1000 = 2kHz*/
    //Buddy Holly notes:
    //9631, 5727, 6428, 7645, 9631, 8581, 7645, 8581, 9631, 11453, 12857  
    enableWDTInterrupts();
    or_sr(0x18);          // CPU off, GIE on
}
int halfNoteLength = 200;
int quarterNoteLength = 125;
int eighthNoteLength = 63;
int seconds = 0;
int notes[]= {9361, 5727, 6428, 7645, 9631, 8581, 7645, 8581, 9631, 11453, 12857};
int noteLength[] = {1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1};
int i = 0;
int playBH = 0;
int led_seconds = 0;
int playSus = 0;
int sus_notes[] = {15288, 32394};
int sus_seconds = 0;

void __interrupt_vec(WDT_VECTOR) WDT()
{
  //if(i = 0){
  //buzzer_set_period(9361);
  //}
  if(playBH)
    play_buddyHolly();
  else if (playSus)
    play_sus();
  else
    buzzer_set_period(0);
  
  led_seconds++;
  if(led_seconds >= blinks){
    led_seconds = 0;
    if(led)
      toggle_green();
    else
      toggle_red();
  }
  
  
}

void play_buddyHolly()
{
  seconds++;
  if(i > 10){
    i = 0;
    playBH = 0;
  }
  if(noteLength[i]){
    if(!i)
      buzzer_set_period(notes[0]);
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

int j = 0;
void play_sus()
{
  sus_seconds++;
  if(j > 1){
    j = 0;
    playSus = 0;
  }
  if(!j)
    buzzer_set_period(sus_notes[0]);
  if(sus_seconds >= halfNoteLength){
    sus_seconds = 0;
    j++;
    buzzer_set_period(sus_notes[j]);
  }
  else if(sus_seconds >= eighthNoteLength)
    buzzer_set_period(sus_notes[j]+1000);
}
