#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"

int main() {
    configureClocks();
 
    buzzer_init();
    buzzer_set_period(8581);	/* start buzzing!!! 2MHz/1000 = 2kHz*/
    //Buddy Holly notes:
    //9631, 5727, 6428, 7645, 9631, 8581, 7645, 8581, 9631, 11453, 12857  

    or_sr(0x18);          // CPU off, GIE on
}
