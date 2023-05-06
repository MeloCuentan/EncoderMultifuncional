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
void EncoderMultifuncional::inicializar()
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
 * @brief Función para activar la detección del flanco de bajada de los pulsadores
 *
 * @param pin Pasamos el nombre del pin que queremos activar (PIN_A, PIN_B, PIN_C, PIN_D, PIN_SW)
 */
void EncoderMultifuncional::detectarFlancoBajada(uint8_t pin)
{
  if (pin >= _VALOR_MINIMO_PIN && pin <= _VALOR_MAXIMO_PIN)
  {
    uint8_t ajustePin = pin - 2;
    _flancoPines[ajustePin] = true;
  }
}

/**
 * @brief Comprobar el estado del pulsador
 *
 * @param pin Número del pin a comprobar
 */
bool EncoderMultifuncional::esPresionado(uint8_t pin)
{
  if (pin >= _VALOR_MINIMO_PIN && pin <= _VALOR_MAXIMO_PIN)
  {
    uint8_t ajustePin = pin - 2;

    if (_flancoPines[ajustePin] == false) // si no está activada la detección del flanco de ese pin, se devuelve el valor mientras esté pulsado
    {
      return !(_estadoActual & (1 << pin));
    }

    uint8_t mascara = (1 << pin);                      // Creamos la máscara para filtrar el bit que queremos utilizar
    uint8_t estadoPinActual = _estadoActual & mascara; // Aplicamos la máscara y guardamos en una nueva variable
    uint8_t estadoPinAnterior = _estadoAnterior & mascara;

    if (estadoPinActual != estadoPinAnterior) // Si son diferentes
    {
      _estadoAnterior = _estadoActual;      // Actualizamos el valor
      return !(_estadoActual & (1 << pin)); // Devolvemos el valor
    }
  }

  return false; // En caso de que la condición del if no se cumpla, retornamos false porque no se ha pulsado
}

/**
 * @brief Detección de cambio de flanco de cada pulsador
 *
 * @param pin Se le pasará el número de pin, entendiendo que PIN_A = 0, PIN_B = 1, PIN_C = 2, PIN_D = 3, PIN_SW = 4
 * @return int8_t Devuelve el cambio de pin o NULO sin o ha cambiado
 */
int8_t EncoderMultifuncional::detectarFlancos(int8_t pin)
{
  if (pin >= _VALOR_MINIMO_PIN && pin <= _VALOR_MAXIMO_PIN)
  {
    uint8_t mascara = (1 << pin);                      // Creamos la máscara para filtrar el bit que queremos utilizar
    uint8_t estadoPinActual = _estadoActual & mascara; // Aplicamos la máscara y guardamos en una nueva variable
    uint8_t estadoPinAnterior = _estadoAnterior & mascara;

    if (estadoPinActual != estadoPinAnterior) // Si son diferentes
    {
      _estadoAnterior = _estadoActual; // Actualizamos el valor

      if (!(_estadoActual & (1 << pin)) == true)
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
 * @brief Asignar un valor al encoder
 *
 * @param value Valor que se le asigna
 */
void EncoderMultifuncional::asignarValor(int16_t value)
{
  if (value > _valorMinimo && value < _valorMaximo)
    _valorEncoder = value;
}

/**
 * @brief Cambiamos los límites del encoder. Si la posición actual está por encima o por debajo, se establece en los límites asignados
 *
 * @param valorMinimo Valor mínimo del encoder
 * @param valorMaximo Valor máximo del encoder
 */
void EncoderMultifuncional::cambiarLimites(int16_t valorMinimo, int16_t valorMaximo)
{
  if (valorMinimo != valorMaximo && valorMinimo < valorMaximo)
  {
    _valorMinimo = valorMinimo;
    _valorMaximo = valorMaximo;

    if (_valorEncoder > _valorMaximo)
      _valorEncoder = _valorMaximo;

    if (_valorEncoder < _valorMinimo)
      _valorEncoder = _valorMinimo;
  }
}

/**
 * @brief Cambiamos los límites del encoder y asignamos el valor que queramos del encoder. Este solo será cambiado si está dentro de los limites
 *
 * @param valorMinimo Valor mínimo del encoder
 * @param valorMaximo Valor máximo del encoder
 * @param valorEncoder Asignamos un valor al encoder. Si esta valor no está dentro de los límites, se establece en el valor mínimo asignado
 */
void EncoderMultifuncional::cambiarLimites(int16_t valorMinimo, int16_t valorMaximo, int16_t valorEncoder)
{
  if (valorMinimo != valorMaximo && valorMinimo < valorMaximo)
  {
    _valorMinimo = valorMinimo;
    _valorMaximo = valorMaximo;

    if (valorEncoder > _valorMinimo && valorEncoder < _valorMaximo)
    {
      _valorEncoder = valorEncoder;
    }
    else
    {
      _valorEncoder = _valorMinimo;
    }
  }
}

/**
 * @brief Comprueba si hay cambio y actualiza el último valor almacenado
 *
 * NOTE: Comprobar esta función. Las variables _contador y _contadorAnterior no tienen asignación
 * @return int16_t Delvuelve el valor del contador
 */
int16_t EncoderMultifuncional::obtenerValor()
{
  return _valorEncoder;
}

/**
 * @brief Leemos los pines del PCF8574 y los guardamos en un byte
 *
 * @return uint8_t valor en BIN del estado de los pines
 */
uint8_t EncoderMultifuncional::_read8()
{
  Wire.requestFrom(_direccion_I2C, (uint8_t)1);
  uint8_t value = Wire.read();

  return value;
}

/**
 * @brief Cambiaremos el valor según el movimiento del encoder
 *
 * NOTE: Función en prueba. La función anterior está comentada justo debajo
 * @param accion Si es true, se suma el valor
 */
void EncoderMultifuncional::_cambiarValor(bool accion)
{
  if (_bucle == true)
  {
    if (accion == _suma)
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

    if (accion == _resta)
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
    if (accion == _suma)
    {
      if (_valorEncoder < _valorMaximo)
        _valorEncoder++;
    }
    if (accion == _resta)
    {
      if (_valorEncoder > _valorMinimo)
        _valorEncoder--;
    }
  }
}

/**
 * @brief Función encargada de leer y actualizar los pines del PCF y comprobar el movimiento del encoder
 *
 */
void EncoderMultifuncional::actualizarBits()
{
  _estadoActual = _read8();
  uint8_t _bitsActuales = _estadoActual & 0b00000011; // Aplicamos una máscara para quedarnos solo con los dos últimos valores (pines CLK y DT)

  if (_bitsActuales != 0b00000011 && _lecturaEncoder == true) // Si no está en reposo
  {
    _lecturaEncoder = false;

    if (_bitsActuales != _bitsAnteriores)
    {
      if (_bitsActuales == 0b00000010) // Si se ha actiado el pin DT
      {
        _cambiarValor(_suma); // Sumamos el valor
      }
      else
      {
        _cambiarValor(_resta); // Restamos el valor
      }
    }
  }

  if (_bitsActuales == 0b00000011) // Cuando llegue al reposo
  {
    _lecturaEncoder = true;
    _bitsAnteriores = _bitsActuales;
  }
}
