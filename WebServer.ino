#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include <Espalexa.h>


//-------------------VARIAVEIS GLOBALES--------------------------
int contconexion = 0;


char ssid[25];      
char pass[25];
char broker[25];
char broker_port[25];

char device_name[25];
char subs[25];    
char publi[25];

char configs[25];

String alexa_name;


const char *ssidConf = "IoT_Setup";
const char *passConf = "12345678";

String mensaje = "";

//-----------CODIGO HTML PAGINA DE CONFIGURACION---------------
extern String pagina;

String paginafin = "</body>"
"</html>";



WiFiClient espClient;
ESP8266WebServer server(80);


PubSubClient MQTT(espClient); 


Espalexa espalexa;
extern void firstLightChanged(uint8_t brightness);

//------------------------SETUP-----------------------------
void setup() {

  pinMode(1, OUTPUT); //RELAY OUTPUT  
  //Serial.begin(115200);
  EEPROM.begin(512);

  pinMode(3, INPUT); //BUTTON CONFIG
  if (digitalRead(3) == 0) {
    modoconf();
  }

  leer(0).toCharArray(ssid, 25);
  leer(25).toCharArray(pass, 25);
  leer(50).toCharArray(broker,25);
  leer(75).toCharArray(broker_port,25);
  leer(100).toCharArray(subs,25);
  leer(125).toCharArray(publi,25);
  leer(150).toCharArray(device_name,25);
  leer(175).toCharArray(configs,25);
  

  alexa_name  = String(device_name);

  
  setup_wifi();
  init_mqtt();
  //reconnectMQTT();
  
  espalexa.addDevice(alexa_name, firstLightChanged); 
  espalexa.begin();

  digitalWrite(1,LOW);
  
}

void loop() {
espalexa.loop();
MQTT.loop();
}
