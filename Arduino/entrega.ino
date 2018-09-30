#include <DHT.h>
#include <Ethernet.h>
#include <Base64.h>

#define DHTTYPE DHT11

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetServer server(80);    // Puerto 80 por defecto para HTTP


const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);

float t = 0;  //TEMPERATURA
float h = 0;  //HUMEDAD

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);    //el servidor DHCP nos da una IP dinamica
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());   //la IP que me asigno
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:

  h = dht.readHumidity();       //leo humedad
  t = dht.readTemperature();    //leo temperatura

  if(isnan(h) || isnan(t)){
      Serial.println("Fallo del sensor al medir");         
      return;                                       //vuelvo al loop
   }

  EthernetClient client; // Inicializa cliente como servidor ethernet


  String temperatureString = String(t);
  int lengthtemp = temperatureString.length();
  char temperature[lengthtemp];
  temperatureString.toCharArray(temperature, temperatureString.length());
  char datatemp[17]="0000000000000000";     //16 chars == 16 bytes
  int i;
  int j=0;
  for (i = 17-lengthtemp; i < 17; i = i + 1) {
    datatemp[i] = temperature[j];
    j++;
  }
 
  int encodedLen = base64_enc_len(sizeof(datatemp));
  char encoded[encodedLen];
  
  Serial.print(datatemp); 
  Serial.print(" = ");
  
  base64_encode(encoded, datatemp, sizeof(datatemp)); 
  
  Serial.println(encoded);

  String enviar = String("{\"measure\": { \"value\":\"" + String(encoded) + "\",\"patient_id\":3,\"sensor_id\":3 } }");

//String enviar = String("{\"measure\": { \"value\":\"" + String(f) + "\",\"patient_id\":3,\"sensor_id\":3 } }");

  client.stop();
   
  if (client.available()) {
      char c = client.read();
      Serial.write(c);
  }
  
  while(!client.connect("192.168.2.100", 3000)) { 
    Serial.println("intentando conectar");
    delay(500);
  }
  
  Serial.println("connecting...");
  client.println("POST /v1/measures HTTP/1.1");
  client.println("Host: 192.168.2.100");
  client.println("Content-Type: application/json; charset:utf-8");
  client.print("Content-Length: ");
  client.println(enviar.length());
  client.println("Connnection: close");
  client.println();
  client.println(enviar);
  client.println();
  
  delay(5000);           // 20 segundos
  
}


