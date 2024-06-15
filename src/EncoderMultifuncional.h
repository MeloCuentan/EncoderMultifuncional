#ifndef ENCODERMULTIFUNCIONAL_H
#define ENCODERMULTIFUNCIONAL_H

#include <Arduino.h>
#include <Wire.h>

// Asignación del número de los pines que utilizará el usuario
const uint8_t PIN_C = 2;      // Pin P4 del PCF8574
const uint8_t PIN_D = 3;      // Pin P5 del PCF8574
const uint8_t PIN_A = 4;      // Pin P2 del PCF8574
const uint8_t PIN_B = 5;      // Pin P3 del PCF8574
const uint8_t PIN_SW = 6;     // Pin P6 del PCF8574
const uint8_t PRESIONADO = 1; // Valor que devuelve al pulsar el botón
const uint8_t LIBERADO = 2;   // Valor que devuelve al soltar el botón

class EncoderMultifuncional
{
public:
  EncoderMultifuncional(uint8_t pDireccion_I2C);
  EncoderMultifuncional(uint8_t pDireccion_I2C, int16_t pValorMinimo, int16_t pValorMaximo);
  EncoderMultifuncional(uint8_t pDireccion_I2C, int16_t pValorMinimo, int16_t pValorMaximo, bool pBucle);
  void inicializar(void);
  void actualizarBits(void);
  void detectarFlancoBajada(uint8_t pPin);
  void asignarValor(int16_t pValor);
  void cambiarLimites(int16_t pValorMinimo, int16_t pValorMaximo);
  void cambiarLimites(int16_t pValorMinimo, int16_t pValorMaximo, int16_t pValorEncoder);
  bool esPresionado(uint8_t pPin);
  bool limitePulsado(uint8_t pPin, uint16_t pTiempo, bool pRepeticion = false);
  int8_t detectarFlancos(int8_t pPin);
  int16_t obtenerValor(void);
  uint16_t medirTiempoPulsado(uint8_t pPin);

private:
  static const uint8_t _TOTAL_PULSADORES = 5;        // Total de pulsadores que tiene el encoder
  static const uint8_t _PULSADO = 1;                 // Valor que devuelve al pulsar el botón
  static const uint8_t _LIBRE = 2;                   // Valor que devuelve al soltar el botón
  static const uint8_t _NULO = 0;                    // Valor que devuelve si no hay accción en el botón
  static const uint8_t _VALOR_MINIMO_PIN = 2;        // Número del pin menor del PCF, correspondiente al P2
  static const uint8_t _VALOR_MAXIMO_PIN = PIN_SW;   // Número del pin mayor del PCF, correspondiente al P6
  uint8_t _direccion_I2C;                            // Dirección I2C del PCF8574
  uint8_t _estadoActual;                             // Estado actual de los  botones
  uint8_t _estadoAnterior;                           // Estado anterior de los botones
  uint8_t _bitsActuales;                             // Byte de todas las entradas medidas
  uint8_t _bitsAnteriores;                           // Byte de todas las entradas medidas anteriores
  int16_t _valorEncoder;                             // Valor numérico del encoder
  int16_t _valorMinimo = -32768;                     // Valor mínimo de un int16_t
  int16_t _valorMaximo = 32767;                      // Valor máximo de un int16_t
  uint32_t _presionInicioCentral;                    // Guardamos el tiempo cualdo se pulsa el botón central
  uint32_t _presionInicioCruz;                       // Guardamos el tiempo cualdo se pulsa cualquier botón de la cruz
  uint32_t _tiempoInicioPulsador[_TOTAL_PULSADORES]; // Almacenar tiempos de inicio al pulsar
  bool _botonPresionadoAnterior[_TOTAL_PULSADORES];  // Estado del botón anterior
  bool _botonPresionado[_TOTAL_PULSADORES];          // Estado actual del botón
  bool _flancoPines[_TOTAL_PULSADORES] = {
      // Establecemos la detección de todos los flancos como desactivados por defecto
      false, // PIN_C
      false, // PIN_D
      false, // PIN_A
      false, // PIN_B
      false  // PIN_SW
  };
  bool _reposoEncoder;              // Variable para sabe si el reposo es 00 o 11
  bool _bucle = false;              // Activa el modo bucle del valor numérico del encoder
  bool _suma = true;                // Suma el valor
  bool _resta = false;              // Resta el valor
  void _cambiarValor(bool pAccion); // Función que cambia el valor del encoder
  uint8_t _read8(void);             // Función para leer todos los pines del PCF
};

#endif
