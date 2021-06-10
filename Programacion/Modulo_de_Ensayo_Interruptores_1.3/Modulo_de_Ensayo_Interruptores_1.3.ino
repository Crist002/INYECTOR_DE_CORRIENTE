//////////////////////////// PROYECTO SOL TECNICA /////////////////////////
#include <EEPROM.h>
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
int cont = 0;
float v_fs = 0;
float Valor_sumado;
int vuelta_prom;
float Val_prom;
int vuelta;
float val_max=0;
float val_comp = 0.1;
float relacion=1;
int digito1=0;
int digito2=0;
int digito3=0;
int digito4=0;
int relacion1;
//Variables interruptores ,rele y auxiliar
byte bot_start = 2;
byte bot_reset = 3;
byte bot_corriente = 4;
byte bot_tension = 5;
byte rele = 8;
byte bot_auxiliar = 10;
boolean est_start = 0;
boolean seg_start = 0;
boolean est_reset = 0;
boolean seg_reset = 0;
boolean est_corriente = 0;
boolean seg_corriente = 0;
boolean est_tension = 0;
boolean seg_tension = 0;
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
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(rele,     OUTPUT);
  digitalWrite(rele, LOW);
  contenido1= "  INYECTOR DE";
  contenido2= "   CORRIENTE";
  Visual();
  delay(2000);
  contenido1= "";
  contenido2="Tiempo: 000,000";
  Visual();
  Lectura_EEPROM();
  relacion1 = (digito4*1000) + (digito3*100) + (digito2*10) + digito1;
  relacion = relacion1/5;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  lectura_botones();
  if(est_corriente == 1){
    fila = 0;
    contenido1= "Corriente       ";
    fila2 = 0;
    contenido2="Tiempo: 000,000";
    Visual2();
  }else if(est_tension == 1){
    fila = 0;
    contenido1= "Auxiliar        ";
    fila2 = 0;
    contenido2="Tiempo: 000,000";
    Visual2();
  }else{
    fila=0;
    contenido1="Set -> " + String(digito4) + String(digito3) + String(digito2) + String(digito1) + "/5A";
    fila2=0;
    contenido2="                ";
    Visual2();  
  }
  if((est_start == 1)&&((est_corriente == 1)||(est_tension == 1))){
    cronometro();
  }
  if((est_start == 1)&&(est_corriente == 0)&&(est_tension == 0)){
    Conf_corriente();
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
  if((seg_start == 1)&&(est_start == 0)){   //Configuracion de est_start
    seg_start = 0;
  }
  if((seg_start == 1)&&(est_start == 1)){
    est_start = 0;  
  }
  if((seg_start == 0)&&(est_start == 1)){
    est_start = 1;
    seg_start = 1;
  }
  
  if((seg_reset == 1)&&(est_reset == 0)){   //Configuracion de est_reset
    seg_reset = 0;
  }
  if((seg_reset == 1)&&(est_reset == 1)){
    est_reset = 0;  
  }
  if((seg_reset == 0)&&(est_reset == 1)){
    est_reset = 1;
    seg_reset = 1;
  }
}

void lectura_botones2(){
  est_start = digitalRead(bot_start);
  est_reset = digitalRead(bot_reset);
  est_corriente = digitalRead(bot_corriente);
  est_tension = digitalRead(bot_tension);
  est_auxiliar = digitalRead(bot_auxiliar);
  if((seg_start == 1)&&(est_start == 0)){   //Configuracion de est_start
    seg_start = 0;
  }
  if((seg_start == 1)&&(est_start == 1)){
    est_start = 0;  
  }
  if((seg_start == 0)&&(est_start == 1)){
    est_start = 1;
    seg_start = 1;
  }
  
  if((seg_reset == 1)&&(est_reset == 0)){   //Configuracion de est_reset
    seg_reset = 0;
  }
  if((seg_reset == 1)&&(est_reset == 1)){
    est_reset = 0;  
  }
  if((seg_reset == 0)&&(est_reset == 1)){
    est_reset = 1;
    seg_reset = 1;
  }
  if((seg_tension == 1)&&(est_tension == 0)){   //Configuracion de est_tension
    seg_tension = 0;
  }
  if((seg_tension == 1)&&(est_tension == 1)){
    est_tension = 0;  
  }
  if((seg_tension == 0)&&(est_tension == 1)){
    est_tension = 1;
    seg_tension = 1;
  }
  
  if((seg_corriente == 1)&&(est_corriente == 0)){   //Configuracion de est_corriente
    seg_corriente = 0;
  }
  if((seg_corriente == 1)&&(est_corriente == 1)){
    est_corriente = 0;  
  }
  if((seg_corriente == 0)&&(est_corriente == 1)){
    est_corriente = 1;
    seg_corriente = 1;
  }
}

void Conf_corriente(){
  fila = 0;
  contenido1="RELACION TRANF";
  fila2= 0;
  contenido2="             /5A";
  Visual();
  int pos=1;
  long tiempo=0;
  long set_tiempo=millis();
  while(1){
    tiempo = millis();
    lectura_botones2();
    fila2 = 9;
    contenido2= String(digito4) + String(digito3) + String(digito2) + String(digito1);
    Visual2();
    if(tiempo>(set_tiempo + 500)){
      lcd.setCursor(13-pos,1);
      lcd.print(" ");
    }
    if(tiempo>(set_tiempo + 1000)){
      set_tiempo=millis();
    }
    if(est_corriente == 1){
      switch(pos){
        case 1: digito1++; if(digito1>9){digito1=9;} break;  
        case 2: digito2++; if(digito2>9){digito2=9;} break;  
        case 3: digito3++; if(digito3>9){digito3=9;} break;  
        case 4: digito4++; if(digito4>9){digito4=9;} break;  
      }
    }
    if(est_tension == 1){
      switch(pos){
        case 1: digito1--; if(digito1<0){digito1=0;} break;  
        case 2: digito2--; if(digito2<0){digito2=0;} break;  
        case 3: digito3--; if(digito3<0){digito3=0;} break;  
        case 4: digito4--; if(digito4<0){digito4=0;} break;  
      }
    }
    if(est_reset == 1){
      pos++;
      if(pos>4){pos = 1;}
    }
    if(est_start == 1){
      relacion1 = (digito4*1000) + (digito3*100) + (digito2*10) + digito1;
      relacion = relacion1/5;
      Escritura_EEPROM();
      break;  //Sale del bucle while
    }
  }
  seg_tension = 0;
  seg_corriente = 0;
}


