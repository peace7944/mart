/*
 * mart.h
 *
 * Created: 2018-11-07 오전 10:45:51
 *  Author: USER
 */ 
#ifndef MART_H_
#define MART_H_

#include <avr/io.h>
#define F_CPU 16000000  // cpu 클럭 설정
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>

#define CLOCK 0
#define MENU 1
#define CALCUL 2
#define CHANGE 3
#define TOTAL 4

#define sw1 30
#define sw2 46
#define sw3 78
#define sw4 142
#define sw5 29
#define sw6 45
#define sw7 77
#define sw8 141
#define sw9 27
#define sw10 43
#define sw11 75
#define sw12 139
#define sw13 23
#define sw14 71
#define sw15 135

unsigned int prekey = 0, msec = 0;

unsigned int my_getkey(){  // flag 처리
	unsigned int key = 0, result = 0;
	for(int i=0;i<4;i++){
		PORTD = ~(0x00 | (1<<i));
		_delay_us(5);
		
		key = (~PIND & 0xf0);
		if(key){
			result = key | (PORTD & 0x0f);
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

/*
unsigned int my_getkey(){
	unsigned int key = 0, result = 0;
	for(int i=0;i<4;i++){
		PORTD = ~(0x00 | (1<<i)); // Row 검출 ex)1111 1110, 1111 1101
		_delay_us(5);
		
		key = (~PIND & 0xf0);  // Col 검출 ex) 0001 0000, 0010 0000
		if(key){
			result = key | (PORTD & 0x0f);  // Col + Row
			return result;
		}
	}
	return 0;
}
*/
void initSystem(){
	
	DDRA = 0xff;  // 0,1 = motor, 5,6,7 = LED
	DDRD = 0x0f;  // 0,1,2,3 = Row, 4,5,6,7 = col
	DDRF = 0x01;  // photo
	
	PORTD = 0xff;
	
	lcd_init();
	lcd_clear();
	
	lcd_putsf(0,0, (unsigned char*)"    MARKET     ");
	lcd_putsf(0,1, (unsigned char*)"    SYSTEM     ");
	PORTA = 0x00;
	_delay_ms(500);
	
	lcd_clear();
	PORTA = 0b11100000;
	_delay_ms(500);
	
	lcd_putsf(0,0, (unsigned char*)"    MARKET     ");
	lcd_putsf(0,1, (unsigned char*)"    SYSTEM     ");
	PORTA = 0x00;
	_delay_ms(500);
	
	lcd_clear();
	PORTA = 0b11100000;
	_delay_ms(500);
}



#endif /* MART_H_ */