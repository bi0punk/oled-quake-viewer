#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h> // Agregar la librería WiFi para realizar la conexión a internet
#include <HTTPClient.h> // Agregar la librería para realizar peticiones HTTP

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Variable para almacenar el tamaño de fuente actual
uint8_t currentTextSize = 1;
uint8_t centralTextSize = 2; // Tamaño de fuente para el número central

// Función para ajustar el tamaño de fuente según el número
void setTextSize(int number) {
  if (number >= 100) {
    currentTextSize = 1;
  } else if (number >= 10) {
    currentTextSize = 1; // Cambiado a tamaño de fuente 1
  } else {
    currentTextSize = 1; // Cambiado a tamaño de fuente 1
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin(14, 12); // Inicia el bus I2C con pines GPIO 14 (SDA) y 12 (SCL)

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.setTextColor(SSD1306_WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

  // Realizar la petición a la API
  WiFi.begin("nombre_de_tu_red", "contraseña_de_tu_red"); // Cambiar "nombre_de_tu_red" y "contraseña_de_tu_red" por los datos de tu red WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }
  Serial.println("Conectado a la red WiFi");

  HTTPClient http;
  http.begin("https://ejemplo.com/api"); // Cambiar "https://ejemplo.com/api" por la URL de tu API
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload); // Mostrar los datos obtenidos desde la API en el monitor serial
  } else {
    Serial.println("Error al realizar la petición HTTP");
  }
  http.end();
}

void loop() {
  display.clearDisplay();

  // Obtener la fecha y hora actual
  String currentDateTime = "27/03/2024 12:34:56";

  // Dibujar la fecha y hora en la parte superior
  display.setTextSize(currentTextSize);
  display.setCursor(0, 0);
  display.print(currentDateTime);

  // Dibujar la hora en la parte inferior
  display.setCursor(0, SCREEN_HEIGHT - (currentTextSize * 8));
  display.print(currentDateTime);

  // Dibujar un número en el medio
  int middleNumber = 42;

  // Ajustar el tamaño de fuente según el valor del número
  setTextSize(middleNumber);
  display.setTextSize(centralTextSize); // Tamaño de fuente más grande para el número central
  display.setCursor((SCREEN_WIDTH - (6 * centralTextSize * 3)) / 2, (SCREEN_HEIGHT - (centralTextSize * 16)) / 2 + 8); // Posición centrada
  display.print(middleNumber);

  display.display();
  delay(1000); // Actualizar cada segundo
}
