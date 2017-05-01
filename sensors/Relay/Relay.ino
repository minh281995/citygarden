// Replay to turn on/off Bump
#define RELAYCONTROL_D_PIN 4


void setup() {
  // put your setup code here, to run once:
	Serial.begin(115200);
	 pinMode(RELAYCONTROL_D_PIN, OUTPUT);
}

void loop() {
	digitalWrite(RELAYCONTROL_D_PIN, HIGH);
	delay(2000);
	digitalWrite(RELAYCONTROL_D_PIN, LOW);
	delay(2000);
  // put your main code here, to run repeatedly:

}
