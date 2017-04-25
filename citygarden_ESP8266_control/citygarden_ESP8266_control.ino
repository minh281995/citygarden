#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//-------- Customise these values -----------

const char* ssid = "";
const char* password = "";

#define ORG "s2vwfm"
#define DEVICE_TYPE "citygarden"
#define DEVICE_ID ""
#define TOKEN ""

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

const char topic[] = "iot-2/evt/status/fmt/json";

void wifiConnect();
void mqttConnect();
void publishData();

String payload;
String data;

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);

void wifiConnect()
{
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void mqttConnect()
{
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
  }
}

void publishData()
{
 Serial.print("Sending payload: ");
 Serial.println(data);

 if (client.publish(topic, (char*) data.c_str())) {
   Serial.println("OK");
 } else {
   Serial.println("FAIL");
 }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  wifiConnect();
  mqttConnect();
}

void loop()
{
  if (Serial.available() > 0) {
    data = Serial.readString();
    data.trim();
    Serial.print(data);
    publishData();
  }
  if (!client.loop()) {
    mqttConnect();
  }
}
