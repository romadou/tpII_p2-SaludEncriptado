# Bitacora de Rocío Madou

## 29 de Septiembre

Creación del repositorio del proyecto junto con la organización que se tendrá a lo largo del desarrollo del mismo.

Lectura el Proyecto 7 del 2017.

---
## 6 de Octubre

Pruebas de funcionamiento de los componentes de hardware individuales y en conjunto (6 de Octubre, bitácora-krasowski)

---
## 7 de Octubre

Pruebas de conexión entre el Arduino y la PC.

Al realizar una conexión directa entre los componentes no se le asigna una IP a la placa, necesaria para su identificación al momento de envío de paquetes. Es por ello que se vuelve necesaria la incorporación de un switch o router (para acceso a internet), encargado de cumplir dicha función.

Al realizar lo mencionado anteriormente, se soluciona el problema de comunicación.

Continuación con las pruebas de componentes (7 de Octubre, bitácora-krasowski).

---
## 9 de Octubre

Redacción del Informe de Avance I

---
## 10 de Octubre

Corrección y finalización del Informe de Avance I

---

## 11 de Octubre

Entrega del Informe de Avance I

---

## 1 de Noviembre

Análisis del scripts de prueba de encriptación AES

**encriptación_con_aes**

``` Arduino
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

  h = dht.readHumidity();       //leo humedad
  t = dht.readTemperature();    //leo temperatura

  //uint8_t key[] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};  //clave de encriptacion

    uint8_t key[] = { //asdfasdf
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
  };
  
  if(isnan(h) || isnan(t)){
    Serial.println("Fallo del sensor al medir");         
    return;                    //vuelvo al loop
  }

  //Paso de float a vector de char de 16
  
  String temperatureString = String(t);
  int lengthtemp = temperatureString.length();
  char temperature[lengthtemp];
  temperatureString.toCharArray(temperature, temperatureString.length());

  String humidityString = String(h);
  int lengthhum = humidityString.length();
  char humidity[lengthhum];
  humidityString.toCharArray(humidity, humidityString.length());
  
  char datatemp[17]="0000000000000000";     //16 chars == 16 bytes
  char datahum[17]="0000000000000000";      //16 chars == 16 bytes
  int i;
  int j=0;
  for (i = 17-lengthtemp; i < 17; i = i + 1) {
    datatemp[i] = temperature[j];
    j++;
  }
  j=0;
  for (i = 17-lengthhum; i < 17; i = i + 1) {
    datahum[i] = humidity[j];
    j++;
  }

  int x;
  dump("Key: ", x, key, sizeof(key));     //imprimo la clave

  Serial.print("Datatemp:");
  Serial.println(datatemp);
  aes128_enc_single(key,datatemp);    //encripto los datos de temperatura
  Serial.print("encrypted:");
  Serial.print(datatemp);
  Serial.println();
  aes128_dec_single(key,datatemp);    //desencripto los datos de temperaura
  Serial.print("Decrypted:");
  Serial.println(datatemp);

  
  Serial.println();
  
  Serial.print("Datahum:");
  Serial.println(datahum);
  aes128_enc_single(key,datahum);     //encripto los datos de humedad
  Serial.print("encrypted:");
  Serial.print(datahum);
  Serial.println();
  aes128_dec_single(key,datahum);     //desencripto los datos de humedad
  Serial.print("Decrypted:");
  Serial.println(datahum);

  Serial.println();
  
  Serial.print("Temperatura en char[]:");
  Serial.println(temperature);
    
  Serial.println();
  
  Serial.print("Humedad en char[]:");
  Serial.println(humidity);
  
  Serial.println();
  
  Serial.println("Temperatura en float: ");
  Serial.println(t);
  
  Serial.println();

  Serial.println("Humedad en float: ");
  Serial.println(h);
  
  delay(5000);           // 20 segundos
  
}

void dump(String str,int i,uint8_t buf[],int sz) { 
    Serial.println(str); 
    for(i=0; i<(sz); ++i) { 
        if(buf[i]<0x10) 
            Serial.print('0'); 
        Serial.print(char(buf[i]), HEX); 
    } 
    Serial.println(); 
} //Help function for printing the Output

```
 **Conclusión** : 
 
 El script encripta y desencripta el arreglo de caracteres correctamente (ver imágen _test_enc_aes.png_ ) 

**Irregularidades** :

- Se realiza la lectura de valores de humedad y temperatura por duplicado (sobreescribiéndolo) antes de la encriptación
- La función _dump_ (encargada de la muestra de la clave en hezadecimal utilizando 2 dígitos) recibe como parámetro el índice del for - Se considera ilógico pudiendo ser una variable local -
- El tamaño de los arreglos de datos es de 17 elementos, cuando se dispone que los mismos puedan tener un máximo de 16 bytes.

> Con el objetivo de lograr la comunicación encriptada del dato al servidor se procede a crear un nuevo archivo (_entrega_aes.ino_) basado en el ya existente _entrega.ino_

---
## 6 de Noviembre 

Se habilita, dentro el código del sistema web (archivo de modelo _measure.rb_) el desencriptado por AES que se encontraba estaba comentado. Eliminación del carácter 'ñ', reemplazándolo por 'n'.

```ruby
AES.decrypt(self[:value], "aaaabbbbccccdddd")
```
--

Prueba del código con encriptación (2 de Noviembre, bitacora-krasowski).

**Problema** :

No se encontraba el método "aes18_enc_single"

**Solución** :

El método se llama "aes128_enc_single"

--

Nueva prueba

Carga y comunicación correctas. 

Error de contraseña (ver imagen _passwd_error.png_): "IV must be 16 bytes".

**Solución** :
 
 Utilizar una contraseña de 16 bytes de largo; se utiliza "aaaabbbbccccdddd" en lugar de la anterior "una contrasena".

**Problema** :

 "IV must be 16 bytes" (se supone que es porque se cuenta el '\0' de final de String)

Se lee que IV es por "Initialization Vector". El mismo es un parámetro propio de la librería de encriptación de Ruby, pero no está incluido en la de Arduino.

---
## 7 de Noviembre

Buscando una solución a la problemática, se encuentra una opción a implementar, detallada dentro del [presente _issue_](https://github.com/DavyLandman/AESLib/issues/10).

Utilizando como base el código _encriptación_con_aes_, se crea el siguiente script ( _encriptación_con_aes_cbc_ ) como prueba:

```Arduino
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

```
**Conclusión** : 
 
 El script funciona de la forma esperada (ver imágen _test_enc_aes_cbc.png_ )

 > Se procede a testear la solución encontrada dentro del archivo del servidor (8 de Noviembre, bitacora-krasowski)

---
## 8 de Noviembre 

Finalización y Entrega del Informe de Avance II

---

## 19 de Noviembre

Cambio de configuraciones de conexión

```markdown
IP = 192.168.0.39

Puertos :
- Comunicación = 3000
- Servidor = 8000 / 8080 (a definir)
```

_Verificar la red en la que trabaja el switch_

---