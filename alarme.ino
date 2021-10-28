#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte LINHAS = 4;
const byte COLUNAS = 3;

LiquidCrystal_I2C lcd(0x27,16,2);

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

String horario = "";
unsigned long myTime;
int hora;
int tempo_restante;
int aux;

void contagem(){
  hora = horario.toInt();
  hora = hora + millis()/1000;
  
  while (0 == 0){
    myTime = millis()/1000;
    tempo_restante = hora - myTime;
    
    if (tempo_restante != aux){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Tempo restante:");
      lcd.setCursor(7,1);
      lcd.print(tempo_restante);
      aux = tempo_restante;
    }
  
    if (tempo_restante <= 0){
      
      digitalWrite(3, HIGH);
      while (0 == 0){
        if (digitalRead(2) == HIGH){
          digitalWrite(3, LOW);
          return;
        }
      }
    }
  }
}

void set_display(){
  lcd.init();
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("Tempo para");
  lcd.setCursor(0,1);
  lcd.print("o alarme: ");
}

void setup(){
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  Serial.begin(9600);
  set_display();
}

void loop(){
  char tecla_pressionada = meuteclado.getKey();

  if (tecla_pressionada){
    if (tecla_pressionada == '*'){
      lcd.clear();
      lcd.print("Tempo restante:");
      contagem();
      set_display();
      horario = "";
      hora = 0;
      aux = 0;
    }
    
    else if (tecla_pressionada != '#'){
      lcd.print(tecla_pressionada);
      horario += tecla_pressionada;
    }
  }
}
