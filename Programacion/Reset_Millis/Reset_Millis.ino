extern volatile unsigned long timer0_millis;
unsigned long new_value = 0;

void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.println(millis());
  if(millis()>6000){ 
  //Change Millis
  setMillis(new_value);
  }
}

void setMillis(unsigned long new_millis){
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}
