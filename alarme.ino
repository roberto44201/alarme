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

String set_time = "";   //string usada para definição do horário do alarme
unsigned long myTime;   //tempo atual, em millis
int fire_time;          //tempo a despertar, em millis
int time_remaining;     //tempo restante para despertar
int aux;                //variável auxiliar
bool countdown = false; //condições de alarme ativo, alarme disparando e alarme desligado
bool alarm_on = false;
bool wake_up = false;

void alarm_firing(){      //se o alarme estiver disparando
  digitalWrite(3, HIGH);  //ativa o buzzer
  aux = millis();
  while (0 == 0){
    if (digitalRead(2) == HIGH){  //se o botão for pressionado, desliga o alarme
      digitalWrite(3, LOW);
      countdown = false;
      alarm_on = false;
      set_time = "";
      fire_time = 0;
      aux = 0;
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
    if (countdown){           // se o alarme estiver ativo
      if (time_remaining != aux){   //atualiza a cada segundo
        lcd.clear();
        lcd.print("Tempo restante:");
        lcd.setCursor(0,1);
        lcd.print(time_remaining);
        aux = time_remaining;
      }
    }

    else{             //se o alarme disparar
      if (wake_up){
        digitalWrite(13, LOW);  //reinicia o ISD1820
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
        lcd.print(analogRead(1)* 0.0048828125 * 100);
        lcd.print(" oC");
        delay(4000);
        wake_up = false;    //alarme desligado
        lcd.clear();
      }
      lcd.clear();
      lcd.print("Tempo para");
      lcd.setCursor(0,1);
      lcd.print("o alarme: ");
    }
}

void get_key(){
  while (0 == 0){
    
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
  pinMode(12, INPUT);  //define os pinos
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(3, LOW);  //desliga o alarme
  lcd.init();
  lcd.setBacklight(HIGH);
  Serial.begin(9600);
}

void loop(){
  print_display();
  
  if (alarm_on){    //se o alarme disparar
    alarm_firing();
    print_display();
  }

  if ((not countdown) and (not wake_up)){     //função para definição do alarme, se ele já não estiver ativo
    get_key();
  }

  else if (countdown){    //calcula o tempo até o disparo
    counting();
  }
}
