#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

int greenLED = 7;
int redLED = 8;
int buzzer = 6;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // RFID kartı algılandı
    Serial.println("RFID Kart Algılandı!");

    // Kartın UID'sini oku
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      uid.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    uid.toUpperCase();

    Serial.println("UID Etiketi: ");
    Serial.println(uid);

    // Erişim izni kontrolü
    if (uid.equals("83A2E1FC")) {
      // Erişim izni var
      Serial.println("Erişime izin verildi!");
      digitalWrite(greenLED, HIGH);
      digitalWrite(redLED, LOW);
      tone(buzzer, 1000, 1000);  // 1000 Hz frekansında 1 saniye süreyle ses çal
      delay(2000);
    } 
    else {
      // Erişim izni yok
      Serial.println("Erişim Reddedildi!");
      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, HIGH);
      tone(buzzer, 500, 1000);  // 500 Hz frekansında 1 saniye süreyle ses çal
      delay(2000);
    }

    // 1 saniye sonra LED'leri ve buzzer'ı kapat
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);
    noTone(buzzer);

    mfrc522.PICC_HaltA();
  }
}

