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