#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27,16,2);

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
bool countdown = false;
bool alarm_on = false;
int temp;
int temp_aux;

float get_temp(){
  return temp = (analogRead(A1) * 0.0048828125 * 100);
}

void alarm_firing(){
  digitalWrite(3, HIGH);
  while (0 == 0){
    if (digitalRead(2) == HIGH){
      digitalWrite(3, LOW);
      countdown = false;
      alarm_on = false;
      set_time = "";
      fire_time = 0;
      aux = 0;
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

  if ((digitalRead(12) == HIGH) or (alarm_on)){
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
      lcd.clear();
      lcd.print("Tempo para");
      lcd.setCursor(0,1);
      lcd.print("o alarme: ");
    }
  }
  else{
    temp = get_temp();
    if (temp - temp_aux >= 3 or temp - temp_aux <= -3){
      lcd.clear();
      lcd.print("Temperatura:");
      lcd.setCursor(0,1);
      lcd.print(temp);
      temp_aux = temp;
    }
  }
}

void get_key(){
  while (digitalRead(12) == HIGH){
    Serial.println(digitalRead(12));
    
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
  digitalWrite(3, LOW);
  lcd.init();
  lcd.setBacklight(HIGH);
  Serial.begin(9600);
}

void loop(){
  Serial.println(digitalRead(12));
  print_display();
  
  if (alarm_on){
    alarm_firing();
    print_display();
  }
  if ((not countdown) and (digitalRead(12) == HIGH)){
    get_key();
  }

  else if (countdown){
    counting();
  }
}
