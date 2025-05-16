//www.elegoo.com
//2018.10.25
//Original Code modified by Greenwich Library CT for Farenheit readings. 5.6.2025


#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 3; /* IMPORTANT: This is the numbered digital PWM port on the arduino board
                                      * that the temperature and humidity sensor is plugged into. 
                                      * If you change this to another port on the board make sure to change the value here.
                                      */
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

/*
 * Initialize the serial port.
 */
void setup()
{
  Serial.begin(9600); // Set the baud rate
}

/*
 * Poll for a measurement, keeping the state machine alive. Returns
 * true if a measurement is available.
 */
static bool measure_environment(float *temperature, float *humidity)
{
  static unsigned long measurement_timestamp = millis();

  /* Measure once every four seconds. */
  if (millis() - measurement_timestamp > 3000ul)
  {
    if (dht_sensor.measure(temperature, humidity) == true)
    {
      measurement_timestamp = millis();
      return (true);
    }
  }

  return (false);
}

/*
 * Main program loop.
 */
void loop()
{
  float temperatureC;
  float humidity;

  /* Measure temperature and humidity. If the function returns
     true, then a measurement is available. */
  if (measure_environment(&temperatureC, &humidity) == true)
  {
    float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

    Serial.print("T = ");
    Serial.print(temperatureF, 1);
    Serial.print(" deg. F, H = ");
    Serial.print(humidity, 1);
    Serial.println("%");
  }
}
