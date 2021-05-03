#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "CAPPONI";
const char* password = "clau1963";
int ledRojo = 13;
const char* mqtt_server= "node02.myqtthub.com";
const int mqtt_port = 1883;
const char *root_topic_subscribe = "esp/led";
const char *root_topic_publish = "esp/test";
WiFiClient espClient;
PubSubClient client(espClient);


void callback(char* topic, byte* payload, unsigned int length) {
	String incoming = "";
	Serial.print("Mensaje recibido desde -> ");
	Serial.print(topic);
	Serial.println("");
	for (int i = 0; i < length; i++) {
		incoming += (char)payload[i];
	}
	incoming.trim();
	Serial.println("Mensaje -> " + incoming);

	if (String(topic).equals("esp/led")) {    
		if(incoming.equals("on")){
		digitalWrite(ledRojo, HIGH);
		}
		else if(incoming.equals("off")){
		digitalWrite(ledRojo, LOW);
		}
	}
}

void reconnect() {
	while (!client.connected()) {
		Serial.print("Intentando conexión Mqtt...");
		// Intentamos conectar
		if (client.connect("lucas.capponi@gmail.com", "lucas_vehiculo","vehiculo") ){
			Serial.println("Conectado!");
			// Nos suscribimos
			if(client.subscribe(root_topic_subscribe)){
				Serial.println("Suscripcion ok");
			}else{
				Serial.println("fallo Suscripciión");
			}
		} else {
			Serial.print("falló :( con error -> ");
			Serial.print(client.state());
			Serial.println(" Intentamos de nuevo en 5 segundos");
			delay(5000);
		}
	}
}

void setup_wifi(){
	delay(10);
	// Nos conectamos a nuestra red Wifi
	Serial.println();
	Serial.print("Conectando a ssid: ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("Conectado a red WiFi!");
	Serial.println("Dirección IP: ");
	Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  pinMode(ledRojo, OUTPUT); 

}

void loop() {
	if (!client.connected()) {
    	reconnect();
  	}

  	//if (client.connected()){
    //	client.publish(root_topic_publish,"Probando...");
    //	delay(300);
  	//}
	  
	client.loop();
}

