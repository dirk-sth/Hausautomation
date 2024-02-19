/*
 * Garage01.c
 *
 * Created: 26.03.2016 19:15:00
 * Author : Dirk Heine
 */ 


#include <avr/io.h>
#define F_CPU 16000000UL     /* Quarz mit 16 Mhz */
#include <util/delay.h>
void init(void);
void auf(void);
void zu(void);
void halt(void);
int richtung;
int stop;
int zaehler;
int leuchtdauer;
uint8_t sample_auf;
uint8_t sample_auf_last;
uint8_t sample_zu;
uint8_t sample_zu_last;

int main(void)
{
    /* Replace with your application code */
	
	init();
	halt();
	
    while (1) 
    {	
				/*	+++ Beispiel für Flankenerkennung ++++
			
					uint8_t  sample;
					uint8_t  sample_Last

					sample = PINC & (1<<PC3);  // Signal einlesen

					if (Sample != 0 && SampleLast == 0    )
					{
					// pos Flanke
					}
					if (Sample == 0 && SampleLast != 0    )
					{
					// neg Flanke
					}
					sampleLast = Sample;
			
					--- Beispiel für Flankenerkennung --- */
	
		// Befehl "auf" über Funk empfangen
		sample_auf = (PIND & (1<<PIND0)); //Signal D0 einlesen
		if (sample_auf == 0 && sample_auf_last != 0) // Negative Flanke
		{
			_delay_ms (10);
			
			if (stop == 1)
			{
				auf(); //auf
			}
			else
			{
				halt();
			}
		}
		sample_auf_last = sample_auf;
		
		
		// Befehl "zu" über Funk empfangen
		sample_zu = (PIND & (1<<PIND1)); //Signal D1 einlesen
		if (sample_zu == 0 && sample_zu_last != 0) // Negative Flanke
		{
			_delay_ms (10);
			if (stop == 1)
			{
				zu(); //zu
			}
			else
			{
				halt();
			}
		}
		
		sample_zu_last = sample_zu;
    }
}



void init(void)
{
	DDRB=0x03;  //PORTB PB0 - PB1 as OUTPUT
	PORTB=0x00; //All pins of PORTB LOW
	
	DDRC=0x3f; //PORTC PC0 - PC5 as OUTPUT
	PORTB=0x00; //All pins of PORTC LOW

	DDRD  = 0x00; /* alle Pins von Port D als Eingang */
	PORTD = 0xff; /* interne Pull-Ups an allen Port-Pins aktivieren */
	
	TCCR1B |= ((1 << CS10) | (1 << CS12)); // Set up timer at Fcpu/1024
	//TCCR1B |= ((1 << CS10) | (1 << CS11)); // Set up timer at Fcpu/64
	//TCCR1B |= (1 << CS12); // Set up timer at Fcpu/256
}


void halt (void)
{
	// Alle Ausgaenge zurücksetzen
	
	PORTB &= ~(1 << DDB0);   // loescht Bit 0 an Port B
	PORTB &= ~(1 << DDB1);   // loescht Bit 1 an Port B
	PORTC &= ~(1 << DDC0);   // loescht Bit 0 an Port C
	PORTC &= ~(1 << DDC1);   // loescht Bit 1 an Port C
	PORTC &= ~(1 << DDC2);   // loescht Bit 2 an Port C
	PORTC &= ~(1 << DDC3);   // loescht Bit 3 an Port C
	PORTC &= ~(1 << DDC4);   // loescht Bit 4 an Port C
	PORTC &= ~(1 << DDC5);   // loescht Bit 5 an Port C
	
	stop = 1;
}


void auf(void)
{
	stop = 0;
	PORTB |= (1 << DDB0);  // setzt Bit 0 an PortB Relais 1 auf
	_delay_ms (60000);
	PORTB &= ~(1 << DDB0); // loescht Bit 0 an Port B
	PORTC |= (1 << DDC0);  // setzt Bit 0 an PortC Relais 3 auf
	_delay_ms (60000);
	PORTC &= ~(1 << DDC0); // loescht Bit 0 an Port C
	PORTC |= (1 << DDC2);  // setzt Bit 2 an PortC Relais 5 auf
	_delay_ms (60000);
	PORTC &= ~(1 << DDC2); // loescht Bit 2 an Port C
	PORTC |= (1 << DDC4);  // setzt Bit 4 an PortC Relais 7 auf
	_delay_ms (60000);
	PORTC &= ~(1 << DDC4); // loescht Bit 4 an Port C
	
	richtung = 2;		  // Rolladen faehrt auf
	stop = 1;
	TCNT1 = 0;
	leuchtdauer = 300;
}


void zu(void)
{
	stop = 0;
	PORTB |= (1 << DDB1);  // setzt Bit 1 an PortB Relais 2 zu
	_delay_ms (60000);
	PORTB &= ~(1 << DDB1); // loescht Bit 1 an Port B
	PORTC |= (1 << DDC1);  // setzt Bit 1 an PortC Relais 4 zu
	_delay_ms (60000);
	PORTC &= ~(1 << DDC1); // loescht Bit 1 an Port C
	PORTC |= (1 << DDC3);  // setzt Bit 3 an PortC Relais 6 zu
	_delay_ms (60000);
	PORTC &= ~(1 << DDC3);   // loescht Bit 3 an Port C
	PORTC |= (1 << DDC5);  // setzt Bit 5 an PortC Relais 8 zu
	_delay_ms (60000);
	PORTC &= ~(1 << DDC5); // loescht Bit 5 an Port C
	
	richtung = 3;		   // Rolladen faehrt zu
	stop = 1;
	TCNT1 = 0;
	leuchtdauer = 300;
}
