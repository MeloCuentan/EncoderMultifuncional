#include <Arduino.h>
#include "EncoderMultifuncional.h"

/**
 * @brief Constructor básico
 *
 * @param pDireccion_I2C Dirección I2C del PCF8574
 */
EncoderMultifuncional::EncoderMultifuncional(uint8_t pDireccion_I2C)
{
  _direccion_I2C = pDireccion_I2C;
}

/**
 * @brief Constructor intermedio
 *
 * @param pDireccion_I2C Dirección I2C del PCF8574
 * @param pValorMinimo Asignamos el valor mínimo que devolverá el encoder
 * @param pValorMaximo Asignamos el valor máximo que devolverá el encoder
 */
EncoderMultifuncional::EncoderMultifuncional(uint8_t pDireccion_I2C, int16_t pValorMinimo, int16_t pValorMaximo)
{
  _direccion_I2C = pDireccion_I2C;
  _valorMinimo = pValorMinimo;
  _valorMaximo = pValorMaximo;
}

/**
 * @brief Constructor completo
 *
 * @param pDireccion_I2C Dirección I2C del PCF8574
 * @param pValorMinimo Asignamos el valor mínimo que devolverá el encoder
 * @param pValorMaximo Asignamos el valor máximo que devolverá el encoder
 * @param pBucle Si es true, activamos el modo bucle de los valores del encoder
 */
EncoderMultifuncional::EncoderMultifuncional(uint8_t pDireccion_I2C, int16_t pValorMinimo, int16_t pValorMaximo, bool pBucle)
{
  _direccion_I2C = pDireccion_I2C;
  _valorMinimo = pValorMinimo;
  _valorMaximo = pValorMaximo;
  _bucle = pBucle;
}

/**
 * @brief Inicializamos el PCF8574
 *
 */
void EncoderMultifuncional::inicializar(void)
{
  Wire.begin();
  Wire.beginTransmission(_direccion_I2C);
  Wire.write(0xFF); // Activamos los PULLUP internos en todos los pines
  Wire.endTransmission();
  _estadoActual = _read8();
  _estadoAnterior = _estadoActual;
  _valorEncoder = 0;
}

/**
 * @brief Función encargada de leer y actualizar los pines del PCF y comprobar el movimiento del encoder
 *
 */
void EncoderMultifuncional::actualizarBits(void)
{
  _estadoActual = _read8();

  uint8_t _bitsActuales = _estadoActual & 0b00000011; // Aplicamos una máscara para quedarnos solo con los dos últimos valores (pines CLK y DT)

  if (_bitsActuales != _bitsAnteriores) // Si no está en reposo
  {
    _bitsAnteriores = _bitsActuales;

    if (_bitsActuales == 0b00000010) // Si se ha actiado el pin CLK
    {
      if (_reposoEncoder == true)
        _cambiarValor(_suma);
      else
        _cambiarValor(_resta);
    }

    if (_bitsActuales == 0b00000001) // Si se ha actiado el pin DT
    {
      if (_reposoEncoder == false)
        _cambiarValor(_suma);
      else
        _cambiarValor(_resta);
    }
  }

  if (_bitsActuales == 0b00000000)
  {
    _reposoEncoder = false;
  }
  else if (_bitsActuales == 0b00000011)
  {
    _reposoEncoder = true;
  }
}

/**
 * @brief Función para activar la detección del flanco de bajada de los pulsadores
 *
 * @param pPin Pasamos el nombre del pin que queremos activar (PIN_A, PIN_B, PIN_C, PIN_D, PIN_SW)
 */
void EncoderMultifuncional::detectarFlancoBajada(uint8_t pPin)
{
  if (pPin >= _VALOR_MINIMO_PIN && pPin <= _VALOR_MAXIMO_PIN)
  {
    uint8_t ajustePin = pPin - 2;
    _flancoPines[ajustePin] = true;
  }
}

/**
 * @brief Asignar un valor al encoder
 *
 * @param pValor Valor que se le asigna
 */
void EncoderMultifuncional::asignarValor(int16_t pValor)
{
  if (pValor > _valorMinimo && pValor < _valorMaximo)
    _valorEncoder = pValor;
}

