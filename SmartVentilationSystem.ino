#include <dht_nonblocking.h>
#include <Servo.h>

// Define DHT sensor settings
#define DHT_SENSOR_TYPE DHT_TYPE_11
static const int DHT_SENSOR_PIN = 3; // Digital PWM port for temperature/humidity sensor
#define LED_PIN 7                    // LED connected to pin 7

// Create objects
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
Servo myservo; // Create a servo object

// Define temperature threshold (in Fahrenheit)
const float TEMP_THRESHOLD = 72.0;

// Track servo position to avoid unnecessary movements
int currentServoPos = 0;
bool ledState = false;  // Track LED state

/*
 * Initialize the serial port and servo.
 */
void setup()
{
  Serial.begin(9600); // Set the baud rate
  myservo.attach(9);  // Servo is connected to pin 9
  myservo.write(0);   // Initialize servo at 0 degrees
  pinMode(LED_PIN, OUTPUT); // Set LED pin as output
  digitalWrite(LED_PIN, LOW); // Ensure LED starts off
  
  Serial.println("DHT11 and Servo Control System");
  Serial.print("If temperature > ");
  Serial.print(TEMP_THRESHOLD, 1);
  Serial.println("°F, Standby.... OPENING VENT to release heat!");
  Serial.print("If temperature <= ");
  Serial.print(TEMP_THRESHOLD, 1);
  Serial.println("°F, Standby.... CLOSING VENT");
  Serial.println("LED on pin 7 will illuminate when temperature exceeds threshold");
}

/*
 * Poll for a measurement, keeping the state machine alive. Returns
 * true if a measurement is available.
 */
static bool measure_environment(float *temperature, float *humidity)
{
  static unsigned long measurement_timestamp = millis();
  /* Measure once every three seconds. */
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
    
    // Print temperature and humidity values
    Serial.print("T = ");
    Serial.print(temperatureF, 1);
    Serial.print(" deg. F, H = ");
    Serial.print(humidity, 1);
    Serial.println("%");
    
    // Control servo and LED based on temperature
    if (temperatureF > TEMP_THRESHOLD)
    {
      // Handle servo position
      if (currentServoPos != 90) {
        Serial.print("Temperature above ");
        Serial.print(TEMP_THRESHOLD, 1);
        Serial.println("°F! Standby.... OPENING VENT to release heat!");
        myservo.write(90);
        currentServoPos = 90;
      }
      
      // Turn on LED if it's not already on
      if (!ledState) {
        digitalWrite(LED_PIN, HIGH);
        ledState = true;
        Serial.println("LED alert activated!");
      }
    } 
    else // temperatureF <= TEMP_THRESHOLD
    {
      // Handle servo position
      if (currentServoPos != 0) {
        Serial.print("Temperature at or below ");
        Serial.print(TEMP_THRESHOLD, 1);
        Serial.println("°F! Standby.... CLOSING VENT");
        myservo.write(0);
        currentServoPos = 0;
      }
      
      // Turn off LED if it's on
      if (ledState) {
        digitalWrite(LED_PIN, LOW);
        ledState = false;
        Serial.println("LED alert deactivated");
      }
    }
  }
}