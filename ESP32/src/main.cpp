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
#define RangoError 40

Servo servo_garra;
Servo servo_muneca_pitch;
Servo servo_muneca_yaw;
Servo servo_codo;
Servo servo_antebrazo;
Servo servo_base;

// Set these to your desired credentials.
const char *ssid = "Brazo Robotico";
const char *password = "12345678";
int ultimoValor = 0;
int intentosFallidos = 0;

WiFiServer server(80);

void moverServo(int angulo, String servo){
  
  // Ajustar el rango del ángulo según tus necesidades
  int anguloAjustado = constrain(angulo, 0, 180);

  if (servo == "garra"){
    Serial.println(anguloAjustado);
    servo_garra.write(anguloAjustado);
  }

  if (servo == "muneca_pitch"){
    Serial.println(anguloAjustado);
    servo_muneca_pitch.write(anguloAjustado);
  }
  
  if (servo == "muneca_yaw"){
    Serial.println(anguloAjustado);
    servo_muneca_yaw.write(anguloAjustado);
  }
  
  if (servo == "codo"){
    Serial.println(anguloAjustado);
    servo_codo.write(anguloAjustado);
  }
  
  if (servo == "antebrazo"){
    Serial.println(anguloAjustado);
    servo_antebrazo.write(anguloAjustado);
  }

  if (servo == "base"){
    Serial.println(anguloAjustado);
    servo_base.write(anguloAjustado);
  }

}


void controlarLED(int valor){
  int brillo = map(valor, 0, 180, 0, 255);  // Mapear el valor del ángulo a un valor de brillo para el LED
  analogWrite(LED_PIN, brillo);             // Establecer el brillo del LED
  Serial.print("Brillo del LED: ");
  Serial.println(brillo);
}


void controlarServos(String line){

  if      (line.startsWith("GET /control?garra=") and line.endsWith("HTTP/1.1")){
          // Extraer el valor del servomotor de la solicitud
          int pos = line.indexOf('=');

          if (pos != -1) {
            int valorServomotor = line.substring(pos + 1).toInt();

            // Filtrar valores incorrectos
            if ((abs(valorServomotor - ultimoValor)) < RangoError) {
              moverServo(valorServomotor,"garra");
              Serial.printf("\n\nÚltimo valor de la garra: %d\n",ultimoValor);

              if(valorServomotor != 0){
                ultimoValor = valorServomotor;
                intentosFallidos = 0;
                Serial.printf("El valor de la garra: %d\n", valorServomotor);
              }
            } else {
              intentosFallidos++;
            }
          }
  }
  else if (line.startsWith("GET /control?muneca_pitch=") and line.endsWith("HTTP/1.1")){
          // Extraer el valor del servomotor de la solicitud
          int pos = line.indexOf('=');

          if (pos != -1) {
            int valorServomotor = line.substring(pos + 1).toInt();

            // Filtrar valores incorrectos
            if ((abs(valorServomotor - ultimoValor)) < RangoError) {
              moverServo(valorServomotor,"muneca_pitch");
              Serial.printf("\n\nÚltimo valor de muneca_pitch: %d\n\n",ultimoValor);

              if(valorServomotor != 0){
                ultimoValor = valorServomotor;
                intentosFallidos = 0;
                Serial.printf("El valor de muneca_pitch: %d\n", valorServomotor);
              }
            } else {
              intentosFallidos++;
            }
          }
  }
  else if (line.startsWith("GET /control?muneca_yaw=") and line.endsWith("HTTP/1.1")){
          // Extraer el valor del servomotor de la solicitud
          int pos = line.indexOf('=');

          if (pos != -1) {
            int valorServomotor = line.substring(pos + 1).toInt();

            // Filtrar valores incorrectos
            if ((abs(valorServomotor - ultimoValor)) < RangoError) {
              moverServo(valorServomotor,"muneca_yaw");
              Serial.printf("\n\nÚltimo valor muneca_yaw: %d\n\n",ultimoValor);

              if(valorServomotor != 0){
                ultimoValor = valorServomotor;
                intentosFallidos = 0;
                Serial.printf("El valor de muneca_yaw: %d\n", valorServomotor);
              }
            } else {
              intentosFallidos++;
            }
          }
  }
  else if (line.startsWith("GET /control?codo=") and line.endsWith("HTTP/1.1")){
          // Extraer el valor del servomotor de la solicitud
          int pos = line.indexOf('=');

          if (pos != -1) {
            int valorServomotor = line.substring(pos + 1).toInt();

            // Filtrar valores incorrectos
            if ((abs(valorServomotor - ultimoValor)) < RangoError) {
              moverServo(valorServomotor,"codo");
              Serial.printf("\n\nÚltimo valor de codo: %d\n\n",ultimoValor);

              if(valorServomotor != 0){
                ultimoValor = valorServomotor;
                intentosFallidos = 0;
                Serial.printf("El valor de codo: %d\n", valorServomotor);
              }
            } else {
              intentosFallidos++;
            }
          }
  }
  else if (line.startsWith("GET /control?antebrazo=") and line.endsWith("HTTP/1.1")){
          // Extraer el valor del servomotor de la solicitud
          int pos = line.indexOf('=');

          if (pos != -1) {
            int valorServomotor = line.substring(pos + 1).toInt();

            // Filtrar valores incorrectos
            if ((abs(valorServomotor - ultimoValor)) < RangoError) {
              moverServo(valorServomotor,"antebrazo");
              Serial.printf("\n\nÚltimo valor de antebrazo: %d\n\n",ultimoValor);

              if(valorServomotor != 0){
                ultimoValor = valorServomotor;
                intentosFallidos = 0;
                Serial.printf("El valor de antebrazo: %d\n", valorServomotor);
              }
            } else {
              intentosFallidos++;
            }
          }
  }
  else if (line.startsWith("GET /control?base=") and line.endsWith("HTTP/1.1")){
          // Extraer el valor del servomotor de la solicitud
          int pos = line.indexOf('=');

          if (pos != -1) {
            int valorServomotor = line.substring(pos + 1).toInt();

            // Filtrar valores incorrectos
            if ((abs(valorServomotor - ultimoValor)) < RangoError) {
              moverServo(valorServomotor,"base");
              Serial.printf("\n\nÚltimo valor de base: %d\n\n",ultimoValor);

              if(valorServomotor != 0){
                ultimoValor = valorServomotor;
                intentosFallidos = 0;
                Serial.printf("El valor de base: %d\n", valorServomotor);
              }
            } else {
              intentosFallidos++;
            }
          }
  }

}
void setup() {
  pinMode(LED_PIN, OUTPUT);
  servo_garra.attach(3);
  /*
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
  Falta hacer el attach los pines para cada una de los servos
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
  */

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

        if (currentLine.startsWith("GET /control?garra=") and currentLine.endsWith("HTTP/1.1")){
          // Extraer el valor del servomotor de la solicitud
          int pos = currentLine.indexOf('=');

          if (pos != -1) {
            int valorServomotor = currentLine.substring(pos + 1).toInt();
            // Filtrar valores incorrectos
            if ((abs(valorServomotor - ultimoValor)) < RangoError) {
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
