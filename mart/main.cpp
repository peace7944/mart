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

unsigned int hour = 8, min = 0, sec = 0, end_time = 0;
int mode = CLOCK;
bool error_flag = false, change_flag = false, total_flag = true, endtime_flag = false, end_flag = false;
char str[16] = {0,};
long int sum = 0;
	
ISR(TIMER0_OVF_vect){
	msec++;
	if(endtime_flag) end_time++;
	
	if(hour >= 24) hour = min = sec = msec = 0;
	if(min >= 60){hour++; min = 0;}
	if(sec >= 60){min++; sec = 0;}
	if(msec >= 1000){sec++; msec = 0;}
		
	if(end_time >= 5000){
		end_flag = true;
		endtime_flag = false;
		end_time = 0;
	}
}

void key_input(unsigned int key){
	if(key == sw1) sum = sum * 10 + 1;
	if(key == sw2) sum = sum * 10 + 2;
	if(key == sw3) sum = sum * 10 + 3;
	if(key == sw5) sum = sum * 10 + 4;
	if(key == sw6) sum = sum * 10 + 5;
	if(key == sw7) sum = sum * 10 + 6;
	if(key == sw9) sum = sum * 10 + 7;
	if(key == sw10) sum = sum * 10 + 8;
	if(key == sw11) sum = sum * 10 + 9;
	if(key == sw13) sum = sum * 10 + 0;	
}

int main(void)
{
	//timer init
	TCNT0 = 6;  // TCNT0가 256이 될때 overflow -> 0  1/16mHz = 0.0000000625, 0.0000000625 * 64 = 0.000004 분주한 속도
	TIMSK = 0x01;  // 0b00000001, timer 0 enable
	TCCR0 = 0b00000100;  // 0b00000100, 분주비 = 64
	sei();  // 숫자를 카운트
	
	initSystem();
	
	long int total_sale = 0, total_sum = 0;
    while (1) 
    {
		if(mode == CLOCK){
			while(1){
				unsigned int key = my_getkey();
				if(key == sw12){
					mode = MENU;
					lcd_clear();
					break;
				}
				
				if(PINF == 0x01){
					mode = CALCUL;
					lcd_clear();
					break;					
				}
				
				lcd_putsf(0,0,(unsigned char*)"    WELCOME!!   ");
				sprintf(str,"    %02d:%02d:%02d    ",hour, min, sec);
				lcd_putsf(0,1,(unsigned char*)str);
			}
		}
		
		else if(mode == MENU){
			while(1){
				unsigned int key = my_getkey();
				
				if(key == sw1){
					mode = CALCUL;
					lcd_clear();
					break;
				}
				
				if(key == sw2){
					mode = TOTAL;
					lcd_clear();
					break;
				}
				
				lcd_putsf(0,0,(unsigned char*)"1: Calculation  ");
				lcd_putsf(0,1,(unsigned char*)"2: Total Sales  ");
			}
		}
		
		else if(mode == CALCUL){
			while(1){
				unsigned int key = my_getkey();
				
				lcd_putsf(0,0,(unsigned char*)"Calculate Mode  ");
				
				key_input(key);
					
				if(key == sw4){ // +
					if(sum > 200000) error_flag = true;
					else{
						total_sum += sum;
						sum = 0;
						lcd_clear();
					}
				}
				
				if(key == sw8){
					if(total_sum > 200000) error_flag = true;
					else{
						total_sum += sum;
						sum = 0;
						lcd_clear();
						change_flag = true;
						total_flag = false;
					}
				}
				
				if(change_flag && key == sw14){
					mode = CHANGE;
					lcd_clear();
					total_flag = true;
					change_flag = false;
					break;
				}

				if(error_flag){
					for(int i=0;i<3;i++){
						sprintf(str,"     ERROR!     ");
						lcd_putsf(0,1,(unsigned char*)str);
						_delay_ms(100);
						lcd_clear();
						_delay_ms(100);
					}
					total_sum = 0;
					sum = 0;
					error_flag = false;
					mode = CLOCK;
					break;
				}

				if(total_flag) sprintf(str,"    %ld",sum);
				else sprintf(str,"= %ld won",total_sum);
				lcd_putsf(0,1,(unsigned char*)str);
			}
		}

		else if(mode == CHANGE){
			while(1){
				unsigned int key = my_getkey();
				lcd_putsf(0,0,(unsigned char*)"Changes Mode    ");
				key_input(key);

				if(key == sw8){
					if(total_sum > sum) error_flag = true;
					else{
						total_sale += total_sum;
						sum = sum - total_sum;
						total_sum = 0;
						endtime_flag = true;
						lcd_clear();
					}
				}
				
				if(error_flag){
					for(int i=0;i<3;i++){
						sprintf(str,"     ERROR!     ");
						lcd_putsf(0,1,(unsigned char*)str);
						_delay_ms(100);
						lcd_clear();
						_delay_ms(100);
					}
					sum = 0;
					error_flag = false;
				}				
				
				if(key == sw15 || end_flag){
					mode = CLOCK;
					end_time = 0;
					sum = 0;
					endtime_flag = false;
					end_flag = false;
					break;
				}
				
				sprintf(str,"    %ld",sum);
				lcd_putsf(0,1,(unsigned char*)str);
				
			}
		}

		else if(mode == TOTAL){
			endtime_flag = true;
			lcd_clear();
			while(1){
				unsigned int key = my_getkey();
				
				if(key == sw15 || end_flag){
					mode = CLOCK;
					end_time = 0;
					endtime_flag = end_flag = false;
					break;
				}
				
				lcd_putsf(0,0,(unsigned char*)"Total Sales     ");
				sprintf(str,"= %ld won",total_sale);
				lcd_putsf(0,1,(unsigned char*)str);
			}
		}		
    }
}

