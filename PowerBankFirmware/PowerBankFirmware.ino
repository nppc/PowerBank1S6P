#include <avr/sleep.h>

#define LED_RED PB1
#define LED_ORANGE PB3
#define LED_GREEN PB4
#define ADCPIN PB2 //ADC1
#define CHARGESENSEPIN PB0

// Vin = (Vbat*15)/(15+20)
// ADC = Vin * 1024 / Vref
// Vin = (Vref / 1024) * ADC
// 
//                   | discharging | charging
//-------------------+-------------+---------
// 4.2V = 1024 ADC   | GREEN       | GREEN
// 4.1V = 999 ADC    |             |
// 4.0V = 975 ADC    |             | YELLOW
// 3.8V = 926 ADC    |             |
// 3.7V = 902 ADC    | YELLOW      |
// 3.6V = 877 ADC    |             |
// 3.5V = 853 ADC    |             | RED
// 3.4V = 828 ADC    |             |
// 3.3V = 804 ADC    |             |
// 3.2V = 780 ADC    |             |
// 3.1V = 755 ADC    |             |
// 3.0V = 731 ADC    | RED         | RED BLINK
// 2.8V = 682 ADC    | RED BLINK   | 

uint8_t charging = 0; // 0 - not charging; 1 - charging
uint8_t ledmode = 0; // 0 - RED BLINK; 1 - RED; 2 - YELLOW; 3 - GREEN
int oldADCval=0;
uint8_t blink=0;

void setLED(int curADC){
	uint8_t hyst = 2;
	uint8_t newledmode=ledmode;
	int hystval = curADC-oldADCval;
	if(hystval<0) hystval=-hystval;
	if(hystval>hyst){
		oldADCval = curADC;
		if(charging==0){
			// not charging pattern
			if(curADC>902) newledmode=3;
			else if(curADC>731) newledmode=2;
			else if(curADC>682) newledmode=1;
			else newledmode=0;
		} else {
			// charging pattern
			if(curADC>975) newledmode=3;
			else if(curADC>853) newledmode=2;
			else if(curADC>731) newledmode=1;
			else newledmode=0;
		}
		// set the LED
		if(newledmode!=ledmode){
			//DDRB = 0; // turn off all leds
			ledmode = newledmode;
			if(ledmode==3) DDRB = 1<<LED_GREEN;
			else if(ledmode==2) DDRB = 1<<LED_ORANGE;
			else DDRB = 1<<LED_RED; // 1 or 0
		}
		
	}
}

uint16_t ReadADC(){
	uint16_t res32=0;	// accumulator register for ADC readings
	while (bit_is_set(ADCSRA,ADSC)); // wait for any unclompleted ADC conversions
	for(uint16_t i=0;i<5300;i++){ // about 1 sec. In reality will be 32 cycles
		do{
			sei(); // conversion should start automatically after entering sleep mode
			sleep_cpu();
			cli();
			//bitSet (ADCSRA, ADSC); // Start conversion
		} while (bit_is_set(ADCSRA,ADSC));
		//return ADC; // read the value
		uint8_t low  = ADCL; // must read ADCL first
		uint8_t high = ADCH;
		res32+=(uint16_t)((high<<8) | low);
	}
	return res32 /32;
}

ISR(ADC_vect){ // else application is reset
}

void setup() {
  cli(); // no interrupts
  TIMSK = 0; // disable timers interrupts
  TCCR1 = (1<<CS13) | (1<<CS12); // bur tick timer with 2048 prescaller
  GIMSK = 0; // disable pin interrupts
  //noInterrupts();

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
  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADIE); // enable ADC with 16 divider and start first convertion. Interrupt should be enabled to wake up after conversion is completed
  //ADCSRA |= (1<<ADSC); // Start conversion
  bitSet (ADCSRA, ADSC); // Start conversion
  //while (ADCSRA & (1 << ADSC)); // wait for a/d complete
  while (bit_is_set(ADCSRA,ADSC)); // wait for a/d complete
  
  //set_sleep_mode(SLEEP_MODE_ADC);
  set_sleep_mode(SLEEP_MODE_IDLE); // we want timers to run
  sleep_enable(); // set sleep enable bit
}

void loop() {
	uint8_t tmptmr = TCNT1;
	if(ledmode==0){ // only blink in mode 0
		if(tmptmr>128) DDRB = 1<<LED_RED; else DDRB = 0;
	}
	
	uint16_t adcVal = ReadADC();
	setLED(adcVal);
	
	// TODO check charge sensing pin (after schematic is ready)
/*
  bitSet(DDRB, LED_RED);
  ReadADC();
  bitClear(DDRB, LED_RED);
  bitSet(DDRB, LED_ORANGE);
  ReadADC();
  bitClear(DDRB, LED_ORANGE);
  bitSet(DDRB, LED_GREEN);
  ReadADC());
  bitClear(DDRB, LED_GREEN);
*/
}

