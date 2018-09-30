#include <DHT.h>
#include <Ethernet.h>
#include <Base64.h>
#include <xxtea-iot-crypt.h>

#define DHTTYPE DHT11

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetServer server(80);    // Puerto 80 por defecto para HTTP


const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);

float t = 0;  //TEMPERATURA
float h = 0;  //HUMEDAD

void setup() {
  Serial.begin(9600);
  Serial.print("HOla");
  Ethernet.begin(mac);    //el servidor DHCP nos da una IP dinamica
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());   //la IP que me asigno
  delay(3000);

   EthernetClient client; // Inicializa cliente como servidor ethernet
   
  Serial.println();
  uint8_t keybuf[] = "Hello Password";
  uint8_t plaintext[] = "Hi There we can work with this";
  uint8_t buffer[200];
  size_t len = 200, i;

  // Setup the Key - Once
  if(xxtea_setup(keybuf, strlen((char *)keybuf)) != XXTEA_STATUS_SUCCESS)
  {
    Serial.println(" Assignment Failed!");
    return;
  }

  // Perform Encryption on the Data
  len = 200;  // - Initialize the Maximum buffer length
  if(xxtea_encrypt(plaintext, strlen((char*)plaintext), buffer, &len) !=
    XXTEA_STATUS_SUCCESS)
  {
    Serial.println(" Encryption Failed!");
    return;
  }
  else
  {
    Serial.println(" Encrypted Data: ");
    for(i = 0;i<len;i++)
      Serial.print(buffer[i], HEX);
    Serial.println();
  }

  
  int encodedLen = base64_enc_len(sizeof((char *)buffer));
  char encoded[encodedLen];
    
  base64_encode(encoded, (char *)buffer, sizeof((char *)buffer)); 
  
  Serial.println(encoded);

  String enviar = String("{\"measure\": { \"value\":\"" + String(encoded) + "\",\"patient_id\":3,\"sensor_id\":3 } }");


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
  
  
}

void loop() {
}


