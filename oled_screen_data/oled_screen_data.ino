#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

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