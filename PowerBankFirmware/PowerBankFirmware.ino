#include <avr/sleep.h>

#define LED_RED PB1
#define LED_ORANGE PB3
#define LED_GREEN PB4
#define ADCPIN PB2 //ADC1
#define CHARGESENSEPIN PB0

// ADC = Vin * 1024 / Vref

uint16_t delayReadADC(){
	//uint16_t c = 0;
	while (bit_is_set(ADCSRA,ADSC));
	for(uint16_t i=0;i<2000;i++){ // about 1 sec
    //sleep_enable();
    //sleep_cpu();

		bitSet (ADCSRA, ADSC); // Start conversion
		while (bit_is_set(ADCSRA,ADSC));
		//return ADC; // read the value
		uint8_t low  = ADCL; // must read ADCL first
		uint8_t high = ADCH;
    //c=(uint16_t)((high<<8) | low);
	}
  //return c;
}

void setup() {
  //cli; // no interrupts
  noInterrupts();

  // power reduction
  PRR = (1<<PRTIM1) | (1<<PRTIM0) | (1<<PRUSI);
  ACSR = (1<<ACD); // power down analog comparator

  //initialize pins
  //Float LED pins (OFF)
  DDRB = 0;
  PORTB = 0;
  
  // initialize ADC
  DIDR0 = (1<<ADC1D); // disable digital input logic on ADC1
  ADMUX = (1<<MUX0);	// ADC1 on PB2
  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1)| (1<<ADPS0); // enable ADC with 16 divider and start first convertion
  //ADCSRA |= (1<<ADSC); // Start conversion
  bitSet (ADCSRA, ADSC); // Start conversion
  //while (ADCSRA & (1 << ADSC)); // wait for a/d complete
  while (bit_is_set(ADCSRA,ADSC)); // wait for a/d complete
  
  set_sleep_mode(SLEEP_MODE_ADC);
  //sleep_enable();
  //sleep_cpu();
}

void loop() {
  uint16_t tmp;
  bitSet(DDRB, LED_RED);
  delayReadADC();
  bitClear(DDRB, LED_RED);
  bitSet(DDRB, LED_ORANGE);
  delayReadADC();
  bitClear(DDRB, LED_ORANGE);
  bitSet(DDRB, LED_GREEN);
  delayReadADC();
  bitClear(DDRB, LED_GREEN);
}

