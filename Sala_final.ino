#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define wifi_ssid "ChicoTea"
#define wifi_password "ningu&tea"

#define mqtt_server "iot.eclipse.org"
#define mqtt_user "khayan2"
#define mqtt_password "khay120"

#define TOPICO_SUBSCRIBE "MQTTkhayEnvia"     //LED
#define TOPICO_PUBLISH   "MQTTkhayRecebe"

#define humidity_topic "sensor/humidity"   //Sensor
#define temperature_topic "sensor/temperature"

#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15
#define D9    3
#define D10   1

#define DHTPIN D2
#define DHTTYPE DHT11 


 DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);
char EstadoSaida = '0';
void InitOutput(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setCallback(mqtt_callback);  
  client.setServer(mqtt_server, 1883);
   dht.begin();
   InitOutput();
  }

  
void EnviaEstadoOutputMQTT(void)
{
    if (EstadoSaida == '0')
      client.publish(TOPICO_PUBLISH, "D");

    if (EstadoSaida == '1')
      client.publish(TOPICO_PUBLISH, "L");

    Serial.println("- Estado da saida D0 enviado ao broker!");
    delay(500);
}
 
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool checkBound(float newValue) {
  return !isnan(newValue) ;
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
 
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
  
    //toma ação dependendo da string recebida:
    //verifica se deve colocar nivel alto de tensão na saída D0:
    //IMPORTANTE: o Led já contido na placa é acionado com lógica invertida (ou seja,
    //enviar HIGH para o output faz o Led apagar / enviar LOW faz o Led acender)
    if (msg.equals("L"))
    {
        digitalWrite(D7, HIGH);
        EstadoSaida = '1';
    }
 
    //verifica se deve colocar nivel alto de tensão na saída D0:
    if (msg.equals("D"))
    {
        digitalWrite(D7, LOW);
        EstadoSaida = '0';
    }
    
}

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
int i = 0;

void InitOutput(void)
{
    //IMPORTANTE: o Led já contido na placa é acionado com lógica invertida (ou seja,
    //enviar HIGH para o output faz o Led apagar / enviar LOW faz o Led acender)
    pinMode(D7, OUTPUT);
    digitalWrite(D7, LOW);         
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

 EnviaEstadoOutputMQTT();
 
  long now = millis();
  if (now - lastMsg > 200) {
    lastMsg = now;

    float newTemp = dht.readTemperature();
      temp = newTemp;
    float newHum = dht.readHumidity();
      hum = newHum;

if(i%2==0)
{
    client.publish(humidity_topic, String(hum).c_str(), true);
      client.publish(temperature_topic, String(temp).c_str(), true);
}
else
{
    client.publish(temperature_topic, String(temp).c_str(), true);
      client.publish(humidity_topic, String(hum).c_str(), true); 
}

i = i+1;
      

      Serial.print("New temperature:");
      Serial.println(String(temp).c_str());
      Serial.print("New humidity:");
      Serial.println(String(hum).c_str());
 
    
}
  client.loop();
}
