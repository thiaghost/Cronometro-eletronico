
#define  F_CPU 16e6
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t min = 0;
volatile uint8_t seg = 0;
volatile uint16_t mili = 0;

uint8_t display7seg[10] =  {0x7E, 0x30, 0x6D, 0x79, 0x33,
	0x5B, 0x5F, 0x70, 0x7F, 0x7B};

ISR(TIMER0_COMPA_vect) // timer counter
{
	PORTB |= 1<<5;
	mili++;
	if(mili >= 1000)
	{
		mili = 0;
		seg++;
		if (seg >= 60)
		{
			seg = 0;
			min++;
		}
	}
	PORTB &= ~(1<<5);
}

void timer_init()
{
	TCCR0A = (1<<WGM01);	// modo CTC, contagem até OCR0A
	TCCR0B = (1<<CS01) | (1<<CS00); //Pré-scaler DE 64x -> 1 deslocado de CS01 + 1 deslocado de CS00
	TCNT0 = 0;
	OCR0A = 249;	// numero de contagens.
	TIMSK0 = (1<<OCIE0A);
}

void io_init(void)
{
	DDRD = 0xFF;
	DDRB = 0x0F;
	PORTB = 0x30;
}

void display_on(uint8_t d)
{
	uint8_t mask = 1<<d;
	PORTB |= mask;
}

void display_off(uint8_t d)
{
	uint8_t mask = 1<<d;
	PORTB &= ~(mask);
}

int main(void)
{
    io_init();
	timer_init();
    sei();	//Set Enable Interrupt
	
    while (1) 
    {
		display_on(0);
		PORTD = display7seg[min]+0x80;
		_delay_ms(1);
		display_off(0);
		
		display_on(1);
		PORTD = display7seg[seg/10];
		_delay_ms(1);
		display_off(1);
		
		display_on(2);
		PORTD = display7seg[seg%10]+0x80;
		_delay_ms(1);
		display_off(2);
		
		display_on(3);
		PORTD = display7seg[mili/100];
		_delay_ms(1);
		display_off(3);
		
		if(!(PINB & 0x10)){
			 TCCR0B = 0;
		}
			
		if(!(PINB & 0x20)){
			 TCCR0B = (1<<CS01) | (1<<CS00);
		}
    }
}

