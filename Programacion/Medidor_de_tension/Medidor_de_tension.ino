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


void setup() {
  lcd.init();                  
  lcd.backlight();
  contenido1= "Iniciando";
  contenido2= "Programa";
  Visual();
  delay(2000);
  contenido1= "Corriente:     A";
  contenido2="";
  Visual();
}

void loop() {
  for(int i = 0; i<1000 ; i++){
    float valor = analogRead(A2);
    float tension = (valor*5)/1023;
    val_max = tension +val_max;
  }
  val_max = (val_max/1000)*1.5707963;
  fila = 11;
  contenido1 = val_max;
  Visual2();
  val_max = 0;
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
