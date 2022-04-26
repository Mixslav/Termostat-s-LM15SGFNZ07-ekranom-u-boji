// Mislav Stipić 2016

/**
 * The LM15SGFNZ07 is an LCD with a 101x80 pixels resolution, capable of displaying 4096 colors. It is found in Siemens
 * mobile phones (models A65, M55, C60, MC60, S55, A60). Connect the LCD to your Arduino as follows:
 * LCD pin   -->   Arduino pin
 *   1 (/CS)        6
 *   2 (/RST)       5
 *   3 (RS)         4
 *   4 (Sclk)       13 (or SPI SCK)
 *   5 (Sdata)      11 (or SPI MOSI)
 *   6 (+3.3v)      3.3v
 *   7 (GND)        GND
 *   8 (LED1)       3.3v
 *   9 (GND)        GND
 *  10 (LED2)       3.3v
 */

#include <SPI.h>
#include <LM15SGFNZ07.h>


LM15SGFNZ07 lcd(6, 5, 4);


float sensorPin = A0;   //senzor temperature
float regul_pin = A1;   //regulacija temperature
int relay = 12;         //relej
int relay_state = LOW;

const unsigned long period = 1000; //1000 ms
unsigned long prev_time = 0;

void setup()
{
  lcd.init();
  pinMode(relay, OUTPUT);
  intro();
  Serial.begin(9600);
}

void loop() {

float sensor_Value = analogRead(A0);
float regul_Value = analogRead(A1);
unsigned long currentMillis = millis();

char current_temp =  char(analogRead(A0),1); //pretvorba analognog ulaza float sa 1 dec mjestom u char 
char poten_temp =  char(analogRead(A1),1);

workScreen(current_temp, poten_temp);

 if (currentMillis - prev_time >= period*60*5) { // 1000ms * 60 * 5 = 5 min, period
    
    prev_time = currentMillis;  // spremanje zadnjeg vremena millis() funkcije i čekaj za nastavak brojanja
    
    if(sensor_Value <= regul_Value && relay_state == LOW)
    
    relay_state = HIGH;
    
    else 
    relay_state = LOW;
    
    digitalWrite(relay, relay_state);
 }

 


}



void intro() {
  lcd.clear(0x000);
  lcd.fillRect(1,  0, 14, 45, 0xFFF);
  lcd.fillRect(15, 0, 14, 45, 0xFF0);
  lcd.fillRect(29, 0, 14, 45, 0x0FF);
  lcd.fillRect(43, 0, 14, 45, 0x0F0);
  lcd.fillRect(57, 0, 14, 45, 0xF0F);
  lcd.fillRect(71, 0, 14, 45, 0xF00);
  lcd.fillRect(85, 0, 14, 45, 0x00F);
  
  lcd.fillRect(1, 45, 98, 15, 0xF00);
  lcd.drawString("Termostat", 18, 49, 0xFFF, 0xF00);
  
  for (int i = 0; i < 16; i ++) {
    lcd.fillRect(i * 6 + 1, 60, 8, 5, (i << 8) + (i << 4) + i);
    lcd.fillRect(i * 6 + 1, 65, 8, 5, i << 8);
    lcd.fillRect(i * 6 + 1, 70, 8, 5, i << 4);
    lcd.fillRect(i * 6 + 1, 75, 8, 5, i);
  }
  
  delay(5000);
}


char workScreen(char temp, char poten) {
  lcd.clear(0xFFF);

  lcd.fillRect(0, 0, 101, 80, 0x00F);
  lcd.fillRect(1, 1, 100, 79, 0x00F);
  lcd.fillRect(2, 2, 99, 78, 0xFF0);

  lcd.drawString("TEMP: ", 4, 4, 0xFF0, 0xFFF);
  lcd.drawString(temp, 16, 4, 0xFF0, 0xFFF);
  lcd.drawString("°", 24, 4, 0xFF0, 0xFFF);

  lcd.drawString("POTEN: ", 4, 10, 0xFF0, 0xFFF);
  lcd.drawString(poten, 16, 16, 0xFF0, 0xFFF);
  lcd.drawString("°", 24, 20, 0xFF0, 0xFFF);

  delay(1);
}
