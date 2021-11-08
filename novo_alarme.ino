#include <Thermistor.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


LiquidCrystal_I2C lcd(0x27,16,2);
//Thermistor temp(1);

const byte LINHAS = 4;
const byte COLUNAS = 3;
char matriz_teclas[LINHAS][COLUNAS] = 
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte PinosLinhas[LINHAS] = {4, 5, 6, 7};
byte PinosColunas[COLUNAS] = {8, 9, 10};

Keypad meuteclado = Keypad( makeKeymap(matriz_teclas), PinosLinhas, 
                            PinosColunas, LINHAS, COLUNAS); 

String set_time = "";
unsigned long myTime;
int fire_time;
int time_remaining;
int aux;
int aux_despertador = 0;
bool countdown = false;
bool alarm_on = false;
bool wake_up = false;
int temp_aux;

void alarm_firing(){
  digitalWrite(3, HIGH);
  aux = millis();
  Serial.println(aux_despertador);
  while (0 == 0){
    //digitalWrite(3, HIGH);
    //delay(300);
    //digitalWrite(3, LOW);
    //delay(300);
    /*
    if (millis() == aux+300){
      Serial.println("oi");
      if (aux_despertador == 0){
        digitalWrite(3, LOW);
        aux = millis();
        aux_despertador = 1;
      }
      else{
        digitalWrite(3, HIGH);
        aux = millis();
        aux_despertador = 0;
      }
    }
    */
    if (digitalRead(2) == HIGH){
      digitalWrite(3, LOW);
      countdown = false;
      alarm_on = false;
      set_time = "";
      fire_time = 0;
      aux = 0;
      aux_despertador = 0;
      wake_up = true;
      return;
    }
  }
}

void counting(){
  myTime = millis()/1000;
  time_remaining = fire_time - myTime;
      
  if (time_remaining <= 0){
    alarm_on = true;
  }
}

void print_display(){
  lcd.setCursor(0,0);

  //if ((digitalRead(12) == HIGH) or (alarm_on)){
    if (countdown){
      if (time_remaining != aux){
        lcd.clear();
        lcd.print("Tempo restante:");
        lcd.setCursor(0,1);
        lcd.print(time_remaining);
        aux = time_remaining;
      }
    }

    else{
      if (wake_up){
        digitalWrite(13, LOW);
        digitalWrite(13, HIGH);
        lcd.clear();
        lcd.print("Bom dia, fulano!");
        lcd.setCursor(0,1);
        lcd.print("Hora de acordar!");
        delay(4000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("A temperatura e");
        lcd.setCursor(0,1);
        //lcd.print(temp.getTemp());
        lcd.print(analogRead(1)* 0.0048828125 * 100);
        lcd.print(" oC");
        delay(4000);
        wake_up = false;
        lcd.clear();
      }
      lcd.clear();
      lcd.print("Tempo para");
      lcd.setCursor(0,1);
      lcd.print("o alarme: ");
    }
  //}
  /*else{
    temp = get_temp();
    if (temp - temp_aux >= 3 or temp - temp_aux <= -3){
      lcd.clear();
      lcd.print("Temperatura:");
      lcd.setCursor(0,1);
      lcd.print(temp);
      temp_aux = temp;
    }
  }*/
}

void get_key(){
  while (0 == 0){
  //while (digitalRead(12) == HIGH){
    
    char tecla_pressionada = meuteclado.getKey();
  
    if (tecla_pressionada){
      if (tecla_pressionada == '*' and set_time != "" and set_time.toInt() != 0){
        countdown = true;
        fire_time = set_time.toInt();
        fire_time += millis()/1000;
        return;
      }
      
      else if (tecla_pressionada != '#' and tecla_pressionada != '*'){
        lcd.print(tecla_pressionada);
        set_time += tecla_pressionada;
      }
    }
  }
}

void setup(){
  pinMode(12, INPUT);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(3, LOW);
  lcd.init();
  lcd.setBacklight(HIGH);
  Serial.begin(9600);
}

void loop(){
  print_display();
  
  if (alarm_on){
    alarm_firing();
    print_display();
  }

  if ((not countdown) and (not wake_up)){
    get_key();
  }

  else if (countdown){
    counting();
  }
}
