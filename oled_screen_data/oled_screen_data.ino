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
const char* ssid     = "Zeus";
const char* password = "4xD46>WV";

// URL del endpoint Django
const char* serverUrl = "http://192.168.1.87:8000/api/sismo/latest/";

unsigned long lastRequestTime = 0;
const unsigned long requestInterval = 180000; // Intervalo de solicitud en milisegundos

// Objeto HTTPClient global para reutilización
WiFiClient client;
HTTPClient http;

String fechaLocal;
String ubicacion;
float latitud;
float longitud;
float magnitud;
float profundidad;
IPAddress localIP;

int16_t x1, y1_coord; // Cambio de nombre de la variable y1 a y1_coord
uint16_t w, h;

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
  display.getTextBounds(F("Inicializando..."), 0, 0, &x1, &y1_coord, &w, &h); // Uso de la variable y1_coord
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
      
      // Analiza el JSON y guarda los datos
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      
      fechaLocal = doc["fecha_local"].as<String>();
      ubicacion = doc["ubicacion"].as<String>();
      latitud = doc["latitud"].as<float>();
      longitud = doc["longitud"].as<float>();
      magnitud = doc["magnitud"].as<float>();
      profundidad = doc["profundidad"].as<float>();
      localIP = WiFi.localIP();

      // Reemplazar caracteres acentuados
      ubicacion.replace("ü", "\u00FC");

      ubicacion.replace(" al ", " | "); // Reemplaza " al " por " | "
      // Modificar la cadena de ubicación
      ubicacion.replaceFirst(" de ", " | "); // Reemplaza la primera ocurrencia de " de " por " | "


      // Imprimir la ubicación en el formato deseado
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0); // Posición en la parte superior
      display.println(ubicacion);
      
      // Imprime la magnitud centrada
      display.setTextSize(2); // Tamaño de fuente más grande
      display.setTextColor(WHITE);
      display.getTextBounds(String(magnitud), 0, 0, &x1, &y1_coord, &w, &h); // Uso de la variable y1_coord
      display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2); // Centra el texto horizontalmente
      display.println(magnitud);

      // Imprime la profundidad justo debajo de la magnitud centrada
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.getTextBounds(String(profundidad), 0, 0, &x1, &y1_coord, &w, &h); // Uso de la variable y1_coord
      display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT + h) / 2); // Centra el texto horizontalmente
      display.println("Deep" + String(profundidad));
      
      // Imprime la fecha local en la parte inferior
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, SCREEN_HEIGHT - 10); // Posición en la parte inferior
      display.println(" " + fechaLocal);
      
      display.display();
    } 
    
    http.end(); // Libera los recursos de la solicitud HTTP
  }

  // Puedes utilizar los datos almacenados aquí
  // Por ejemplo, puedes enviarlos por MQTT, guardarlos en una base de datos, etc.
}


