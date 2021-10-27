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

void setup(){
 lcd.init();
 lcd.setCursor(0,0);
 lcd.print("Tempo para");
 lcd.setCursor(0,1);
 lcd.print("o alarme: ");

}

void loop(){
  char tecla_pressionada = meuteclado.getKey();

  if (tecla_pressionada){
    lcd.print(tecla_pressionada);
  }
}