void cronometro(){
    setMillis(new_value);
    byte val = 1;
    fila =0;
    contenido1="";
    fila2=0;
    contenido2 = "Tiempo: 000,000";
    Visual();
    if(est_tension == 1){
      lcd.setCursor(0,0);
      lcd.print("AUX:");
    }
    if(est_corriente == 1){
      lcd.setCursor(15,0);
      lcd.print("A");
    }
    digitalWrite(rele, HIGH);
  while(val == 1){
      digitalWrite(6, HIGH);
      unsigned long tiempo = millis();
      int segundo = tiempo/1000;
      int milisegundo = tiempo -(1000*segundo);
      //----------COMIENZA VISUAL DE TIEMPO----------
    if((vuelta_prom==120)||(est_tension == 1)){
      if(segundo<10){
          lcd.setCursor(10,1);
          lcd.print(segundo);
      }else if((segundo<100)&&(segundo>=10)){
          lcd.setCursor(9,1);
          lcd.print(segundo);
      }else{
          lcd.setCursor(8,1);
          lcd.print(segundo); 
      }
      
      if(milisegundo<10){
          lcd.setCursor(12,1);
          lcd.print("00");
          lcd.setCursor(14,1);
          lcd.print(milisegundo);
      }else if((milisegundo<100)&&(milisegundo>=10)){
          lcd.setCursor(12,1);
          lcd.print("0");
          lcd.setCursor(13,1);
          lcd.print(milisegundo);
      }else{
          lcd.setCursor(12,1);
          lcd.print(milisegundo);
      }
      vuelta_prom = 0;
  }
      //----------TERMINA VISUAL DE TIEMPO----------
      //----------RUTINA DE RECOLECCION Y CALCULO DE CORRIENTE----------
      if(est_corriente == 1){   
        corriente();
      }
      //----------RUTINA DE AUXILIAR----------
      lectura_botones();
      if(est_tension == 1){
        if(est_auxiliar == 0){  
          lcd.setCursor(5,0);
          lcd.print("ABIERTO");
        }
        if(est_auxiliar == 1){
          lcd.setCursor(5,0);
          lcd.print("CERRADO");
        }
      
  }
      if(tiempo > 100){
        if(est_tension == 1){
          if(est_auxiliar == 0){
            digitalWrite(rele, LOW);
            val = 0;
          }
        }
      }
      if(tiempo > 100){
        if(est_corriente == 1){
          if(val_max < val_comp){
            digitalWrite(rele, LOW);
            val = 0;
            /*
            fila= 0;
            contenido1= "0.00";
            Visual2();    */
          }
        }
      }
        if(est_reset == 1){  //Si Reset es presionado
          digitalWrite(rele, LOW);
          new_value = tiempo;
          val = 0; //Para salir del bucle while
        }
    digitalWrite(6, LOW);
    delayMicroseconds(5);
    }
    while(1){
      lectura_botones();
      if(est_reset == 1){
        break;
      }
    }
}

void corriente(){
  float valor = analogRead(A2);
  Valor_sumado = Valor_sumado + valor;
  vuelta_prom++;
  if(vuelta_prom==120){
    digitalWrite(7,HIGH);
    float tension = (Valor_sumado*5)/1023;
    val_max = (tension/120)*1.57079;
    v_fs = v_fs + val_max;
    cont++;
    if(cont == 10){
      v_fs = v_fs/10;
      visual_corriente();
      cont = 0;
      v_fs = 0;
    }
    Valor_sumado = 0;
    digitalWrite(7,LOW);
  }
}

void visual_corriente(){
  float Val_ajustado = v_fs * relacion;
    if((Val_ajustado <10000)&&(Val_ajustado>=1000)){
      lcd.setCursor(0,0);
      lcd.print(Val_ajustado,0);
    }else if((Val_ajustado <1000)&&(Val_ajustado>=100)){
      lcd.setCursor(0,0);
      lcd.print(Val_ajustado,0);
      lcd.setCursor(3,0);
      lcd.print(" ");
    }else if((Val_ajustado <100)&&(Val_ajustado>=10)){
      lcd.setCursor(0,0);
      lcd.print(Val_ajustado,1);
    }else if(Val_ajustado <10){
      lcd.setCursor(0,0);
      lcd.print(Val_ajustado,2);
    }
    
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
  contenido2 = "Tiempo: 000,000";
  Visual2();
}

void setMillis(unsigned long new_millis){
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}

void Lectura_EEPROM(){
  digito1 = EEPROM.read(1);
  digito2 = EEPROM.read(2);
  digito3 = EEPROM.read(3);
  digito4 = EEPROM.read(4);
}

void Escritura_EEPROM(){
  EEPROM.put(1,digito1);
  EEPROM.put(2,digito2);
  EEPROM.put(3,digito3);
  EEPROM.put(4,digito4);
}
