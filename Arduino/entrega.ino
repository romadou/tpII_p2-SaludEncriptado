#include <AESLib.h>
#include <DHT.h>
#include <Ethernet.h>
#include <Base64.h>

#define DHTTYPE DHT11

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetServer server(80);    // Puerto 80 por defecto para HTTP

uint8_t key[] = { 0x61, 0x61, 0x61, 0x61, 0x62, 0x62, 0x62, 0x62, 0x63, 0x63, 0x63, 0x63, 0x64, 0x64, 0x64, 0x64 };
//uint8_t key[] = "aaaabbbbccccdddd";

uint8_t iv[16] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};

const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);

float t = 0;  //TEMPERATURA
float h = 0;  //HUMEDAD

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, 192.168.0.39);    //asignación de una IP estática
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());   //muestra de la IP asignada al usuario
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
  
  uint16_t dataLength = 16;
  char datatemp[dataLength]="0000000000000000";     //16 chars == 16 bytes
  
  int i;
  int j=0;
  for (i = dataLength-lengthtemp; i < dataLength; i = i + 1) {
    datatemp[i] = temperature[j];
    j++;
  }

  Serial.print("Datatemp: ");
  Serial.println(datatemp);
  aes128_cbc_enc(key, iv, datatemp, dataLength);
  Serial.print("encrypted:");
  Serial.write(datatemp, sizeof(datatemp));
  Serial.println();
 
  int encodedLen = base64_enc_len(sizeof(datatemp));
  char encoded[encodedLen];
  
  Serial.write(datatemp, sizeof(datatemp)); 
  Serial.print(" = ");
  
  base64_encode(encoded, datatemp, sizeof(datatemp)); 
  
  Serial.println(encoded);

  String enviar = String("{\"measure\": { \"value\":\"" + String(encoded) + "\",\"patient_id\":1,\"sensor_id\":1 } }");

//String enviar = String("{\"measure\": { \"value\":\"" + String(f) + "\",\"patient_id\":1,\"sensor_id\":1 } }");

  client.stop();
   
  if (client.available()) {
      char c = client.read();
      Serial.write(c);
  }
  
  while(!client.connect("192.168.0.39", 3000)) { 
    Serial.println("intentando conectar");
    delay(500);
  }
  
  Serial.println("connecting...");
  client.println("POST /v1/measures HTTP/1.1");
  client.println("Host: 192.168.0.39");
  client.println("Content-Type: application/json; charset:utf-8");
  client.print("Content-Length: ");
  client.println(enviar.length());
  client.println("Connnection: close");
  client.println();
  client.println(enviar);
  client.println();
  
  delay(5000);           // 20 segundos
  
}
