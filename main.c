//#include "stdint.h"
#include "stm32f4xx.h"
#include "main.h"
#include "tts_proces.h"
#include "pwm.h"

uint8_t qewq;
extern const SLeter dictionary[];
extern const uint8_t samples[];
extern const uint8_t *digit[];
const uint8_t string[]="ONE";


int main(void)
{
	uint8_t sentence1[]="bluetooth was connected 1 2 3 4 5 6 7 8 9";
	uint8_t sentence2[]="bluetooth disconnected  1 2 3 4 5 6 7 8 9";
	uint8_t sentence3[]="waite charging   1 2 3 4 5 6 7 8 9";

	SystemInit();
	pwm_config();



    while(1)
    {
    	tts_proces_play(sentence1);
    	tts_proces_play(sentence2);
    	tts_proces_play(sentence3);
    }
}
