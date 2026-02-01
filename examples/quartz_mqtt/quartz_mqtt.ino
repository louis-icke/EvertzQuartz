/*
  Allows for inquiring and setting of routes on a single level over MQTT
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <EvertzQuartz.h>

// Wifi
const char* ssid = "*****";
const char* password = "*****";
const String hostname = "router";

// MQTT Broker
const char* mqtt_broker = "****";
const String root_topic = "quartz";
const char* mqtt_username = "****";
const char* mqtt_password = "****";
const int mqtt_port = 1883;

// Router
const int router_size = 16;
const int router_rx = 20;
const int router_tx = 19;
const int refresh_delay = 60;

// Setup clients
WiFiClient espClient;
PubSubClient client(espClient);
EvertzQuartz router = EvertzQuartz(Serial1);//,5000,&Serial);

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname.c_str());
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void getAllRoutes(bool subscribe = false) {
  // Loop through router, interrogating routes and sending to broker
  for (int i=1; i<=router_size; i++) {
    Serial.print("R.DST"+String(i)+"=");
    int source = router.getRoute("A", i);
    Serial.println(source);
    
    // If source is 0, error or no revertive received
    if (source != 0) {
      client.publish(root_topic.c_str(), "connected");
      String topic = root_topic + "/dst" + String(i);
      client.publish(topic.c_str(), String(source).c_str());

      if (subscribe) {
        // Subscribe to set topic
        topic = topic + "/set";
        client.subscribe(topic.c_str());
        Serial.println("Subscribed to " + topic);
      }
    } else {
      Serial.println("Invalid source for destination " + String(i)); 
      client.publish(root_topic.c_str(), "error");
    }
  }
}

void refreshRoutes(void * arg) {
  while (true) {
    delay(refresh_delay*1000);
    getAllRoutes();
  }
}

void setup() {
  // Setup serial connections
  Serial.begin(9600);
  Serial1.begin(38400, SERIAL_8N1, router_rx, router_tx);

  // Conect to network
  initWiFi();

  // Connect to broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = hostname + "-" + String(WiFi.macAddress());
    Serial.println("Connecting to broker as " + client_id);
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected");
    } else {
      Serial.println("Failed with state " + String(client.state()) + " retrying in 2s");
      delay(2000);
    }
  }

  // Get initial state of router and subscribe to inputs
  getAllRoutes(true);

  // Setup refresh routes task
  xTaskCreate(refreshRoutes, "refreshRoutes", 4096, NULL, 2, NULL);
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Extract destination from payload
  String dest = String(topic).substring((root_topic.length()+4), (String(topic).length()-4));

  // Get payload into string
  String message;
  for (int i=0; i<length; i++) {
    message += (char)payload[i];
  }

  // Send command
  Serial.println("S.DST" + dest + "=" + message);
  if (router.setXPT("A", dest.toInt() , message.toInt())) {
    // If valid, send update to main topic
    Serial.println("R.");
    String topic = root_topic + "/dst" + dest;
    client.publish(topic.c_str(), message.c_str());
  } else {
    // If failed, re-run route interroragtion
    Serial.println("Failed to set destination " + dest + " to " + message);
    getAllRoutes();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();
}
