void reconnectMQTT() 
{
    while (!MQTT.connected()) 
    {

        if (MQTT.connect(device_name)) 
        {
            //Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(subs); 
        } 
        else
        {
            //Serial.println("Falha ao reconectar no broker.");
            delay(2000);
        }
    }
}


void init_mqtt(){
  int port = atoi(broker_port);
    MQTT.setServer(broker,port ); 
    MQTT.setCallback(mqtt_callback); 

}


void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
 
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }

  //Serial.println(msg);

}



//------------------------SETUP WIFI-----------------------------
void setup_wifi() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(250);
  
  }
  if (contconexion <50) {   
    //  Serial.println("WiFi conectado");
  
  }
  else { 
     // Serial.println("Error !");

  }
}


//-------------------PAGINA DE CONFIG--------------------
void paginaconf() {
  server.send(200, "text/html", pagina + mensaje + paginafin); 
}

//------------------ CONFIGURANDO DEVICE ----------------------


void modoconf() {
   

  WiFi.softAP(ssidConf, passConf);
  IPAddress myIP = WiFi.softAPIP(); 


  server.on("/", paginaconf);

  server.on("/guardar_conf", guardar_conf);

  
  server.begin();

  while (true) {
      server.handleClient();
  }
}

//---------------------GUARDAR CONFIGURACION-------------------------
void guardar_conf() {
  String a = server.arg("1") + server.arg("2") + server.arg("3")+ server.arg("4")+ server.arg("5");
  
  gravar(0,server.arg("ssid"));
  gravar(25,server.arg("pass"));
  gravar(50,server.arg("broker"));
  gravar(75,server.arg("port_broker"));
  gravar(100,server.arg("subs"));
  gravar(125,server.arg("publi"));
  gravar(150,server.arg("device_name"));
  gravar(175,a);


  mensaje = "Configuração salva com sucesso!";
  paginaconf();

  delay(4000);
  ESP.reset();

}


//----------------Función para grabar en la EEPROM-------------------
void gravar(int addr, String a) {
  int tamano = a.length(); 
  char inchar[25]; 
  a.toCharArray(inchar, tamano+1);
  for (int i = 0; i < tamano; i++) {
    EEPROM.write(addr+i, inchar[i]);
  }
  for (int i = tamano; i < 25; i++) {
    EEPROM.write(addr+i, 255);
  }
  EEPROM.commit();
}

//--------------------- ler EEPROM------------------------
String leer(int addr) {
   byte lectura;
   String strlectura;
   for (int i = addr; i < addr+25; i++) {
      lectura = EEPROM.read(i);
      if (lectura != 255) {
        strlectura += (char)lectura;
      }
   }
   return strlectura;
}
