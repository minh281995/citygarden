// Soil Moisture Sensor
#define SOILMOISTURE_D_PIN 4
#define SOILMOTURE_A_PIN 0 

// Water level of plant
#define WATERLEVER_MAX 800
#define WATERLEVEL_MIN 400

int waterLevelStatus = 0; 

void setupSoilMoistureSensor()
{
  pinMode(SOILMOISTURE_D_PIN, INPUT);
}

int getSoilMoistureValue()
{
	int soilMoistureSensorStatus = digitalRead(SOILMOISTURE_D_PIN);
	int timeToGetSample = 10;
	int totalSoilMoistureValue =0;
	for(int counter=0; counter<timeToGetSample; counter++){
	    totalSoilMoistureValue += analogRead(SOILMOTURE_A_PIN);
	    delay(timeToGetSample);
	}
  	int	soilMoistureValue = totalSoilMoistureValue / timeToGetSample;

	// int soilMoistureValue = analogRead(SOILMOTURE_A_PIN);

  	Serial.print("Value of Soil Moisture Sensor: "); Serial.println(soilMoistureValue);

	Serial.print("Digital: "); Serial.println(soilMoistureSensorStatus);
  	Serial.print("Analog: "); Serial.println(soilMoistureValue);


      if(soilMoistureValue > WATERLEVER_MAX)
      {
          waterLevelStatus = 1;
      }
      else if(soilMoistureValue <  WATERLEVEL_MIN)
      {
          waterLevelStatus = -1;
      }
      else
      {
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

}

void setup() {
  // put your setup code here, to run once:
	Serial.begin(115200);
	setupSoilMoistureSensor();
}

void loop() {
  // put your main code here, to run repeatedly:
	getSoilMoistureValue();
	delay(2000);
}
