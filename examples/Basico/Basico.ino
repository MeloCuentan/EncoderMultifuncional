#include <Arduino.h>
#include "EncoderMultifuncional.h"

// Creamos dos variables para obtener los valores que devuelve el encoder
int16_t contador;
int16_t contadorAnterior;

// Creamos objeto del encoder con la dirección que hayamos asignado (0x20 - 0x27)

EncoderMultifuncional encoder(0x20); // Dirección I2C del PCF8574

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
  
  // Si está presinoado el pin, lo mostarmos en el monitor serie
  if (encoder.esPresionado(PIN_A))
  {
    Serial.println(F("Pulsado Pin A"));
  }

  if (encoder.esPresionado(PIN_B))
  {
    Serial.println(F("Pulsado Pin B"));
  }

  if (encoder.esPresionado(PIN_C))
  {
    Serial.println(F("Pulsado Pin C"));
  }

  if (encoder.esPresionado(PIN_D))
  {
    Serial.println(F("Pulsado Pin D"));
  }

  if (encoder.esPresionado(PIN_SW))
  {
    Serial.println(F("Pulsado Pin SW"));
  }
}
