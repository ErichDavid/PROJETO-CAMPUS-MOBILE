#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>



#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);



#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);



Adafruit_BMP280 bmp;


#define LED_PIN 4



void setup() {

  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  
  Wire.begin(21, 22);

  
  dht.begin();

 
  if (!bmp.begin(0x76)) {

    Serial.println("BMP280 nao encontrado!");

    
    if (!bmp.begin(0x77)) {

      Serial.println("BMP280 tambem nao encontrado em 0x77!");
    }
  }

 
  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS)) {

    Serial.println("OLED nao encontrado!");

    while (true);
  }

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);

  display.setCursor(10, 10);

  display.println("INICIANDO");

  display.display();

  delay(2000);
}


void loop() {

  // Le temperatura do BMP280
  float temperatura = bmp.readTemperature();

  // Le umidade do DHT22
  float umidade = dht.readHumidity();

  // Verifica erro no DHT22
  if (isnan(umidade)) {

    Serial.println("Erro no DHT22!");

    display.clearDisplay();

    display.setTextSize(1);

    display.setCursor(0, 20);

    display.println("Erro no sensor");
    display.println("de umidade!");

    display.display();

    digitalWrite(LED_PIN, LOW);

    delay(2000);

    return;
  }


  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");

  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println(" %");

 
  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);

  display.print("TEMP: ");

  display.print(temperatura, 1);

  display.println(" C");

  display.setCursor(0, 12);

  display.print("UMID: ");

  display.print(umidade, 0);

  display.println(" %");


  if (temperatura < 30) {

    digitalWrite(LED_PIN, LOW);

    display.setCursor(0, 28);

    display.println("Nivel: NORMAL");

    display.setCursor(0, 42);

    display.println("Mantenha-se");

    display.println("hidratado.");
  }

  

  else if (temperatura < 35) {

    digitalWrite(LED_PIN, LOW);

    display.setCursor(0, 28);

    display.println("Nivel: ATENCAO");

    display.setCursor(0, 42);

    display.println("Evite exposicao");

    display.println("prolongada ao sol.");
  }



  else if (temperatura < 40) {

    digitalWrite(LED_PIN, HIGH);

    display.setCursor(0, 28);

    display.println("CALOR ALTO!");

    display.setCursor(0, 42);

    display.println("Beba agua e");

    display.println("procure sombra.");
  }


  else {

    digitalWrite(LED_PIN, HIGH);

    display.setCursor(0, 28);

    display.println("ALERTA EXTREMO!");

    display.setCursor(0, 42);

    display.println("Evite o sol.");

    display.println("Procure local fresco.");
  }

 
  display.display();

 
  delay(2000);
}