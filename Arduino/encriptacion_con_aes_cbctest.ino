#include <AESLib.h>
#include <DHT.h>
#include <Base64.h>

#define DHTTYPE DHT11

const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);

float t = 0;  //TEMPERATURA
float h = 0;  //HUMEDAD

void setup() {
  Serial.begin(9600);
  //Ethernet.begin(mac);    //el servidor DHCP nos da una IP dinamica
  //Serial.print("My IP address: ");
  //Serial.println(Ethernet.localIP());   //la IP que me asigno
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

  // Clave
   uint8_t key[] = {
     0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
     0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
   };

  // Vector de inicialización
   uint8_t iv[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};
  

  //Paso de float a vector de char de 16
  
  String temperatureString = String(t);
  int lengthtemp = temperatureString.length();
  char temperature[lengthtemp];
  temperatureString.toCharArray(temperature, temperatureString.length());

  String humidityString = String(h);
  int lengthhum = humidityString.length();
  char humidity[lengthhum];
  humidityString.toCharArray(humidity, humidityString.length());

  uint16_t dataLength = 16; // Tamaño del dato
  
  char datatemp[dataLength]="0000000000000000";     //16 chars == 16 bytes
  char datahum[dataLength]="0000000000000000";      //16 chars == 16 bytes
  int i;
  int j=0;
  for (i = dataLength-lengthtemp; i < dataLength; i = i + 1) {
    datatemp[i] = temperature[j];
    j++;
  }
  j=0;
  for (i = dataLength-lengthhum; i < dataLength; i = i + 1) {
    datahum[i] = humidity[j];
    j++;
  }
  
  dump("Key: ", key, sizeof(key));     //imprimo la clave
  dump2("IV: ", iv, sizeof(iv));     //imprimo el vector de inicialización

  Serial.println();

  Serial.print("Datatemp:");
  Serial.println(datatemp);
  //aes128_enc_single(key,datatemp);    //encripto los datos de temperatura
  aes128_cbc_enc(key, iv, datatemp, dataLength);
  Serial.print("encrypted:");
  Serial.write(datatemp, dataLength);
  Serial.println();
  //aes128_dec_single(key,datatemp);    //desencripto los datos de temperaura
  aes128_cbc_dec(key, iv, datatemp, dataLength);
  Serial.print("Decrypted:");
  Serial.println(datatemp);

  
  Serial.println();
  
  Serial.print("Datahum:");
  Serial.println(datahum);
  //aes128_enc_single(key,datahum);     //encripto los datos de humedad
  aes128_cbc_enc(key, iv, datahum, dataLength);
  Serial.print("encrypted:");
  Serial.write(datahum, dataLength);
  Serial.println();
  //aes128_dec_single(key,datahum);     //desencripto los datos de humedad
  aes128_cbc_dec(key, iv, datahum, dataLength);
  Serial.print("Decrypted:");
  Serial.println(datahum);

  Serial.println();
  
  Serial.print("Temperatura en char[]: ");
  Serial.println(temperature);
    
  Serial.println();
  
  Serial.print("Humedad en char[]: ");
  Serial.println(humidity);
  
  Serial.println();
  
  Serial.println("Temperatura en float: ");
  Serial.println(t);
  
  Serial.println();

  Serial.println("Humedad en float: ");
  Serial.println(h);
  
  delay(5000);           // 20 segundos
  
}

void dump(String str,uint8_t buf[],int sz) {
  int i; 
  Serial.println(str); 
  for(i=0; i<(sz); ++i) { 
    if(buf[i]<0x10) 
      Serial.print('0'); 
    Serial.print(char(buf[i]), HEX); 
  } 
  Serial.println(); 
} //Help function for printing the Output

void dump2(String str,uint8_t buf[],int sz) {
  int i; 
  Serial.println(str); 
  for(i=0; i<(sz); ++i) { 
      Serial.print(char(buf[i]));
      Serial.print(' '); 
  } 
  Serial.println(); 
} //Help function for printing the Output
