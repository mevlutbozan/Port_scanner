#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

#define BUZZER_PIN 8
int leds[] = {2, 3, 4, 5, 6}; // 5 LED pini

// Nota frekansları - daha doğru frekanslar
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define REST     0

// "Mini Mini Kuş" melodisi - düzeltilmiş
int melody[] = {
  NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_D4, NOTE_C4,
  
  NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4,
  NOTE_E4, NOTE_D4, NOTE_C4,
  
  NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4,
  NOTE_E4, NOTE_D4, NOTE_C4
};

// Süreler (ms) - daha doğru ritim
int noteDurations[] = {
  500, 500, 250, 500, 500, 250,
  500, 250, 500,
  
  500, 500, 250, 500,
  500, 250, 500,
  
  500, 500, 250, 500,
  500, 250, 750
};

// LED dizisi: Hangi LED yanacak (0–4), -1 ise LED yok
int ledPattern[] = {
  0, 1, 2, 3, 4, 3,
  2, 1, 0,
  
  1, 2, 3, 4,
  3, 2, 1,
  
  0, 1, 2, 3,
  4, 3, 2
};

// Ritim vurgu seviyesi (0=zayıf, 1=orta, 2=güçlü)
int rhythmPattern[] = {
  1, 0, 1, 2, 2, 1,
  0, 2, 1,
  
  1, 0, 1, 2,
  1, 2, 1,
  
  1, 0, 1, 2,
  1, 2, 2
};

// Şarkı sözleri
String lyrics[] = {
  "Mini mini bir kus",
  "Donmustu",
  "Pencereme konmustu",
  "Aldim onu iceri",
  "Cik cik cik otsun diye"
};

int lyricCount = sizeof(lyrics) / sizeof(lyrics[0]);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  for (int i = 0; i < 5; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mini Mini Kus");
  delay(1500);
}

void loop() {
  int notesCount = sizeof(melody) / sizeof(melody[0]);
  int lyricIndex = 0;

  for (int i = 0; i < notesCount; i++) {
    // Şarkı sözlerini uygun aralıklarla göster
    if (i % (notesCount / lyricCount) == 0 && lyricIndex < lyricCount) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sozler:");
      lcd.setCursor(0, 1);
      lcd.print(lyrics[lyricIndex]);
      lyricIndex++;
    }

    // Tüm LED'leri kapat
    for (int j = 0; j < 5; j++) {
      digitalWrite(leds[j], LOW);
    }

    // Nota sesi çal
    if (melody[i] == REST) {
      noTone(BUZZER_PIN);
    } else {
      tone(BUZZER_PIN, melody[i]);
    }

    // LED animasyonu ritme göre
    if (ledPattern[i] >= 0) {
      int ledPin = leds[ledPattern[i]];
      switch (rhythmPattern[i]) {
        case 0: // Zayıf vurgu
          digitalWrite(ledPin, HIGH);
          delay(noteDurations[i] / 3);
          digitalWrite(ledPin, LOW);
          delay(noteDurations[i] * 2 / 3);
          break;

        case 1: // Orta vurgu
          digitalWrite(ledPin, HIGH);
          delay(noteDurations[i]);
          digitalWrite(ledPin, LOW);
          break;

        case 2: // Güçlü vurgu – yanıp sönme + komşu LED'ler
          for (int k = 0; k < 3; k++) {
            digitalWrite(ledPin, HIGH);
            if (ledPattern[i] > 0) digitalWrite(leds[ledPattern[i] - 1], HIGH);
            if (ledPattern[i] < 4) digitalWrite(leds[ledPattern[i] + 1], HIGH);
            delay(noteDurations[i] / 8);
            digitalWrite(ledPin, LOW);
            if (ledPattern[i] > 0) digitalWrite(leds[ledPattern[i] - 1], LOW);
            if (ledPattern[i] < 4) digitalWrite(leds[ledPattern[i] + 1], LOW);
            delay(noteDurations[i] / 8);
          }
          break;
      }
    } else {
      delay(noteDurations[i]);
    }

    // Nota bitince sesi kes
    noTone(BUZZER_PIN);
  }

  delay(2000); // Melodi bitince bekle
}