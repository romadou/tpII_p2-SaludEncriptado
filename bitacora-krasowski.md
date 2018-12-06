# Bitacora de Iván Krasowski Bissio

## 29 de Septiembre

Lectura del Proyecto 7 del 2017, punto de comienzo  

```markdown
NOTA: llama la atención el manejo de las variables sensadas con el tipo de dato float, siendo que éste genera complicaciones en entornos de microcontroladores. A tener en cuenta.
```

[Instalación de Ruby, Rails, PostgreSQL](https://gorails.com/setup/ubuntu/16.04)  

[Instalación de Arduino IDE](https://www.arduino.cc/en/Guide/Linux)  

Adición del código del Proyecto 7 del 2017 a la [rama Develop del repositorio de GitHub](https://github.com/romadou/tpII_p2-SaludEncriptado/tree/develop), modificados del mismo la versión de Ruby (2.5.1) y de Rails (5.2.1)  

_NOTA: en la carpeta "Sistema Web", ejecución de los siguientes comandos para la adecuación del sistema Rails (aclarados en el informe del TP7-2017):_  
```bash
$ bundle install
$ rails db:create db:migrate
```

------
## 5 de Octubre

Reinstalación de Ruby y Rails por problemas con más de una versión instaladas (ver 29 de Septiembre).  

Prueba de funcionamiento del servidor brindado, sin modificaciones: correcto (imagen "servidor_principal.png").  
- Generación de un perfil de paciente (imagen "servidor_pacientes1.png").
- Generación de un perfil de sensor (imagen "servidor_sensores1.png").

------
## 6 de Octubre

Prueba del Arduino UNO.  
Sketch usado:
```c
int ledPin = 13;  // LED que se encuentra en el pin 13
int n = 0;    //Entero que contará el paso por la función loop

int delayVal(int f){
   return f*100;
}

void setup(){
   pinMode(ledPin, OUTPUT); // El p1n 13 será una salida digital
}

void loop(){
   digitalWrite(ledPin, HIGH);  // Enciende el LED
   delay(1000);         // Pausa de 1 segundo
   digitalWrite(ledPin, LOW);   // Apaga el LED
   n++;         //Incrementamos n
   delay(delayVal(n));      //Pausa de un tiempo variable
}
```  
PROBLEMA: *avrdude: ser_open(): can't open device "/dev/ttyACM0": Permission denied*  
[Solución](https://arduino.stackexchange.com/questions/21215/first-time-set-up-permission-denied-to-usb-port-ubuntu-14-04)


Prueba del Shield Arduino Ethernet.  
Sketch usado:  
```c
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 100);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
```  
(ejemplo de WebServer del Arduino IDE - ver [tutorial](https://startingelectronics.org/articles/arduino/arduino-ethernet-shield-plugging-testing/) )  

OBSERVACIÓN: Funcionamiento aparentemente correcto del shield (al menos en cuestiones eléctricas). Al cargar el sketch y conectar el cable Ethernet a la PC, se encienden las luces PWR, LINK, 100M, FULLD, COLL y, mientras la PC intenta establecer una conexión, el RX a veces parpadea.  

PROBLEMA: por cableado directo, en el navegador no se puede ver la página que se intenta levantar.

OBSERVACIÓN: En la terminal serie del Arduino IDE (imagen "arduino_testWebServer1.png"), solamente se imprime el texto enviado al Serial en el _setup_, del _loop_ no se obtiene nada.  

Prueba del DHT11.  
Conexión (DHT-Arduino): (-) con GND, (+) con 5V, OUT con A0 (imagen "arduino_connectDHT.png")  
Sketch usado:
```c
#include "dht.h"
#define dht_apin A0 // Analog Pin sensor is connected to

dht DHT;

void setup(){

  Serial.begin(9600);
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor

}//end "setup()"

void loop(){
  //Start of Program

    DHT.read11(dht_apin);

    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature);
    Serial.println("C  ");

    delay(5000);//Wait 5 seconds before accessing sensor again.

  //Fastest should be once every two seconds.

}// end loop()
```

PROBLEMA: *dht.h: No such file or directory*  
INTENTO DE SOLUCIÓN - En Arduino IDE:
- Herramientas
- Gestionar librerías
- DHT Sensor Library by Adafruit
- Instalar

NO SE PUDO REALIZAR LA PRUEBA POR FALTA DE TIEMPO

------
## 7 de Octubre

Búsqueda de alternativa para el problema del 6 de Octubre con el DHT11. [Nuevo tutorial.](https://www.prometec.net/sensores-dht11/)  
RESULTADO: funcionamiento normal (imagen "arduino_testDHT.png").  
Sketch usado:  
```c
#include <DHT11.h>
int pin=A0;
DHT11 dht11(pin);
void setup()
{
   Serial.begin(9600);
  while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }
}

void loop()
{
  int err;
  float temp, humi;
  if((err=dht11.read(humi, temp))==0)
  {
    Serial.print("temperature:");
    Serial.print(temp);
    Serial.print(" humidity:");
    Serial.print(humi);
    Serial.println();
  }
  else
  {
    Serial.println();
    Serial.print("Error No :");
    Serial.print(err);
    Serial.println();    
  }
  delay(DHT11_RETRY_DELAY); //delay for reread
}
```
Nuevo intento de conexión del shield Ethernet.
PROBLEMA DETECTADO: la conexión directa a la PC no es una solución transparente.  
ALTERNATIVA: Conectar el Ethernet Shield a un router (ver "bitacora_madou.md" - se usa otro cable que el provisto, por cuestiones de disposición de los muebles). _*FUNCIONAMIENTO CORRECTO*_

```markdown
NOTA: llama la atención el establecimiento de IP en "entrega.ino", siendo que primero se dice utilizar DHCP y después busca conectar con un cliente a una IP hardcodeada. A tener en cuenta.
```  
Prueba del código provisto por el grupo 7 del 2017.  
RESULTADO: no funciona la conexión Ethernet (el Arduino manda constantemente a la terminal serie el mensaje "intentando conectar").  
CONCLUSIÓN: El trabajo sobre el proyecto empezado será más que sobre la encriptación, sino también sobre entender el código y adaptarlo para su uso en otras condiciones que las dispuestas por los integrantes del grupo 7 del 2017.  

------
## 8 de Octubre

Consultas a la cátedra por dudas sobre el Informe de Avance I (ver mensajería de [Ideas](https://ideas.info.unlp.edu.ar))

Escritura del Informe de Avance I

------
## 9 de Octubre

Escritura del Informe de Avance I

------
## 10 de Octubre

Escritura del Informe de Avance I

------
## 1 de Noviembre

Nueva prueba del código provisto por el grupo 7 del 2017.
PASOS SEGUIDOS:
- Se corre el servidor utilizando la directiva "rails server", que da por defecto la 0.0.0.0, puerto 3000.  
- Se carga el sketch "entrega.ino" (desde ahora, "entrega") en el Arduino correctamente montado con sus periféricos y se abre el monitor serie, así como un browser en la 0.0.0.0:3000.  
    - RESULTADO: el servidor carga la página; el monitor serie muestra la dirección IP asignada al shield Ethernet del Arduino, imprime el dato de temperatura que va a enviarse, y repite indefinidamente la sentencia "intentando conectar".  
    - Lo anterior indica que el único punto que no está funcionando es la comunicación.  
- Se acude al informe del grupo 7 del 2017. A partir de una mejor comprensión del código, se vuelve a lanzar el servidor, esta vez con la directiva "rails server -b 192.168.0.3 -p 3000" (para montar el servidor en dichos dirección y puerto) y se modifica el sketch de Arduino, líneas 73 y 80 (192.168.0.3 en lugar de 192.168.2.100).  
    - RESULTADO: mismo que antes, por lo que se entiende que debe seguir existiendo algún detalle a revisar.  
- Se modifica la línea 62 del sketch, para que se midan valores con el sensor de id 1 para la persona con id 1 (los únicos existentes hasta el momento).  
    - RESULTADO: ¡la información llega adecuadamente a la página!

------
## 2 de Noviembre

Réplica de las pruebas del 1 de Noviembre, incluyendo encriptación (sketch "encriptacion_con_aes.ino", desde ahora referenciado como "aes").
Modificación del código, basado en el informe del grupo 7 del 2017 y los archivos disponibles, para lo que tendría que ser un código de envío de la temperatura encriptada.
- CAMBIOS DEL ORIGINAL:  
    - temp y hum son *int*, no *float*.  
    - se modifica la IP para ser la del servidor Rails.  

```c
#include "aes.h"
#include <DHT.h>
#include <Ethernet.h>
#include <Base64.h>

#define DHTTYPE DHT11

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetServer server(80);    // Puerto 80 por defecto para HTTP

//asdfasdf
uint8_t key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);

int t = 0;  //TEMPERATURA
int h = 0;  //HUMEDAD

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

  Serial.print("Datatemp:");
  Serial.println(datatemp);
  aes18_enc_single(key, datatemp);
  Serial.print("encrypted:");
  Serial.print(datatemp);
  Serial.println();
 
  int encodedLen = base64_enc_len(sizeof(datatemp));
  char encoded[encodedLen];
  
  Serial.print(datatemp); 
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
  
  while(!client.connect("192.168.0.3", 3000)) { 
    Serial.println("intentando conectar");
    delay(500);
  }
  
  Serial.println("connecting...");
  client.println("POST /v1/measures HTTP/1.1");
  client.println("Host: 192.168.0.3");
  client.println("Content-Type: application/json; charset:utf-8");
  client.print("Content-Length: ");
  client.println(enviar.length());
  client.println("Connnection: close");
  client.println();
  client.println(enviar);
  client.println();
  
  delay(5000);           // 20 segundos
  
}
```

ERROR:  
  *entrega:57:33: error: 'aes18_enc_single' was not declared in this scope*
  
------
## 7 de Noviembre

Escritura del Informe de Avance II

------
## 8 de Noviembre

PRUEBA: envío de datos encriptados y codificados en Base64.

*SITUACIÓN 1*

Los datos llegan bien, no se pueden decodificar.
ERROR: "iv must be 16 bytes" (ver imagen "passwd_error.png")

*SITUACIÓN 2*

Se deja de lado la encriptación single/EBC (aes128_enc_single) y se empieza a usar la aes128_cbc_enc (incluye como parámetro un iv, o *initialization vector* - ver bitacora-madou, 7 de noviembre)
Se envía la información y llega correctamente, aunque sigue habiendo errores de desencriptación (el problema, ahora, radica en cómo identificar el iv del envío en la recepción).

*SITUACIÓN 3*

Debido a límites de tiempo, se interrumpen acá las pruebas de desencriptación.
Para verificar que efectivamente los datos se mandan bien, y la información encriptada llega, se modifica el código del sistema web en *models/measure.rb* para que impriman el Base64 recibido (ver imagen "envio_encriptacion.png") y el código encriptado (debido a la incompatibilidad de representaciones, la copia no es exacta - ver imagen "received_crypto.png"):

```ruby
#caso 1:
Base64.decode64(self[:value]).unpack("A*").first.to_i

#caso 2:
Base64.decode64(self[:value]).unpack("A*").first.force_encoding(Encoding::UTF_8)
```


Escritura del Informe de Avance II

------
## 15 de Noviembre

Preparación de Presentación de Avance
Presentación de Avance

------
## 20 de Noviembre

Búsqueda de la obtención del resultado sensado (temperatura) tal cual enviado: hasta aquí, se lo tenía encriptado y codificado en Base64.  

Continuando el trabajo del 8 de Noviembre, se modifica el archivo de modelo *measure.rb* para el tratamiento del dato que llega. Se pasa del "Código A" (informe de avance 2) al "Código B"  

- Código A  
```ruby
def decrypted_value
  Base64.decode64(self[:value]).unpack("A*").first.to_f
end
```  

- Código B  
```ruby
def decrypted_value
  # El valor llega codificado en Base64
  decoded = Base64.decode64(self[:value])

  c = OpenSSL::Cipher::Cipher.new("aes-128-cbc")
  # Objeto de cifrado en modo para desencriptar
  c.decrypt
  # Importante, puesto que la información viene sin padding
  c.padding = 0
  # La clave y el vector de inicialización deben ser los mismos que los puestos al encriptar
  c.key = 'aaaabbbbccccdddd'
  c.iv = 'ABCDEFGHIJKLMNOP'
  d = c.update(decoded).unpack("A*").first.to_f

end
```

PROBLEMA: las pruebas sólo se pueden realizar sobre el servidor montado en localhost (comando `rails server`), por el siguiente error para otras IPs:

```bash
Cannot assign requested address - bind(2) for "192.168.0.39" port 3000 (Errno::EADDRNOTAVAIL)
```  

------
## 21 de Noviembre  

Relativo al problema del 20 de Noviembre: en la red doméstica, sólo resulta posible montar el servidor en algún puerto no utilizado de 0.0.0.0, 127.0.0.x, localhost (hasta aquí, todos equivalentes), o 192.168.0.3 (IP asignada por el router, por DHCP, a la PC).  

Pruebas del sistema sobre switch (Facultad de Informática)

*SITUACIÓN 1*

No hay una IP disponible por el puerto Ethernet. No se puede montar el servidor Rails.  
El Arduino toma su IP estática sin problemas. Queda esperando la conexión.

*SITUACIÓN 2*

Se asigna una IP a la interfaz Ethernet de la PC:  
```bash
sudo ifconfig <interfaz-ethernet> 192.168.0.39
```  

PROBLEMA: El Arduino espera conectarse a 192.168.0.39:3000, pero está configurado en la IP 192.168.0.39
SOLUCIÓN: Se asigna al Arduino la IP 192.168.0.38

Comprobación de encriptado, envío, recepción y desencriptado correctos (ver imagen *comm_final.png*)
Finalización de las pruebas

------

## 5 de Diciembre

Escritura del Informe Final

------

## 6 de Diciembre

Escritura del Informe Final  
Presentación  

------
