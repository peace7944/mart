/*
 * mart.cpp
 *
 * Created: 2018-11-07 오전 9:29:44
 * Author : USER
 */ 

#include <avr/io.h>
#define F_CPU 16000000  // cpu 클럭 설정
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"
#include "mart.h"

unsigned int hour = 8, min = 0, sec = 0;
int mode = CLOCK;
char str[16] = {0,};
	
ISR(TIMER0_OVF_vect){
	msec++;
	
	if(hour >= 24) hour = min = sec = msec = 0;
	if(min >= 60){hour++; min = 0;}
	if(sec >= 60){min++; sec = 0;}
	if(msec >= 1000){sec++; msec = 0;}
}

int main(void)
{
	//timer init
	TCNT0 = 6;  // TCNT0가 256이 될때 overflow -> 0  1/16mHz = 0.0000000625, 0.0000000625 * 64 = 0.000004 분주한 속도
	TIMSK = 0x01;  // 0b00000001, timer 0 enable
	TCCR0 = 0b00000100;  // 0b00000100, 분주비 = 64
	sei();  // 숫자를 카운트
	
	initSystem();
	

	
    while (1) 
    {
		if(mode == CLOCK){
			while(1){
				unsigned int key = my_getkey();
				//if(key)
				lcd_putsf(0,0,(unsigned char*)"    WELCOME!!   ");
				
				sprintf(str,"  %d    ",key);
				//sprintf(str,"    %02d:%02d:%02d    ",hour, min, sec);
				lcd_putsf(0,1,(unsigned char*)str);
			}
		}
    }
}

