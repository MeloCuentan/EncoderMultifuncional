#ifndef ENCODERMULTIFUNCIONAL_H
#define ENCODERMULTIFUNCIONAL_H

#include <Arduino.h>
#include <Wire.h>

// Asignación del número de los pines
static const uint8_t PIN_A = 2;  // Pin P2 del PCF8574
static const uint8_t PIN_B = 3;  // Pin P3 del PCF8574
static const uint8_t PIN_C = 4;  // Pin P4 del PCF8574
static const uint8_t PIN_D = 5;  // Pin P5 del PCF8574
static const uint8_t PIN_SW = 6; // Pin P6 del PCF8574
static const uint8_t PRESIONADO = 1;
static const uint8_t LIBERADO = 2;

class EncoderMultifuncional
{
public:
  EncoderMultifuncional(uint8_t direccion_I2C);
  EncoderMultifuncional(uint8_t direccion_I2C, int16_t valorMinimo, int16_t valorMaximo);
  EncoderMultifuncional(uint8_t direccion_I2C, int16_t valorMinimo, int16_t valorMaximo, bool bucle);
  void detectarFlancoBajada(uint8_t pin);
  int8_t detectarFlancos(int8_t pin);
  void inicializar();
  void actualizarBits();
  bool esPresionado(uint8_t pin);
  int16_t obtenerValor();
  void asignarValor(int16_t value);
  void cambiarLimites(int16_t valorMinimo, int16_t valorMaximo);
  void cambiarLimites(int16_t valorMinimo, int16_t valorMaximo, int16_t valorEncoder);

private:
  static const uint8_t _totalPulsadores = 5; // Total de pulsadores que tiene el encoder
  static const uint8_t PULSADO = 1;
  static const uint8_t LIBRE = 2;
  static const uint8_t NULO = 0;
  uint8_t _direccion_I2C;                    // Dirección I2C del PCF8574
  uint8_t _estadoActual;                     // Estado actual de los  botones
  uint8_t _estadoAnterior;                   // Estado anterior de los botones
  int16_t _valorEncoder;                     // Valor numérico del encoder
  uint8_t _bitsActuales;                     // Byte de todas las entradas medidas
  uint8_t _bitsAnteriores;                   // Byte de todas las entradas medidas anteriores
  bool _flancoPines[_totalPulsadores] = {
      // Establecemos la detección de todos los flancos como desactivados por defecto
      false, // PIN_A
      false, // PIN_B
      false, // PIN_C
      false, // PIN_D
      false  // PIN_SW
  };
  uint8_t _estadoPines[_totalPulsadores] = {LIBRE, LIBRE, LIBRE, LIBRE, LIBRE};
  bool _lecturaEncoder;          // Habilita la lectura del encoder hasta que llegue al reposo
  bool _bucle = false;           // Activa el modo bucle del valor numérico del encoder
  bool _suma = true;             // Suma el valor
  bool _resta = false;           // Resta el valor
  int16_t _valorMinimo = -32768; // Valor mínimo de un int16_t
  int16_t _valorMaximo = 32767;  // Valor máximo de un int16_t
  void _cambiarValor(bool accion); // Función que cambia el valor del encoder
  uint8_t _read8();                // Función para leer todos los pines del PCF
};

#endif