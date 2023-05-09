#include <Arduino.h>
#include "EncoderMultifuncional.h"

// Creamos dos variables para obtener los valores que devuelve el encoder
int8_t contador;
int8_t contadorAnterior;

// Creamos objeto del encoder con la dirección que hayamos asignado (0x20 - 0x27)

EncoderMultifuncional encoder(0x20, 0, 10, false); // Dirección I2C del PCF8574, valor mínimo, valor máximo, activamos el bucle

void setup()
{
  Serial.begin(9600); // Iniciamos el monitor serie
  Serial.println(F("INICIO DEL SISTEMA"));
  encoder.inicializar();
}

void loop()
{
  encoder.actualizarBits(); // Esta función tiene que estar en el loop para que se actualice con bastante frecuencia

  contador = encoder.obtenerValor(); // guardamos el valor que devuelve el encoder
  if (contador != contadorAnterior)  // Si ha cambiado el valor, lo imprimirmos en el monitor serie
  {
    contadorAnterior = contador;
    Serial.println(contador);
  }
  uint8_t valorPinA = encoder.detectarFlancos(PIN_A);
  uint8_t valorPinB = encoder.detectarFlancos(PIN_B);
  uint8_t valorPinC = encoder.detectarFlancos(PIN_C);
  uint8_t valorPinD = encoder.detectarFlancos(PIN_D);
  uint8_t valorPinSW = encoder.detectarFlancos(PIN_SW);

  if (valorPinA == PRESIONADO)
  {
    Serial.println("El pulsador A se ha presionado");
  }
  else if (valorPinA == LIBERADO)
  {
    Serial.println("El pulsador A se ha soltado");
  }

  if (valorPinB == PRESIONADO)
  {
    Serial.println("El pulsador B se ha presionado");
  }
  else if (valorPinB == LIBERADO)
  {
    Serial.println("El pulsador B se ha soltado");
  }

  if (valorPinC == PRESIONADO)
  {
    Serial.println("El pulsador C se ha presionado");
  }
  else if (valorPinC == LIBERADO)
  {
    Serial.println("El pulsador C se ha soltado");
  }

  if (valorPinD == PRESIONADO)
  {
    Serial.println("El pulsador D se ha presionado");
  }
  else if (valorPinD == LIBERADO)
  {
    Serial.println("El pulsador D se ha soltado");
  }

  if (valorPinSW == PRESIONADO)
  {
    Serial.println("El pulsador SW se ha presionado");
  }
  else if (valorPinSW == LIBERADO)
  {
    Serial.println("El pulsador SW se ha soltado");
  }
}