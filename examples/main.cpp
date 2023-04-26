/*
  La biblioteca funciona bien de momento.
  Falta agregar a los constructores, si se quiere que se detecte o no los botones en el flanco de bajada
*/

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "EncoderMultifuncional.h"

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Estos valores son de prueba. Creamos dos límites inferiores y superiores, y un valor para asignarlo y probar los nuevos métodos
int16_t valorMinimo0 = -10;
int16_t valorMaximo0 = 10;

int16_t valorMinimo1 = -100;
int16_t valorMaximo1 = 100;

int16_t valorAsignado = 5;

// Estos valores se usarán para comprobar si ha cambiado el valor
int16_t contador;
int16_t contadorAnterior;

uint8_t escalaNumero = 2;
uint8_t escalaLetras = 2;

unsigned long tiempoRefresco;
unsigned long intervaloRefresco = 100;

bool pinActivo[5];

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);
EncoderMultifuncional encoder(0x20); // Dirección I2C del PCF8574
//  EncoderMultifuncional encoder(0x20, 0, 10);  // Dirección I2C del PCF8574, valor mínimo y valor máximo del encocer (-32768 -> 32767)
//  EncoderMultifuncional encoder(0x20, 0, 10, true);  // Dirección I2C del PCF8574, valor mínimo y máximo del encoder (-32768 -> 32767), modo bucle (true es activado)

void OLED();
void posicion(int16_t valor);

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  Serial.println(F("INICIO"));

  encoder.inicializar(); // Inicializa la comunicación I2C y configura los pines como entradas con pull-up interno

  display.setTextColor(1, 0);
  display.setTextSize(2);
  display.setTextWrap(false);
  display.setRotation(2);
  display.clearDisplay();

  // Llamamos al método y le pasamos el pin que queremos que detecte el flanco de bajada
  // encoder.detectarFlancoBajada(PIN_A);
  // encoder.detectarFlancoBajada(PIN_B);
  // encoder.detectarFlancoBajada(PIN_C);
  // encoder.detectarFlancoBajada(PIN_D);
  // encoder.detectarFlancoBajada(PIN_SW);

  //  encoder.asignarValor(0); // Asignamos un valor al encoder
}

void loop()
{
  OLED();

  encoder.actualizarBits(); // Actualiza el estado de los pines

  contador = encoder.obtenerValor(); // guardamos el valor en la variable
  if (contador != contadorAnterior)
  {
    contadorAnterior = contador;

    Serial.println(contador);
  }

  // Estas líneas, como son repetitivas, las pongo dentro de un for
  // estadoPin[0] = encoder.detectarFlancos(PIN_A);
  // estadoPin[1] = encoder.detectarFlancos(PIN_B);
  // estadoPin[2] = encoder.detectarFlancos(PIN_C);
  // estadoPin[3] = encoder.detectarFlancos(PIN_D);
  // estadoPin[4] = encoder.detectarFlancos(PIN_SW);

  for (int i = 0; i < 5; i++)
  {
    static uint8_t estadoPin[5];                                          // Creamos los estados que tendrá cada pin
    String nombrePin[5] = {"Pin A", "Pin B", "Pin C", "Pin D", "Pin SW"}; // Nombres que se mostrarán en el monitor serie
    static const uint8_t pines[5] = {PIN_A, PIN_B, PIN_C, PIN_D, PIN_SW}; // Array con los pines
    estadoPin[i] = encoder.detectarFlancos(pines[i]);                     // Guardamos el estado de cada pin en el array estadoPin
    if (estadoPin[i] == PRESIONADO)                                       // Si pulsamos
    {
      pinActivo[i] = true;                       // guardamos este valor para mostarlo en el OLED
      Serial.println("PULSADO " + nombrePin[i]); // Mostramos pin pulsado en la posición nombrePin[i]
    }
    else if (estadoPin[i] == LIBERADO) // Si soltamos
    {
      pinActivo[i] = false;                     // Guardamos este vlor para mostrarlo en el OLED
      Serial.println("SUELTO " + nombrePin[i]); // Mostramos pin suelto en la posición nombrePin[i]
    }
  }

  /*
    // Comprobamos el estado de los pulsadores
    if (encoder.esPresionado(PIN_A))
    {
      //encoder.cambiarLimites(valorMinimo0, valorMaximo0); // Cambiamos los límites a valorMinimo0 y valorMaximo0
      Serial.println(F("El botón A está pulsado."));
    }
    else if (encoder.esPresionado(PIN_B))
    {
      //encoder.cambiarLimites(valorMinimo1, valorMaximo1); // Cambiamos los límites a valorMinimo1 y valorMaximo1
      Serial.println(F("El botón B está pulsado."));
    }
    else if (encoder.esPresionado(PIN_C))
    {
      //encoder.cambiarLimites(valorMinimo1, valorMaximo1, valorAsignado); // Cambiamos los límites a valorMinimo1 y valorMinimo1 y le asignamos un valor de posición
      Serial.println(F("El botón C está pulsado."));
    }
    else if (encoder.esPresionado(PIN_D))
    {
      Serial.println(F("El botón D está pulsado."));
    }
    else if (encoder.esPresionado(PIN_SW))
    {
      Serial.println(F("El botón SW está pulsado."));
    }
    */
}

String espacios = "";
String nombrePinOled[5] = {"A ", "B ", "C ", "D ", "SW"};

void OLED()
{
  if (millis() - tiempoRefresco >= intervaloRefresco)
  {
    tiempoRefresco = millis();
    display.clearDisplay();
    display.setTextColor(1, 0);
    display.setTextSize(escalaNumero);
    display.setCursor(40, 0);
    display.print(contador);

    display.setTextSize(escalaLetras);
    display.setCursor(0, 16);
    for (int i = 0; i < 5; i++)
    {
      display.setTextColor(pinActivo[i], 0);
      display.print(nombrePinOled[i]);
    }
    display.display();
  }
}