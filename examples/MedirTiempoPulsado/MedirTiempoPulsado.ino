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

  // Con este método, medimos el tiempo que se ha pulsado cada uno
  uint16_t medirTiempoPINA = encoder.medirTiempoPulsado(PIN_A);
  uint16_t medirTiempoPINB = encoder.medirTiempoPulsado(PIN_B);
  uint16_t medirTiempoPINC = encoder.medirTiempoPulsado(PIN_C);
  uint16_t medirTiempoPIND = encoder.medirTiempoPulsado(PIN_D);
  uint16_t medirTiempoPINSW = encoder.medirTiempoPulsado(PIN_SW);

  if (medirTiempoPINA != 0)
  {
    Serial.print(F("PIN A pulsado "));
    Serial.print(medirTiempoPINA);
    Serial.println(F(" ms"));
  }

  if (medirTiempoPINB != 0)
  {
    Serial.print(F("PIN B pulsado "));
    Serial.print(medirTiempoPINB);
    Serial.println(F(" ms"));
  }

  if (medirTiempoPINC != 0)
  {
    Serial.print(F("PIN C pulsado "));
    Serial.print(medirTiempoPINC);
    Serial.println(F(" ms"));
  }

  if (medirTiempoPIND != 0)
  {
    Serial.print(F("PIN D pulsado "));
    Serial.print(medirTiempoPIND);
    Serial.println(F(" ms"));
  }

  if (medirTiempoPINSW != 0)
  {
    Serial.print(F("PIN SW pulsado "));
    Serial.print(medirTiempoPINSW);
    Serial.println(F(" ms"));
  }
}
