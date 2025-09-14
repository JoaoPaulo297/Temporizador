#include <Adafruit_LiquidCrystal.h>


Adafruit_LiquidCrystal lcd(0);

int seconds = 0;
int buzzerPin = 9;
int pauseButton = 7;
int stopAlarmButton = 8;
bool alarmActive = false;
bool paused = false;
bool lastPauseState = HIGH;

void setup() {
  lcd.begin(16,2); // Inicializa o LCD I2C

  pinMode(buzzerPin, OUTPUT);
  pinMode(pauseButton, INPUT_PULLUP);
  pinMode(stopAlarmButton, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  
  lcd.setCursor(0, 0);
  lcd.print("Defina o tempo:");
  lcd.setCursor(0, 1);
  lcd.print("Formato:hh:mm:ss");

  while (Serial.available() == 0) {
    lcd.setCursor(0, 1);
    lcd.print("Aguardando tempo");
    delay(500);
  }

  // Lê o tempo do monitor serial
  String input = Serial.readStringUntil('\n'); 
  int hours = 0, minutes = 0, sec = 0;
  int colonIndex = input.indexOf(':');
  
  if (colonIndex != -1) {
    hours = input.substring(0, colonIndex).toInt();
    minutes = input.substring(colonIndex + 1, colonIndex + 3).toInt();
    sec = input.substring(colonIndex + 4).toInt();
  }
  seconds = hours * 3600 + minutes * 60 + sec;
  lcd.clear();
  lcd.print("Tempo Definido:");
  lcd.setCursor(0, 1);
  if(hours < 10) lcd.print("0");
  lcd.print(hours);
  lcd.print(":");
  if(minutes < 10) lcd.print("0");
  lcd.print(minutes);
  lcd.print(":");
  if (sec < 10) lcd.print("0");
  lcd.print(sec);
  delay(1000);
  lcd.clear();
}

void loop() {
  while (seconds > 0) {
    if (digitalRead(pauseButton) == LOW && lastPauseState == HIGH) {
      paused = !paused;
      lcd.clear();
      if (paused) {
        lcd.print("Pausado");
      } else {
        lcd.print("Retomando...");
        delay(1000);
      }
    }
    lastPauseState = digitalRead(pauseButton);

    if (!paused) {
      int hours = seconds / 3600;        // Cálculo da hora na tela
      int minutes = (seconds % 3600) / 60;   // Cálculo dos minutos na tela
      int secs = (seconds % 3600) % 60;    // Cálculo dos segundos na tela

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tempo: ");
      if(hours < 10) lcd.print("0");
      lcd.print(hours);
      lcd.print(":");
      if(minutes < 10) lcd.print("0");
      lcd.print(minutes);
      lcd.print(":");
      if (secs < 10) lcd.print("0");
      lcd.print(secs);
      
      delay(1000);
      seconds--;
    }
  }

  // Alarme quando o tempo acabar
  alarmActive = true;
  while (alarmActive) {
    tone(buzzerPin, 1000);
    lcd.clear();
    lcd.print("Tempo Esgotado!");
    lcd.setCursor(0, 1);
    lcd.print("Pressione STOP");

    if (digitalRead(stopAlarmButton) == LOW) {
      noTone(buzzerPin);
      alarmActive = false;
      lcd.clear();
      lcd.print("Alarme Parado");
      delay(1000);
      lcd.clear();
      setup();
    }
  }
}
