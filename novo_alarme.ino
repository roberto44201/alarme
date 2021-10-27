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

byte PinosLinhas[LINHAS] = {3, 4, 5, 6};
byte PinosColunas[COLUNAS] = {8, 9, 10};

Keypad meuteclado = Keypad( makeKeymap(matriz_teclas), PinosLinhas, 
                            PinosColunas, LINHAS, COLUNAS); 

String horario = "";
bool activate = false;
unsigned long myTime;

void setup(){
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  lcd.init();
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("Tempo para");
  lcd.setCursor(0,1);
  lcd.print("o alarme: ");
  Serial.begin(9600);

}

void loop(){
  char tecla_pressionada = meuteclado.getKey();

  if (tecla_pressionada){
    lcd.print(tecla_pressionada);
    Serial.println(tecla_pressionada);

    if (tecla_pressionada == '*'){
      lcd.clear();
      lcd.print("Alarme salvo");
      activate = true;
    }
    else{
      horario += tecla_pressionada;
    }
  }
  if (activate){
    myTime = millis();
    if (myTime/1000 >= horario.toInt()){
      Serial.println(horario.toInt());
      Serial.println(myTime);
      lcd.clear();
      lcd.print("Despertar");
      digitalWrite(2, HIGH);
      delay(200);
      digitalWrite(2, LOW);
      delay(200);
      
    }
  }
}
