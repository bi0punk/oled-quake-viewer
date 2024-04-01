#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "Zeus";
const char* password = "4xD46>WV";
const char* serverUrl = "http://192.168.1.92:8000/api/sismo/latest/";

unsigned long lastRequestTime = 0;
const unsigned long requestInterval = 300000;

WiFiClient client;
HTTPClient http;

String fechaLocal;
String ubicacion;
float magnitud;
float profundidad;
String latitud;
String longitud;

bool isDataSwapped = false;

void setup() {
    Serial.begin(115200);
    Wire.begin(14, 12);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("No se pudo encontrar la pantalla OLED"));
        for(;;);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(F("Inicializando..."));
    display.display();

    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
  
    Serial.println("WiFi conectado");
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());

    display.clearDisplay();

    for(int i = 0; i <= 100; i+=10) {
        display.fillRect(0, 15, (int)(1.28*i), 10, WHITE);
        display.display();
        delay(100);
    }

    display.clearDisplay();
}

void replaceFirst(String& original, const String& search, const String& replace) {
    int pos = original.indexOf(search);
    if (pos != -1) {
        original.setCharAt(pos, replace[0]);
    }
}

void loop() {
    if (millis() - lastRequestTime >= requestInterval || lastRequestTime == 0) {
        lastRequestTime = millis();

        http.begin(client, serverUrl);
        int httpCode = http.GET();
    
        if (httpCode > 0) {
            String payload = http.getString();
            Serial.println(payload);
      
            DynamicJsonDocument doc(512);
            deserializeJson(doc, payload);
      
            fechaLocal = doc["fecha_local"].as<String>();
            ubicacion = doc["ubicacion"].as<String>();
            magnitud = doc["magnitud"].as<float>();
            profundidad = doc["profundidad"].as<float>();
            latitud = doc["latitud"].as<String>();
            longitud = doc["longitud"].as<String>();

            ubicacion.replace("ü", "\u00FC");
            ubicacion.replace(" al ", " - ");
            ubicacion.replace(" km ", " Km ");
            replaceFirst(ubicacion, " de ", " - ");

            display.clearDisplay();

            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(0, 0);
            display.println(" " + fechaLocal);

            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(0, (SCREEN_HEIGHT / 4) - 8); // Resta 8 para subir una posición el texto
            display.println(latitud);

            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor((SCREEN_WIDTH - display.getCursorX()) - 1, (SCREEN_HEIGHT / 4) - 8); // Resta 1 para mover una posición a la izquierda
            display.println("Lat");

            char magnitudStr[6]; // Array para almacenar la cadena de la magnitud
            dtostrf(magnitud, 4, 1, magnitudStr); // Convertir magnitud a una cadena con una décima
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor((SCREEN_WIDTH - 30) / 2 - 12, (SCREEN_HEIGHT - 15) / 2); // Ajuste de posición del dígito de magnitud
            display.println(magnitudStr);

            String depthText = "Deep: " + String(profundidad) + " Km";
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor((SCREEN_WIDTH - 80) / 2, (SCREEN_HEIGHT + 15) / 2);
            display.println(depthText);

            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(SCREEN_WIDTH - 50, (SCREEN_HEIGHT / 2 - 16) - 8); // Resta 8 para subir una posición el valor
            display.println(longitud);


            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor((SCREEN_WIDTH - 30) + 8, (SCREEN_HEIGHT / 2 - 8) - 8); // Suma 8 para mover una posición más a la derecha
            display.println("Lon");


            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(0, SCREEN_HEIGHT - 10);
            display.startscrollleft(0x00, 0x0F);
            display.println(ubicacion);
            display.stopscroll();
      
            display.display();

            if (!isDataSwapped) {
                isDataSwapped = true;
                display.clearDisplay();
            } else {
                isDataSwapped = false;
                // No necesitas limpiar la pantalla aquí ya que los datos se están actualizando.
            }
        } 
        http.end();
    }
}