/**
 * @brief Cambiamos los límites del encoder. Si la posición actual está por encima o por debajo, se establece en los límites asignados
 *
 * @param pValorMinimo Valor mínimo del encoder
 * @param pValorMaximo Valor máximo del encoder
 */
void EncoderMultifuncional::cambiarLimites(int16_t pValorMinimo, int16_t pValorMaximo)
{
  if (pValorMinimo != pValorMaximo && pValorMinimo < pValorMaximo)
  {
    _valorMinimo = pValorMinimo;
    _valorMaximo = pValorMaximo;

    if (_valorEncoder > _valorMaximo)
      _valorEncoder = _valorMaximo;

    if (_valorEncoder < _valorMinimo)
      _valorEncoder = _valorMinimo;
  }
}

/**
 * @brief Cambiamos los límites del encoder y asignamos el valor que queramos posicionar. Este solo será cambiado si está dentro de los limites
 *
 * @param pValorMinimo Valor mínimo del encoder
 * @param pValorMaximo Valor máximo del encoder
 * @param pValorEncoder Asignamos un valor al encoder. Si este valor no está dentro de los límites, se establece en el valor mínimo asignado
 */
void EncoderMultifuncional::cambiarLimites(int16_t pValorMinimo, int16_t pValorMaximo, int16_t pValorEncoder)
{
  if (pValorMinimo != pValorMaximo && pValorMinimo < pValorMaximo)
  {
    _valorMinimo = pValorMinimo;
    _valorMaximo = pValorMaximo;

    if (pValorEncoder > _valorMinimo && pValorEncoder < _valorMaximo)
    {
      _valorEncoder = pValorEncoder;
    }
    else
    {
      _valorEncoder = _valorMinimo;
    }
  }
}

/**
 * @brief Comprobar el estado del pulsador
 *
 * @param pPin Pasamos el nombre del pin que queremos activar (PIN_A, PIN_B, PIN_C, PIN_D, PIN_SW)
 */
bool EncoderMultifuncional::esPresionado(uint8_t pPin)
{
  if (pPin >= _VALOR_MINIMO_PIN && pPin <= _VALOR_MAXIMO_PIN)
  {
    uint8_t ajustePin = pPin - 2;

    if (_flancoPines[ajustePin] == false) // si no está activada la detección del flanco de ese pin, se devuelve el valor mientras esté pulsado
    {
      return !(_estadoActual & (1 << pPin));
    }

    uint8_t mascara = (1 << pPin);                     // Creamos la máscara para filtrar el bit que queremos utilizar
    uint8_t estadoPinActual = _estadoActual & mascara; // Aplicamos la máscara y guardamos en una nueva variable
    uint8_t estadoPinAnterior = _estadoAnterior & mascara;

    if (estadoPinActual != estadoPinAnterior) // Si son diferentes
    {
      _estadoAnterior = _estadoActual;       // Actualizamos el valor
      return !(_estadoActual & (1 << pPin)); // Devolvemos el valor
    }
  }

  return false; // En caso de que la condición del if no se cumpla, retornamos false porque no se ha pulsado
}

/**
 * @brief Devuelve true si ha pasado el tiempo pulsado en ese pin
 *
 * @param pPin Valor del pin (PIN_A, PIN_B, PIN_C, PIN_D, PIN_SW)
 * @param pTiempo Tiempo de detección presionado
 * @param pRepeticion Si es true, se repite la pulsación cada pTiempo. Si es false, no repite
 */
bool EncoderMultifuncional::limitePulsado(uint8_t pPin, uint16_t pTiempo, bool pRepeticion)
{
  static bool _inicioBotonCruz;
  static bool _inicioBotonSW;
  uint8_t _ajustePin = pPin - 2;
  _botonPresionado[_ajustePin] = esPresionado(pPin);

  if (_botonPresionado[_ajustePin] == true && _botonPresionadoAnterior[_ajustePin] == false)
  {
    if (_botonPresionado[_ajustePin] == true)
    {
      _tiempoInicioPulsador[_ajustePin] = millis();

      if (pPin == PIN_SW)
        _inicioBotonSW = true;
      else
        _inicioBotonCruz = true;
    }
  }

  _botonPresionadoAnterior[_ajustePin] = _botonPresionado[_ajustePin];

  if (millis() - _tiempoInicioPulsador[_ajustePin] >= pTiempo && _botonPresionado[_ajustePin] == true)
  {
    _tiempoInicioPulsador[_ajustePin] = millis();
    if (pRepeticion == false)
    {
      if (pPin == PIN_SW && _inicioBotonSW == true)
      {
        _inicioBotonSW = false;
        return true;
      }
      else if (pPin != PIN_SW && _inicioBotonCruz == true)
      {
        _inicioBotonCruz = false;
        return true;
      }
    }
    else
    {
      return true;
    }
  }

  return false;
}

