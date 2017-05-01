/* Flame Sensor analog example.
Code by Reichenstein7 (thejamerson.com)

For use with a Rain Sensor with an analog out!

To test view the output, point a serial monitor such as Putty at your Arduino. 

  - If the Sensor Board is completely soaked; "case 0" will be activated and " Flood " will be sent to the serial monitor.
  - If the Sensor Board has water droplets on it; "case 1" will be activated and " Rain Warning " will be sent to the serial monitor.
  - If the Sensor Board is dry; "case 2" will be activated and " Not Raining " will be sent to the serial monitor. 

*/

// lowest and highest sensor readings:
// const int sensorMin = 0;     // sensor minimum
// const int sensorMax = 1023;  // sensor maximum

// void setup() {
//   // initialize serial communication @ 9600 baud:
//   Serial.begin(9600);  
// }
// void loop() {
//   // read the sensor on analog A0:
//   int sensorReading = analogRead(A0);
//   // map the sensor range (four options):
//   // ex: 'long int map(long int, long int, long int, long int, long int)'
//   int range = map(sensorReading, sensorMin, sensorMax, 0, 3);
//   Serial.println(sensorReading);
//   // range value:
//   switch (range) {
//  case 0:    // Sensor getting wet
//     Serial.println("Flood");
//     break;
//  case 1:    // Sensor getting wet
//     Serial.println("Rain Warning");
//     break;
//  case 2:    // Sensor dry - To shut this up delete the " Serial.println("Not Raining"); " below.
//     Serial.println("Not Raining");
//     break;
//   }
//   delay(1000);  // delay between reads
// }

#define RAINSENSOR_D_PIN 7
#define RAINSENSOR_A_PIN 1

#define RAINSENSOR_MIN 400
#define RAINSENSOR_MAX 800

int rainStatus = 0;

void getRainSensorValue()
{
  int rainSensorStatus = digitalRead(RAINSENSOR_D_PIN);
  int rainSensorRawData = analogRead(RAINSENSOR_A_PIN);

  Serial.print("Digital: "); Serial.println(rainSensorStatus);
  Serial.print("Analog: "); Serial.println(rainSensorRawData);

  // if(rainSensorStatus == 0){
      if(rainSensorRawData > RAINSENSOR_MAX){
          rainStatus = 1;
      }
      else if(rainSensorRawData < RAINSENSOR_MIN){
          rainStatus = -1;
      }
      else rainStatus = 0;
  // }
  // else 
  // {
  //   rainStatus = 1;
  // }

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

void setup()
{
  Serial.begin(115200);
  pinMode(RAINSENSOR_D_PIN, INPUT);
}



void loop()
{
  getRainSensorValue();
  delay(1000);
}





















