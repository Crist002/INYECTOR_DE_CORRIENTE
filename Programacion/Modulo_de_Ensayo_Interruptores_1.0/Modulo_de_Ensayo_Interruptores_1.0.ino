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
//Variables del sensor
float val_max=0;
float val_comp = 0.1;
//Variables interruptores ,rele y auxiliar
byte bot_start = 2;
byte bot_reset = 3;
byte bot_corriente = 4;
byte bot_tension = 5;
byte rele = 8;
byte bot_auxiliar = 10;
boolean est_start = 0;
boolean est_reset = 0;
boolean est_corriente = 0;
boolean est_tension = 0;
boolean est_auxiliar = 0;
//RESET DE MILLIS
extern volatile unsigned long timer0_millis;
unsigned long new_value = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  lcd.init();                  
  lcd.backlight();
  pinMode(bot_start,     INPUT);
  pinMode(bot_reset,     INPUT);
  pinMode(bot_corriente, INPUT);
  pinMode(bot_tension,   INPUT);
  pinMode(bot_auxiliar,  INPUT);
  pinMode(rele,     OUTPUT);
  digitalWrite(rele, LOW);
  contenido1= "Iniciando";
  contenido2= "Programa";
  Visual();
  delay(2000);
  contenido1= "";
  contenido2="00:00:00,000";
  Visual();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  lectura_botones();
  if(est_corriente == 1){
    fila = 0;
    contenido1= "Corriente: 0.00A";
    Visual2();
  }
  if(est_tension == 1){
    switch(est_auxiliar){
      case 0:
        fila = 0;
        contenido1= "AUX: ABIERTO    ";
        Visual2();
      break;
      case 1:
        fila = 0;
        contenido1= "AUX: CERRADO    ";
        Visual2();
      break;
    }
  }
  if(est_start == 1){
    cronometro();
  }
  if(est_reset == 1){
    reset();  
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void lectura_botones(){
  est_start = digitalRead(bot_start);
  est_reset = digitalRead(bot_reset);
  est_corriente = digitalRead(bot_corriente);
  est_tension = digitalRead(bot_tension);
  est_auxiliar = digitalRead(bot_auxiliar);
  
}

void cronometro(){
    setMillis(new_value);
    byte val = 1;
    fila2=0;
    contenido2 = "  :  :  ,";
    int v_segundo;
    int v_minuto;
    int v_ms;
    Visual();
    digitalWrite(rele, HIGH);
    while(val == 1){
      unsigned long tiempo = millis();
      int segundo = tiempo/1000;
      int minuto = segundo/60;
      int hora = minuto/60;
      fila2 = 0;
      if(hora<10){
          contenido2= "0" + String(hora);
      }else{
          contenido2 = hora;
      }
      Visual2();
      fila2 = 3;
      v_minuto = minuto -(60*hora);
      if(v_minuto<10){
          contenido2= "0" + String(v_minuto);
      }else{
          contenido2 = v_minuto;
      }
      Visual2();
      fila2 = 6;
      v_segundo = segundo -(60*minuto);
      if(v_segundo<10){
          contenido2= "0" + String(v_segundo);
      }else{
          contenido2 = v_segundo;
      }
      Visual2();
      fila2 = 9;
      v_ms = tiempo -(1000*60*v_minuto) -(1000*v_segundo);
      if(v_ms<10){
          contenido2= "00" + String(v_ms);
      }else if(v_ms<100){
          contenido2= "0" + String(v_ms);
      }else{
          contenido2 = v_ms;
      }
      Visual2();
      if(est_corriente == 1){
        corriente();
      }
      lectura_botones();
      if(est_tension == 1){
    switch(est_auxiliar){
      case 0:
        fila = 0;
        contenido1= "AUX: ABIERTO    ";
        Visual2();
      break;
      case 1:
        fila = 0;
        contenido1= "AUX: CERRADO    ";
        Visual2();
      break;
    }
  }
      if(tiempo > 250){
        if(est_tension == 1){
          if(est_auxiliar == 0){
            digitalWrite(rele, LOW);
            val = 0;
          }
        }
      }
      if(tiempo > 250){
        if(est_corriente == 1){
          if(val_max < val_comp){
            digitalWrite(rele, LOW);
            val = 0;
            fila= 11;
            contenido1= "0.00";
            Visual2();
          }
        }
      }
        if(est_reset == 1){  //Si Reset es presionado
          digitalWrite(rele, LOW);
          new_value = tiempo;
          val = 0; //Para salir del bucle while
        }
    val_max = 0;
    }
}

void corriente(){
  for(int i = 0; i<600 ; i++){
    float valor = analogRead(A2);
    float tension = (valor*5)/1023;
    val_max = tension +val_max;
  }
  val_max = (val_max/600)*1.5707963;
  fila = 11;
  contenido1 = val_max;
  Visual2();
  
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

void reset(){
  new_value = 0;
  fila2= 0;
  contenido2 = "00:00:00,000";
  Visual2();
}

void setMillis(unsigned long new_millis){
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}
