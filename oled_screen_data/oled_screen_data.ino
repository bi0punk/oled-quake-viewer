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

const char* ssid     = "";
const char* password = "";

const char* serverUrl = "";

unsigned long lastRequestTime = 0;
const unsigned long requestInterval = 300000; // Intervalo de solicitud en milisegundos

WiFiClient client;
HTTPClient http;

String fechaLocal;
String ubicacion;
float magnitud;
float profundidad;
String latitud; // Variable para almacenar la latitud
String longitud; // Variable para almacenar la longitud

int16_t x1, y1_coord; // Cambio de nombre de la variable y1 a y1_coord
uint16_t w, h;

bool isDataSwapped = false; // Variable para controlar el intercambio de datos

// Función para reemplazar la primera ocurrencia de una subcadena en una cadena
void replaceFirst(String& original, const String& search, const String& replace) {
    int pos = original.indexOf(search); // Encuentra la posición de la primera ocurrencia de la subcadena
    if (pos != -1) {
        original = original.substring(0, pos) + replace + original.substring(pos + search.length());
    }
}

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
      magnitud = doc["magnitud"].as<float>();
      profundidad = doc["profundidad"].as<float>();
      latitud = doc["latitud"].as<String>(); // Obtén el valor de la latitud
      longitud = doc["longitud"].as<String>(); // Obtén el valor de la longitud

      // Reemplazar caracteres acentuados
      ubicacion.replace("ü", "\u00FC");

      ubicacion.replace(" al ", " - "); 
      ubicacion.replace(" km ", " Km "); 

      // Modificar la cadena de ubicación
      replaceFirst(ubicacion, " de ", " - "); // Reemplaza la primera ocurrencia de " de " por " | "

      // Imprimir la fecha local en la parte superior
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0); // Posición en la parte superior
      display.println(" " + fechaLocal);
      
      // Imprime la magnitud centrada
      display.setTextSize(2); // Tamaño de fuente más grande
      display.setTextColor(WHITE);
      display.getTextBounds(String(magnitud), 0, 0, &x1, &y1_coord, &w, &h); // Uso de la variable y1_coord
      display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h * 2) / 2 + 8); // Centra el texto horizontalmente y baja una línea
      display.println(magnitud);

      // Imprime "Deep" centrado debajo del dígito central
      display.setTextSize(1);
      display.setTextColor(WHITE);
      String depthText = "Deep: " + String(profundidad) + " km";
      display.getTextBounds(depthText, 0, 0, &x1, &y1_coord, &w, &h); // Obtener el ancho total del texto
      int16_t depthTextWidth = w; // Ancho del texto "Deep"
      int16_t depthTextHeight = h; // Alto del texto "Deep"
      int16_t depthTextX = (SCREEN_WIDTH - depthTextWidth) / 2; // Posición X para centrar horizontalmente
      int16_t depthTextY = (SCREEN_HEIGHT + h * 3) / 2; // Posición Y para colocar debajo del dígito central
      display.setCursor(depthTextX, depthTextY); // Establece el cursor en la posición calculada
      display.println(depthText);

      // Imprime el valor de la latitud en la parte superior
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, SCREEN_HEIGHT / 2 - 16); // Posición Y para el valor de la latitud una línea arriba
      display.println(latitud);

      // Imprime la etiqueta "Lat:" en la parte inferior
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, SCREEN_HEIGHT / 2 - 8); // Posición Y para la etiqueta "Lat:" una línea abajo
      display.println(" Lat");

      // Imprime el valor de la longitud en la parte superior
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, SCREEN_HEIGHT / 2 + 8); // Posición Y para el valor de la longitud una línea abajo de la latitud
      display.println(longitud);

      // Imprime la etiqueta "Long:" en la parte inferior
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, SCREEN_HEIGHT / 2 + 16); // Posición Y para la etiqueta "Long:" una línea abajo
      display.println(" Long");

      // Imprime la ubicación en la parte inferior con scroll
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, SCREEN_HEIGHT - 10); // Posición en la parte inferior
      display.startscrollleft(0x00, 0x0F); // Inicia el scroll hacia la izquierda
      display.println(ubicacion);
      display.stopscroll(); // Detiene el scroll
      
      display.display();

      // Realiza el intercambio de datos si corresponde
      if (!isDataSwapped) {
        isDataSwapped = true; // Marca que se ha realizado el intercambio de datos
        // Mueve los datos de la página 0 a la página 7
        display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK); // Borra la pantalla
        display.setCursor(0, 0); // Posición en la parte superior
        display.println(" "); // Espacio para fecha local
        display.setTextSize(2); // Tamaño de fuente más grande
        display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h * 2) / 2 + 8); // Centra el texto horizontalmente y baja una línea
        display.println(" "); // Espacio para magnitud
        display.setTextSize(1); // Tamaño de fuente normal
        display.setCursor(depthTextX, depthTextY); // Establece el cursor en la posición calculada
        display.println(" "); // Espacio para profundidad
        display.setCursor(0, SCREEN_HEIGHT - 10); // Posición en la parte inferior
        display.println(" "); // Espacio para ubicación
      } else {
        isDataSwapped = false; // Marca que se ha restablecido el estado original
        // Mueve los datos de la página 7 a la página 0
        // Los datos se imprimen normalmente arriba
      }
    } 
    
    http.end(); // Libera los recursos de la solicitud HTTP
  }

  // Puedes utilizar los datos almacenados aquí
  // Por ejemplo, puedes enviarlos por MQTT, guardarlos en una base de datos, etc.
}
