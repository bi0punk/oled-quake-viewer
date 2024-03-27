#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128 // ancho de pantalla OLED
#define SCREEN_HEIGHT 64 // alto de pantalla OLED
#define OLED_RESET    -1 // Reset no es requerido en algunos modelos

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Datos de la red WiFi
const char* ssid     = "";
const char* password = "";

// URL del endpoint Django

const char* serverUrl = "http://192.168.1.X:8000/api/";
const char* serverUrl = "http://192.:X000/api/";

unsigned long lastRequestTime = 0;
const unsigned long requestInterval = 180000; // Intervalo de solicitud en milisegundos

// Objeto HTTPClient global para reutilización
WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  
  // Inicia el bus I2C con pines personalizados
  Wire.begin(14, 12); // SDA, SCL
  
  // Inicia la pantalla OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("No se pudo encontrar la pantalla OLED"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  // Calcula el ancho del texto para centrarlo
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(F("Inicializando..."), 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH-w)/2, (SCREEN_HEIGHT-h)/2);
  
  display.println(F("Inicializando..."));
  display.display();

  // Inicia la conexión WiFi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay(); // Limpia la pantalla para la barra de carga y nuevos datos
  
  // Barra de carga
  for(int i = 0; i <= 100; i+=10) {
    display.fillRect(0, 15, (int)(1.28*i), 10, WHITE);
    display.display();
    delay(100);
  }

  display.clearDisplay(); // Limpia la pantalla para mostrar nuevos datos
}

void loop() {
  // Verifica si es tiempo de hacer una solicitud
  if (millis() - lastRequestTime >= requestInterval || lastRequestTime == 0) {
    lastRequestTime = millis(); // Actualiza el tiempo de la última solicitud

    // Realiza la solicitud HTTP al servidor Django
    http.begin(client, serverUrl);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      // Si la solicitud fue exitosa, lee la respuesta JSON
      String payload = http.getString();
      Serial.println(payload);
      // Analiza el JSON y muestra los datos en la pantalla OLED
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(F("Sismologia | Wifi: OK"));
      display.println(F("---------------------"));
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      display.print(F("F:"));
      display.println(doc["fecha_local"].as<String>());
      display.print(F("U:"));
      display.println(doc["ubicacion"].as<String>());
      display.print(F("Lat:"));
      display.print(doc["latitud"].as<float>(), 3); // Muestra 3 decimales para latitud
      display.print(F(" Mag:"));
      display.println(doc["magnitud"].as<float>(), 2); // Muestra 2 decimales para magnitud
      display.print(F("Lon:"));
      display.print(doc["longitud"].as<float>(), 3); // Muestra 3 decimales para longitud
      display.print(F(" Prof:"));
      display.println(doc["profundidad"].as<float>(), 1); // Muestra 1 decimal para profundidad
      display.println(WiFi.localIP());
      display.display();
    } 
    
    http.end(); // Libera los recursos de la solicitud HTTP
  }

  // Puedes agregar más funcionalidad aquí
}
