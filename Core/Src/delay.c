#include "delay.h"
volatile uint64_t x;


void delayuS(int ms)
{

	SysTick->LOAD=100-1;
	SysTick->VAL=0;
	SysTick->CTRL=0x5;
		for (int i=0;i<ms;i++)
		{
			while(!(SysTick->CTRL &0x10000)){}
		}
	SysTick->CTRL=0;	
	

			 
	}

void delay(int ms)
{

	SysTick->LOAD=100000-1;
	SysTick->VAL=0;
	SysTick->CTRL=0x5;
		for (int i=0;i<ms;i++)
		{
			while(!(SysTick->CTRL &0x10000)){}
		}
	SysTick->CTRL=0;	

}
