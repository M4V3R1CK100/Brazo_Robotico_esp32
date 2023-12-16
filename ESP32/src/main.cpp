#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ESP32Servo.h>

#define SERVO_1 19
#define SERVO_2 27
#define SERVO_3 25
#define SERVO_4 32
#define SERVO_5 34
#define SERVO_6 23


Servo servo_garra;
Servo servo_muneca_pitch;
Servo servo_muneca_yaw;
Servo servo_codo;
Servo servo_antebrazo;
Servo servo_base;

// Set these to your desired credentials.
const char *ssid = "Brazo Robotico";
const char *password = "12345678";

int startVarName, endVarName, startVarValue, endVarValue;

String varName,varValue;

WiFiServer server(80);

void moverServo(int angulo, String servo)
{
  if      (servo == "garra")
  {

    servo_garra.write(angulo);

  }

  else if (servo == "muneca_pitch")
  {

    servo_muneca_pitch.write(angulo);

  }

  else if (servo == "muneca_yaw")
  {

    servo_muneca_yaw.write(angulo);

  }

  else if (servo == "codo")
  {

    servo_codo.write(angulo);

  }

  else if (servo == "antebrazo")
  {

    servo_antebrazo.write(angulo);

  }

  else if (servo == "base")
  {

    servo_base.write(angulo);

  }
  
  printf("\n\nnEl servo %s se ha movido %d\n\n",servo,angulo);

}


void setup()
{
  
  servo_garra.attach(SERVO_1,544,2400);
  servo_muneca_pitch.attach(SERVO_2);
  servo_muneca_yaw.attach  (SERVO_3);
  servo_codo.attach        (SERVO_4);
  servo_antebrazo.attach   (SERVO_5);
  servo_base.attach        (SERVO_6);
  

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

          startVarName = currentLine.indexOf("varName=");
          endVarName = currentLine.indexOf("&", startVarName);
          startVarValue = currentLine.indexOf("varValue=", endVarName);
          endVarValue = currentLine.indexOf(" ", startVarValue);

          // Extraer los valores de varName y varValue
          varName = currentLine.substring(startVarName + 8, endVarName);
          varValue = currentLine.substring(startVarValue + 9, endVarValue);

          // Ahora varName y varValue contienen los valores de las variables en la URL          
          moverServo(varValue.toInt(), varName);
        }
      }
    }

    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");

  }
}
