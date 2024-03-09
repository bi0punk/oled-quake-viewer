#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

#define SCREEN_WIDTH 128 // ancho de pantalla OLED
#define SCREEN_HEIGHT 64 // alto de pantalla OLED
#define OLED_RESET    -1 // Reset no es requerido en algunos modelos

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Datos de la red WiFi
const char* ssid     = "CAPI";
const char* password = "NOAH2016";

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

  // Muestra datos de ejemplo y datos de WiFi
  display.setCursor(0,0);
  display.println(F("Sismologia | Wifi: OK"));
  display.println(F("---------------------"));
  display.println(F("Fecha: "));
  display.println(F("Ub: "));
  display.println(F("Lat:"));
  display.println(F("Lon: "));
  display.println(F("Mag: "));
  display.println(F("Prof:"));

  display.println(WiFi.localIP());
  display.display();
}

void loop() {
  // Puedes agregar más funcionalidad aquí
}