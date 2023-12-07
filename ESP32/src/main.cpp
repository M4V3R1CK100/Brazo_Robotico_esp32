#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ESP32Servo.h>

#define LED_PIN_1 12
#define LED_PIN_2 27
#define LED_PIN_3 25
#define LED_PIN_4 32
#define LED_PIN_5 34
#define LED_PIN_6 23

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

int servoPin(String servo)
{
  if (servo == "garra")
  {
    return LED_PIN_1;
  }

  else if (servo == "muneca_pitch")
  {
    return LED_PIN_2;
  }

  else if (servo == "muneca_yaw")
  {
    return LED_PIN_3;
  }

  else if (servo == "codo")
  {
    return LED_PIN_4;
  }

  else if (servo == "antebrazo")
  {
    return LED_PIN_5;
  }

  else if (servo == "base")
  {
    return LED_PIN_6;
  }
  else{
    return 0;
  }

}

void moverServo(int angulo, String servo)
{

  // Ajustar el rango del ángulo según tus necesidades
  int anguloAjustado = constrain(angulo, 0, 180);

  if      (servo == "garra")
  {
    Serial.println(anguloAjustado);
    servo_garra.write(anguloAjustado);
  }

  else if (servo == "muneca_pitch")
  {
    Serial.println(anguloAjustado);
    servo_muneca_pitch.write(anguloAjustado);
  }

  else if (servo == "muneca_yaw")
  {
    Serial.println(anguloAjustado);
    servo_muneca_yaw.write(anguloAjustado);
  }

  else if (servo == "codo")
  {
    Serial.println(anguloAjustado);
    servo_codo.write(anguloAjustado);
  }

  else if (servo == "antebrazo")
  {
    Serial.println(anguloAjustado);
    servo_antebrazo.write(anguloAjustado);
  }

  else if (servo == "base")
  {
    Serial.println(anguloAjustado);
    servo_base.write(anguloAjustado);
  }

}

void controlarLED(int valor, String servo)
{
  
  // Ajustar el rango del ángulo según tus necesidades
  int brillo = map(valor, 0, 180,0,255);

  analogWrite(servoPin(servo),valor);
  
  printf("\n\nEl servo %s se ha movido %d\n\n",servo,valor);
}

void setup()
{
  pinMode(LED_PIN_1, OUTPUT);
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
  if (!WiFi.softAP(ssid, password))
  {
    log_e("Soft AP creation failed.");
    while (1)
      ;
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop()
{
  WiFiClient client = server.available(); // listen for incoming clients

  if (client)
  {                                // if you get a client,
    Serial.println("New Client."); // print a message out the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        if (c == '\n')
        { // if the byte is a newline character
          if (currentLine.length() == 0)
          {
            break;
          }
          else
          { // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }

        if (currentLine.startsWith("GET /servos") and currentLine.endsWith("HTTP/1.1"))
        {
          // Extraer el valor del servomotor de la solicitud
          // int pos = currentLine.indexOf('=');
          // String nameVar = currentLine;
          int startVarName = currentLine.indexOf("varName=");
          int endVarName = currentLine.indexOf("&", startVarName);
          int startVarValue = currentLine.indexOf("varValue=", endVarName);
          int endVarValue = currentLine.indexOf(" ", startVarValue);

          // Extraer los valores de varName y varValue
          String varName = currentLine.substring(startVarName + 8, endVarName);
          String varValue = currentLine.substring(startVarValue + 9, endVarValue);

          // Ahora varName y varValue contienen los valores de las variables en la URL
          controlarLED(varValue.toInt(), varName);
          
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
