#include <Arduino.h>
#include "EncoderMultifuncional.h"

// Creamos objeto del encoder con la dirección que hayamos asignado (0x20 - 0x27)
EncoderMultifuncional encoder(0x20, 0, 10); // Dirección I2C del PCF8574, valor mínimo, valor máximo

void setup()
{
  Serial.begin(9600); // Iniciamos el monitor serie
  Serial.println(F("INICIO DEL SISTEMA"));
  encoder.inicializar();
}

void loop()
{
  encoder.actualizarBits(); // Esta función tiene que estar en el loop para que se actualice con bastante frecuencia

  if (encoder.limitePulsado(PIN_A, 500, true) == true)
  {
    Serial.println(F("Pasaste 500ms el PIN A"));
  }

  if (encoder.limitePulsado(PIN_B, 250, true) == true)
  {
    Serial.println(F("Pasaste 250ms el PIN B"));
  }

  if (encoder.limitePulsado(PIN_C, 500) == true)
  {
    Serial.println(F("Pasaste 500ms el PIN C"));
  }

  if (encoder.limitePulsado(PIN_D, 2000) == true)
  {
    Serial.println(F("Pasaste 2sg el PIN D"));
  }

  if (encoder.limitePulsado(PIN_SW, 2000) == true)
  {
    Serial.println(F("Pasaste 2sg el PIN SW"));
  }
}
