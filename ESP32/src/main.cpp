/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ESP32Servo.h>


#define LED_PIN 12  // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

Servo servo_garra;
// Set these to your desired credentials.
const char *ssid = "Brazo Robotico";
const char *password = "12345678";
int ultimoValor = 0;
int intentosFallidos = 0;

WiFiServer server(80);

void controlarServo(int angulo){
   // Ajustar el rango del ángulo según tus necesidades
  int anguloAjustado = constrain(angulo, 0, 180);

  // Mover el servo al ángulo especificado
  Serial.print(anguloAjustado);
  servo_garra.write(anguloAjustado);
}
void controlarLED(int valor){
  int brillo = map(valor, 0, 180, 0, 255);  // Mapear el valor del ángulo a un valor de brillo para el LED
  analogWrite(LED_PIN, brillo);             // Establecer el brillo del LED
  Serial.print("Brillo del LED: ");
  Serial.println(brillo);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  servo_garra.attach(3);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }else if (c != '\r'){  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.startsWith("GET /control?garra=") and currentLine.endsWith("HTTP/1.1")){
          // Extraer el valor del servomotor de la solicitud
          int pos = currentLine.indexOf('=');
          if (pos != -1) {
            int valorServomotor = currentLine.substring(pos + 1).toInt();
            // Filtrar valores incorrectos
            if ((abs(valorServomotor - ultimoValor)) < 40) {
              controlarLED(valorServomotor);
              Serial.print("El ultimo valor: ");
              Serial.println(ultimoValor);
              if(valorServomotor != 0){
                ultimoValor = valorServomotor;}
                intentosFallidos = 0;
                Serial.print("El valor del servomotor: ");
                Serial.println(valorServomotor);
            } else {
              intentosFallidos++;
            }
            delay(10);
          }
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
