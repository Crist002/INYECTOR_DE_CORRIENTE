#include<Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
//variable LCD
const byte columna = 0;
byte fila;
const byte columna2 = 1;
byte fila2;
String contenido1;
String contenido2;
//Variables pulsadores
boolean est_start;
boolean est_reset;
boolean est_stop;
//RESET DE MILLIS
extern volatile unsigned long timer0_millis;
unsigned long new_value = 0;

void setup() {
  lcd.init();                  
  lcd.backlight();
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  contenido1= "INICIANDO";
  contenido2= "CRONOMETRO";
  Visual();
  delay(3000);
  contenido1= "PRESIONE START";
  contenido2= "";
  Visual();
}

void loop() {
  pulsadores();
  if(est_start == 1){
    start();
  }
}

void pulsadores(){
  est_start = digitalRead(6);
  est_stop = digitalRead(7);
  est_reset = digitalRead(8);
}

void menu(){
    
}

void Visual(){    //Configuro lo que se visualizara en el LCD
  lcd.clear();
  lcd.setCursor(fila,columna);
  lcd.print(contenido1);
  lcd.setCursor(fila2,columna2);
  lcd.print(contenido2);
}

void Visual2(){    //Configuro lo que se visualizara en el LCD
  lcd.setCursor(fila,columna);
  lcd.print(contenido1);
  lcd.setCursor(fila2,columna2);
  lcd.print(contenido2);
}

void start(){
    setMillis(new_value);
    byte val = 1;
    contenido1 = "Minutos: ";
    contenido2 = "Segundos: ";
    int v_segundo;
    int v_minuto;
    Visual();
    while(val == 1){
      unsigned long tiempo = millis();
      int segundo = tiempo/1000;
      int minuto = segundo/60;
      int hora = minuto/60;
      fila = 9;
      v_minuto = minuto -(60*hora);
      if(v_minuto<10){
          contenido1= "0" + String(v_minuto);
      }else{
          contenido1 = v_minuto;
      }
      fila2 = 10;
      v_segundo = segundo -(60*minuto);
      if(v_segundo<10){
          contenido2= "0" + String(v_segundo);
      }else{
          contenido2 = v_segundo;
      }
      Visual2();
      pulsadores();
      if(est_stop == 1){
        val = 0;
      }
    }
}


void setMillis(unsigned long new_millis){
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}
