// PB2 - ADC inout from battery
// PB1 - Red LED
// PB3 - Orange LED
// PB4 - Green LED
void setup() {
  digitalWrite(PB1, LOW);
  pinMode(PB1, INPUT);
  digitalWrite(PB3, LOW);
  pinMode(PB3, INPUT);
  digitalWrite(PB4, LOW);
  pinMode(PB4, INPUT);
}

void loop() {
  pinMode(PB1, OUTPUT);   
  delay(2000);            
  pinMode(PB1, INPUT);    
  pinMode(PB3, OUTPUT);   
  delay(2000);            
  pinMode(PB3, INPUT);    
  pinMode(PB4, OUTPUT);   
  delay(2000);             
  pinMode(PB4, INPUT);     
}