/**
 * @brief Detección de cambio de flanco de cada pulsador
 *
 * @param pPin Se le pasará el nombre de pin (PIN_A, PIN_B, PIN_C, PIN_D, PIN_SW)
 * @return int8_t Devuelve el cambio de pin o NULO si no ha cambiado
 */
int8_t EncoderMultifuncional::detectarFlancos(int8_t pPin)
{
  if (pPin >= _VALOR_MINIMO_PIN && pPin <= _VALOR_MAXIMO_PIN)
  {
    uint8_t mascara = (1 << pPin);                     // Creamos la máscara para filtrar el bit que queremos utilizar
    uint8_t estadoPinActual = _estadoActual & mascara; // Aplicamos la máscara y guardamos en una nueva variable
    uint8_t estadoPinAnterior = _estadoAnterior & mascara;

    if (estadoPinActual != estadoPinAnterior) // Si son diferentes
    {
      _estadoAnterior = _estadoActual; // Actualizamos el valor

      if (!(_estadoActual & (1 << pPin)) == true)
      {
        return _PULSADO;
      }
      else
      {
        return _LIBRE;
      }
    }
  }

  return _NULO;
}

/**
 * @brief Comprueba si hay cambio y actualiza el último valor almacenado
 *
 * @return int16_t Delvuelve el valor del contador
 */
int16_t EncoderMultifuncional::obtenerValor(void)
{
  return _valorEncoder;
}

/**
 * @brief Medir el tiempo que se ha pulsado cualquier botón
 *
 * @param pPin Valor del pin (PIN_A, PIN_B, PIN_C, PIN_D, PIN_SW)
 */
uint16_t EncoderMultifuncional::medirTiempoPulsado(uint8_t pPin)
{
  uint16_t _valorTemporal = 0;

  if (pPin >= _VALOR_MINIMO_PIN && pPin <= _VALOR_MAXIMO_PIN)
  {
    uint8_t _estadoPinActual = detectarFlancos(pPin);

    if (_estadoPinActual == _PULSADO)
    {
      (pPin == PIN_SW) ? (_presionInicioCentral = millis()) : (_presionInicioCruz = millis());
    }
    else if (_estadoPinActual == _LIBRE)
    {
      (pPin == PIN_SW) ? (_valorTemporal = millis() - _presionInicioCentral) : (_valorTemporal = millis() - _presionInicioCruz);
    }
  }

  return _valorTemporal;
}

/**
 * @brief Cambiaremos el valor según el movimiento del encoder
 *
 * @param pAccion Si es true, se suma el valor. Si es false, se resta
 */
void EncoderMultifuncional::_cambiarValor(bool pAccion)
{
  if (_bucle == true)
  {
    if (pAccion == _suma)
    {
      if (_valorEncoder < _valorMaximo)
      {
        _valorEncoder++;
      }
      else
      {
        _valorEncoder = _valorMinimo;
      }
    }

    if (pAccion == _resta)
    {
      if (_valorEncoder > _valorMinimo)
      {
        _valorEncoder--;
      }
      else
      {
        _valorEncoder = _valorMaximo;
      }
    }
  }
  else if (_bucle == false)
  {
    if (pAccion == _suma)
    {
      if (_valorEncoder < _valorMaximo)
        _valorEncoder++;
    }
    if (pAccion == _resta)
    {
      if (_valorEncoder > _valorMinimo)
        _valorEncoder--;
    }
  }
}

/**
 * @brief Leemos los pines del PCF8574 y los guardamos en un byte
 *
 * @return uint8_t valor en BIN del estado de los pines
 */
uint8_t EncoderMultifuncional::_read8(void)
{
  Wire.requestFrom(_direccion_I2C, (uint8_t)1);
  uint8_t value = Wire.read();

  return value;
}