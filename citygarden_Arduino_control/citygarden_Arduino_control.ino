#include <DHT.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

/* Define GPIO*/

// Temperature & Humidity Sensor
#define DHT_A_PIN 6     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302)
DHT DHT(DHT_A_PIN, DHTTYPE);

// Soil Moisture Sensor
#define SOILMOTURE_A_PIN A0 

// Water level of plant
#define WATERLEVER_MAX 700
#define WATERLEVEL_MIN 400

// LED to notify the bump is running or not
#define LED_PIN 13

// Replay to turn on/off Bump
#define RELAYCONTROL_D_PIN 5

// WiFi ESP8266
#define WiFi_RX_D_PIN 3
#define WiFi_TX_D_PIN 2
SoftwareSerial WiFi(WiFi_RX_D_PIN, WiFi_TX_D_PIN);

// Rain Sensor
#define RAINSENSOR_A_PIN 1

#define RAINSENSOR_MIN 400
#define RAINSENSOR_MAX 700

/* Declare variables */

// Temperature & Humidity Sensor
float airHumidityValue = 0;  //Store Air Humidity value
float airTemperatureValue = 0; //Store Air Temperature value

// Soil Moisture Sensor
int soilMoistureValue = 0; // store value of Soil Moisture sensor

// Bump
int bumpStatus = 0; // status of the Bump:
// 0 : not running
// 1 : running

// Soil Moisture Sensor
int waterLevelStatus = 0; 
// -1 : too much water -> need to decrease the water
// 0 : sufficient water
// 1 : lacking water -> need to increase the water

// Rain Sensor
int rainStatus = 0;
// -1 : Flooding
// 0 : Rainning
// 1 : Not Raining

// Json format
const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(12) + JSON_ARRAY_SIZE(1);
char buffer[JSON_BUFFER_SIZE];

void setupTempAndHumdSensor()
{
    DHT.begin();
}

void setupRelayControl()
{
  pinMode(RELAYCONTROL_D_PIN, OUTPUT);
}

void setupLEDPin()
{
  pinMode(LED_PIN, OUTPUT);
}

int getTempAndHumdValue(void) //Why is the type of return function is int while you declare float value
{

  //Read data and store it to variables airHumidityValue and airTemperatureValue
  airHumidityValue = DHT.readHumidity();
  airTemperatureValue= DHT.readTemperature();

  //Print Temperature and Humidity values to Serial monitor
  Serial.print("Air Humidity: ");
  Serial.print(airHumidityValue);
  Serial.print("% \n");
  Serial.print("Air Temperature: ");
  Serial.print(airTemperatureValue);
  Serial.println(" Celsius");
}

int getSoilMoisureValue(void)
{
  int timeToGetSample = 50;
  long totalSoilMoistureValue = 0;
  for(int counter = 0; counter < timeToGetSample; counter++){
      totalSoilMoistureValue += analogRead(SOILMOTURE_A_PIN);
      delay(timeToGetSample);
  }
  soilMoistureValue = totalSoilMoistureValue / timeToGetSample;
  Serial.print("Value of Soil Moisture Sensor: "); Serial.println(soilMoistureValue);
}

int getRainSensorValue()
{
    int timeToGetSample = 50;
    long totalRainSensorValue = 0;

    for(int counter = 0; counter < timeToGetSample; counter++)
    {
        totalRainSensorValue += analogRead(RAINSENSOR_A_PIN);
        delay(timeToGetSample);
    }

    int rainSensorValue = totalRainSensorValue / timeToGetSample;
    Serial.print("Value of Rain Sensor: "); Serial.println(rainSensorValue);

    if(rainSensorValue > RAINSENSOR_MAX){
        rainStatus = 1;
    }
    else if(rainSensorValue < RAINSENSOR_MIN){
        rainStatus = -1;
    }
    else rainStatus = 0;

    Serial.print("Rain Status: ");
    switch (rainStatus) {
    case -1:
      Serial.println("Flooding!");
      break;
    case 0:
      Serial.println("Raining!");
      break;
    case 1:
      Serial.println("Not Raining!");
    }
}

void wateringPlant()
{
    if(soilMoistureValue > WATERLEVER_MAX)
    {
        digitalWrite(RELAYCONTROL_D_PIN, HIGH);
        digitalWrite(LED_PIN, HIGH);
        bumpStatus = 1;
        waterLevelStatus = 1;
    }
    else if(soilMoistureValue <  WATERLEVEL_MIN)
    {
        digitalWrite(RELAYCONTROL_D_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
        bumpStatus = 0;
        waterLevelStatus = -1;
    }
    else
    {
      digitalWrite(RELAYCONTROL_D_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      bumpStatus = 0;
      waterLevelStatus = 0;
    }

  Serial.print("Water level status: ");
  switch (waterLevelStatus) {
      case (-1) :
        Serial.println("Too much water, need to decrease the water!");    
        break;
      case 0 :
        Serial.println("Sufficient water!");
        break;
      case (1):
        Serial.println("Lacking water, need to increase the water!");
  }

  Serial.print("Bump Status: "); Serial.println(bumpStatus);
}

void setupWiFi() 
{
  WiFi.begin(115200);
}


void sendStatusByWiFi(float airTemperatureValue, float airHumidityValue, int soilMoistureValue, int waterLevelStatus, int bumpStatus, int rainStatus)
{
  StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonObject& d = root.createNestedObject("d");
  d["node"] = "1";
  d["airTemperature"] = airTemperatureValue;
  d["airHumidity"] = airHumidityValue;
  d["soilMoisture"] = soilMoistureValue;
  d["waterLevelStatus"] = waterLevelStatus;
  d["rainStatus"] = rainStatus;
  d["bumpStatus"] = bumpStatus;

  root.printTo(buffer, sizeof(buffer));
  // String sensorsValue = String(buffer);
  Serial.println(buffer);
  WiFi.println(buffer);
}

void setup()
{
  Serial.begin(115200);
  setupWiFi();
  setupTempAndHumdSensor();
  setupRelayControl();
  setupLEDPin();
  setupWiFi();
}

void loop()
{
  getTempAndHumdValue();
  getSoilMoisureValue();
  getRainSensorValue();
  delay(100);
  wateringPlant();
  delay(100);
  sendStatusByWiFi(airTemperatureValue, airHumidityValue, soilMoistureValue, waterLevelStatus, bumpStatus, rainStatus);
  delay(10000);
}
