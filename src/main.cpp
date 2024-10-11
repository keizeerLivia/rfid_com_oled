#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <SPI.h>
#include <MFRC522.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DHTPIN 4          // Pino do DHT22
#define DHTTYPE DHT22     // Tipo de DHT

#define OLED_SDA 21       // Pino SDA do OLED
#define OLED_SCL 22       // Pino SCL do OLED

#define SS_PIN 5          // Pino SDA do RFID
#define RST_PIN 15        // Pino RST do RFID
#define SCK_PIN 25       // Novo pino SCK do RFID

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Inicializa o OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  
  // Inicializa o RFID
  SPI.begin();
  rfid.PCD_Init();
  
  Serial.println("DHT22, OLED e RFID prontos!");
}

void loop() {
  // Lê a temperatura e umidade
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Verifica falhas de leitura
  if (isnan(h) || isnan(t)) {
    Serial.println("Falha na leitura do DHT!");
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(t);
    display.println(" C");
    display.print("Umid: ");
    display.print(h);
    display.println(" %");
    display.display();
  }

  // Lê RFID
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    Serial.print("UID do Cartão: ");
    for (byte i = 0; i < rfid.uid.size; i++) {
      Serial.print(rfid.uid.uidByte[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    rfid.PICC_HaltA();
    display.clearDisplay();
  }

  delay(3000); // Aguarda 3 segundos antes da próxima leitura
}
