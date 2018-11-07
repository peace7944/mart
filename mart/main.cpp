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

unsigned int prekey = 0, msec = 0;

ISR(TIMER0_OVF_vect){
	msec++;
}

unsigned int my_getkey(){
	unsigned int key = 0, result = 0;
	for(int i=0;i<3;i++){
		if(i==0) PORTE = 0b11111110;
		if(i==1) PORTE = 0b11111101;
		if(i==2) PORTE = 0b11111011;
		
		_delay_us(5);
		
		key = (~PINE & 0xf0);
		if(key){
			result = key | (PORTE & 0x0f);
			if(result != prekey){
				prekey = result;
				return result;
			}
			else return 0;
		}
	}
	prekey = 0;
	return 0;
}


int main(void)
{
	//timer init
	TCNT0 = 6;  // TCNT0가 256이 될때 overflow -> 0  1/16mHz = 0.0000000625, 0.0000000625 * 64 = 0.000004 분주한 속도
	TIMSK = 0x01;  // 0b00000001, timer 0 enable
	TCCR0 = 0b00000100;  // 0b00000100, 분주비 = 64
	sei();  // 숫자를 카운트	
	
    while (1) 
    {
		
    }
}

