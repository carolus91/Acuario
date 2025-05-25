#include <Arduino.h>

//Bus I2C
#include <Wire.h>
//Includes para la pantalla OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
//Conexión WiFi
#include <WiFi.h>
//Hora y fecha
#include "time.h"
//Menus
#include <menus.h>
//Medidas
#include <Medida.h>

#include <FS.h>
#include <SD.h>
#include <math.h>
//Tests
#include <ESP.h>

#include <ADS1115_WE.h>

#define ADC_ADDRESS 0x48

#define BUTTON_UP_PIN       12  //GPIO para el botón "arriba"
#define BUTTON_DOWN_PIN     14 //GPIO para el botón "abajo"
#define BUTTON_OK_PIN       25 //GPIO para el botón "OK"
#define BUTTON_BACK_PIN     27 //GPIO para el botón "BACK"
#define BUTTON_RIGHT_PIN    26 //GPIO para el botón "RIGHT"

#define INPUT_1             13 //Entrada aislada 1
#define INPUT_2             33 //Entrada aislada 2
#define INPUT_3             32 //Entrada aislada 3
#define INPUT_4             35 //Entrada aislada 4

#define OUTPUT_1            36 //Salida relé 1
#define OUTPUT_2            39 //Salida relé 2
#define OUTPUT_3            34 //Salida relé 3
#define OUTPUT_4            4  //Salida relé 4

#define ENABLE_SD_SUPP      16 //Habilita alimentación SD.

//Funciones
void printLocalTime(void);
void ADC_Init(void);

ADS1115_WE adc = ADS1115_WE(ADC_ADDRESS);

Acuario_Menu Menu;


medida temperaturaAgua;
medida temperaturaExterna;

//Variables WiFi
const char *ssid = "MOVISTAR_28AC";
const char *password = "n22x53Wmik8M3XagaqX7";

//Variables Hora/Fecha
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

//Variables temporización.
unsigned long interval = 1000;
unsigned long previousMillis;
unsigned long currentMillis;

WiFiServer server(80);
WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Conection to the WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //Actualización fecha/hora
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  // Get the size of the flash memory
  uint32_t flash_size = ESP.getFlashChipSize();
  
  Serial.print("Flash size: ");
  Serial.print(flash_size);
  Serial.println(" bytes");

  server.begin();

  Menu.initMenu();

  temperaturaAgua.init(temperatura, 2, Celsius);
  temperaturaExterna.init(temperatura, 1, Celsius);
   
  ADC_Init();
  
  //Digital pin configurations
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_OK_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

  pinMode(INPUT_1, INPUT_PULLDOWN);
  pinMode(INPUT_2, INPUT_PULLDOWN);
  pinMode(INPUT_3, INPUT_PULLDOWN);
  pinMode(INPUT_4, INPUT_PULLDOWN);

  pinMode(OUTPUT_4, OUTPUT);

  pinMode(ENABLE_SD_SUPP,OUTPUT);

  //Tests SD
  digitalWrite(ENABLE_SD_SUPP, HIGH);
  if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
  }

  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
  
  previousMillis = millis();
  currentMillis = previousMillis;
}

void loop() {
  // put your main code here, to run repeatedly:
  //If there is a client connection, button will not be available.
  client = server.accept();
  if(client){
    String currentLine = "";
    Serial.println("New Client.");  // print a message out the serial port
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/1H\">here</a> to turn OUTPUT 1 on.<br>");
            client.print("Click <a href=\"/1L\">here</a> to turn OUTPUT 1 off.<br>");
            client.print("Click <a href=\"/2H\">here</a> to turn OUTPUT 2 on.<br>");
            client.print("Click <a href=\"/2L\">here</a> to turn OUTPUT 2 off.<br>");
            client.print("Click <a href=\"/3H\">here</a> to turn OUTPUT 3 on.<br>");
            client.print("Click <a href=\"/3L\">here</a> to turn OUTPUT 3 off.<br>");
            client.print("Click <a href=\"/4H\">here</a> to turn OUTPUT 4 on.<br>");
            client.print("Click <a href=\"/4L\">here</a> to turn OUTPUT 4 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /1H")) {
          digitalWrite(OUTPUT_1, HIGH);  // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /1L")) {
          digitalWrite(OUTPUT_1, LOW);  // GET /L turns the LED off
        }
                // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /2H")) {
          digitalWrite(OUTPUT_2, HIGH);  // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /2L")) {
          digitalWrite(OUTPUT_2, LOW);  // GET /L turns the LED off
        }
                // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /3H")) {
          digitalWrite(OUTPUT_3, HIGH);  // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /3L")) {
          digitalWrite(OUTPUT_3, LOW);  // GET /L turns the LED off
        }
                // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /4H")) {
          digitalWrite(OUTPUT_4, HIGH);  // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /4L")) {
          digitalWrite(OUTPUT_4, LOW);  // GET /L turns the LED off
        }
      }
    }
     // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }


  if (digitalRead(BUTTON_UP_PIN) == LOW) { // Boton Arriba es presionado
    delay(220);
    Menu.menuHandleUp();
  }

  if (digitalRead(BUTTON_DOWN_PIN) == LOW) { // Boton Abajo es presionado
    delay(220);
    Menu.menuHandleDown();
  }

  if (digitalRead(BUTTON_BACK_PIN) == LOW) { // Botón IZQUIERDA es presionado
    delay(220);
    Menu.menuHandleBack();
  }
  
  if (digitalRead(BUTTON_OK_PIN) == LOW) { // Boton OK es presionado
    while(digitalRead(BUTTON_OK_PIN) == LOW);
    Menu.menuHandleEnter();
  }

  //Actualización de valores cuando está en la pantalla principal.
  if(Menu.getCurrentMenuName() == pantalla_principal){
    if(millis() - previousMillis > interval){
      previousMillis = millis();
      temperaturaAgua.measure();
      temperaturaExterna.measure();
      Menu.printCurrentMenu();
      }  
  }
}

void ADC_Init(void){
      //Inicializar el ADC
      adc.init();
      /* Set the voltage range of the ADC to adjust the gain
      * Please note that you must not apply more than VDD + 0.3V to the input pins!
      * 
      * ADS1115_RANGE_6144  ->  +/- 6144 mV
      * ADS1115_RANGE_4096  ->  +/- 4096 mV
      * ADS1115_RANGE_2048  ->  +/- 2048 mV (default)
      * ADS1115_RANGE_1024  ->  +/- 1024 mV
      * ADS1115_RANGE_0512  ->  +/- 512 mV
      * ADS1115_RANGE_0256  ->  +/- 256 mV
      */
      adc.setVoltageRange_mV(ADS1115_RANGE_2048);
      /* Set the conversion rate in SPS (samples per second)
      * Options should be self-explaining: 
      * 
      *  ADS1115_8_SPS 
      *  ADS1115_16_SPS  
      *  ADS1115_32_SPS 
      *  ADS1115_64_SPS  
      *  ADS1115_128_SPS (default)
      *  ADS1115_250_SPS 
      *  ADS1115_475_SPS 
      *  ADS1115_860_SPS 
      */
      adc.setConvRate(ADS1115_8_SPS); //uncomment if you want to change the default            
      /* Set continuous or single shot mode:
      * 
      *  ADS1115_CONTINUOUS  ->  continuous mode
      *  ADS1115_SINGLE     ->  single shot mode (default)
      */
      adc.setMeasureMode(ADS1115_CONTINUOUS); //comment line/change parameter to change mode            
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}