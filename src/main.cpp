#include <Arduino.h>

//Bus I2C
#include <Wire.h>
//Includes para la pantalla OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
//Conexión WiFi
#include <WiFi.h>
//Menus
#include <menus.h>
//ADC ADS1115
#include <ADS1115_WE.h>
#include <FS.h>
#include <SD.h>
#include <math.h>

#define ADC_ADDRESS 0x48
#define OLED_ADDRESS 0x3c

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64

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
float readChannel(ADS1115_MUX channel);
float readNTC (void);
void ADC_Init (void);

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
ADS1115_WE adc = ADS1115_WE(ADC_ADDRESS);

//Menu
uint8_t item_selected_main = 0;            // qué item en el menu esta seleccionado
uint8_t item_selected_datos = 0;
uint8_t item_selected_graficos = 0;
uint8_t current_menu = menu_principal;
uint8_t current_menu_items = NUM_ITEMS_MAIN;
uint8_t current_selected_item = item_selected_main;

//Variables WiFi
const char *ssid = "MOVISTAR_28AC";
const char *password = "n22x53Wmik8M3XagaqX7";
//const char *ssid = "Taller";
//const char *password = "fQh6*34&h7t?";

//Variables NTC
float temperatura = 0.0;
const int Rc = 10000;
const float A = 1.11492089e-3;
const float B = 2.372075385e-4;
const float C = 6.954079529e-8;

const float K = 2.5; //factor de disipacion en mW/C

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

  server.begin();

  //Inicializa la pantalla
  if(!display.begin(OLED_ADDRESS, true)){
    Serial.println(F("SSD1306 allocation failed"));
   for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
 
  printMenu(current_menu, item_selected_main, current_menu_items);
  
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

//  pinMode(OUTPUT_1, OUTPUT);
//  pinMode(OUTPUT_2, OUTPUT);
//  pinMode(OUTPUT_3, OUTPUT);
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
    menuHandleUp(current_menu_items);
  }

  if (digitalRead(BUTTON_DOWN_PIN) == LOW) { // Boton Abajo es presionado
    delay(220);
    menuHandleDown(current_menu_items);
  }

  if (digitalRead(BUTTON_BACK_PIN) == LOW) { // Botón IZQUIERDA es presionado
    delay(220);
    menuHandleBack(current_menu);
  }
  
  if (digitalRead(BUTTON_OK_PIN) == LOW) { // Boton OK es presionado
    while(digitalRead(BUTTON_OK_PIN) == LOW);
    switch(current_menu){
      case menu_principal:
      switch(current_selected_item){
        case 0:
        //Data menu
          current_menu = menu_datos;
          current_selected_item = 0;
          current_menu_items = NUM_ITEMS_DATOS;
          temperatura = readNTC();
        break;
        case 1:
          //Graph menu
          current_menu = menu_graficos;
          current_selected_item = 0;
          current_menu_items = NUM_ITEMS_GRAFICOS;
        break;
        case 2:
          //Menu red
          current_menu = menu_red;
          current_selected_item = 0;
          current_menu_items = 3;
        break;
        case 3:
        break;
        default:
        break;
      }
      break;
      case menu_datos:
      current_menu = menu_datos;
      current_selected_item = 0;
      current_menu_items = NUM_ITEMS_DATOS;
      break;
      case menu_graficos:
      break;
      case menu_red:
      break;
      case menu_info:
      break;
      default:
      break;
    }
    printMenu(current_menu, current_selected_item, current_menu_items);
  }
  if(current_menu == menu_datos){
    currentMillis = millis();
    if((unsigned long)(currentMillis - previousMillis) >= interval){
      temperatura = readNTC();
      printMenu(current_menu, current_selected_item, current_menu_items);
      previousMillis = millis();
    }
  }
}

void changeMenu(uint8_t name)
{

}

void menuHandleUp(uint8_t max_menu_items)
{
    if(current_selected_item == 0){
      current_selected_item = max_menu_items-1;
    }else{
      current_selected_item--;
    }
    printMenu(current_menu, current_selected_item, current_menu_items);
}

void menuHandleDown(uint8_t max_menu_items)
{
    if(current_selected_item == (max_menu_items-1)){
      current_selected_item = 0;
    }else{
      current_selected_item++;
    }
    printMenu(current_menu, current_selected_item, current_menu_items);
}

void menuHandleBack(uint8_t name)
{
    switch(name){
      case menu_datos:
          current_menu = menu_principal;
          current_selected_item = 0;
          current_menu_items = NUM_ITEMS_MAIN;
      break;
      case menu_graficos:
          current_menu = menu_principal;
          current_selected_item = 0;
          current_menu_items = NUM_ITEMS_MAIN;
      break;
      case menu_salidas:
          current_menu = menu_principal;
          current_selected_item = 0;
          current_menu_items = NUM_ITEMS_MAIN;
      case menu_red:
          current_menu = menu_principal;
          current_selected_item = 0;
          current_menu_items = NUM_ITEMS_MAIN;
      break;
      case menu_info:
          current_menu = menu_principal;
          current_selected_item = 0;
          current_menu_items = NUM_ITEMS_MAIN;
      break;
      default:
      break;
    }
    printMenu(current_menu, current_selected_item, current_menu_items);
}

void menuHandleEnter(uint8_t name, uint8_t selected_item)
{

}

void printMenu(uint8_t name, uint8_t selected_item, uint8_t max_menu_items)
{
    display.clearDisplay();                               //Clear Display.
    display.setTextColor(SH110X_WHITE);                   //Set text color.
    
    //Display the menu texts:
    switch(name){
      case menu_principal:
        for(uint8_t i = 0; i < NUM_ITEMS_MAIN; i++){
            display.setCursor(4,3+(13*i));
            display.println(main_menu_items[i]);
        }
      break;
      case menu_datos:
        for(uint8_t i = 0; i < NUM_ITEMS_DATOS; i++){
            display.setCursor(4,3+(13*i));
            if(i == 0){
                display.print(datos_menu_items[i]);
                display.setCursor(95, 3+(13*i));
                display.print(temperatura);   
            }else{
                display.println(datos_menu_items[i]);
            }
            
        }
      break;

      case menu_graficos:

      break;
      
      case menu_salidas:
        for(uint8_t i = 0; i < NUM_ITEMS_SALIDAS; i++){
            display.setCursor(4,3+(13*i));
            display.println(salidas_menu_items[i]);
        }
      break;
      
      case menu_red:
        display.setCursor(4, 3);
        display.println(ssid);
        display.setCursor(4, 17);
        display.println(WiFi.localIP());
        display.setCursor(4, 30);
        if(WiFi.status() != WL_CONNECTED){
          display.println("Sin conexion");
        }else{
          display.println("Conectado");
        }
       
      break;
      case menu_info:

      break;
      default:
      break;
    }
    //Display the lines dividing
    display.drawFastHLine(0,0,127,SH110X_WHITE);          //First line will be always displayed in list menus.
    for(uint8_t i = 1; i <= max_menu_items; i++){
      //display.setCursor(2,1+(10*i));
      display.drawFastHLine(0,(13*i),127,SH110X_WHITE);
      if(selected_item == i-1){
        //Wider lanes to remark the selected item
        display.drawFastHLine(0,(1+(13*(i-1))),127,SH110X_WHITE);
        display.drawFastVLine(0,13*(i-1),13,SH110X_WHITE);
        display.drawFastVLine(127,13*(i-1),13,SH110X_WHITE);
        display.drawFastHLine(0,(13*i)-1,127,SH110X_WHITE);
      }
    }
    display.display();
}

void updateMenu(uint8_t name, uint8_t selected_item, uint8_t max_menu_items)
{
   //Display the lines dividing
    display.drawFastHLine(0,0,127,SH110X_WHITE);          //First line will be always displayed in list menus.
    for(uint8_t i = 1; i <= NUM_ITEMS_MAIN; i++){
      //display.setCursor(2,1+(10*i));
      display.drawFastHLine(0,(13*i),127,SH110X_WHITE);
      if(selected_item == i-1){
        //Wider lanes to remark the selected item
        display.drawFastHLine(0,(1+(13*(i-1))),127,SH110X_WHITE);
        display.drawFastVLine(0,13*(i-1),13,SH110X_WHITE);
        display.drawFastVLine(127,13*(i-1),13,SH110X_WHITE);
        display.drawFastHLine(0,(13*i)-1,127,SH110X_WHITE);
      }
    }
    display.display();
}

void ADC_Init (void)
{
   if(!adc.init()){
    Serial.println("ADS1115 not connected!");
  }

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
  adc.setVoltageRange_mV(ADS1115_RANGE_2048); //comment line/change parameter to change range

  /* Set the inputs to be compared
   *  
   *  ADS1115_COMP_0_1    ->  compares 0 with 1 (default)
   *  ADS1115_COMP_0_3    ->  compares 0 with 3
   *  ADS1115_COMP_1_3    ->  compares 1 with 3
   *  ADS1115_COMP_2_3    ->  compares 2 with 3
   *  ADS1115_COMP_0_GND  ->  compares 0 with GND
   *  ADS1115_COMP_1_GND  ->  compares 1 with GND
   *  ADS1115_COMP_2_GND  ->  compares 2 with GND
   *  ADS1115_COMP_3_GND  ->  compares 3 with GND
   */
  adc.setCompareChannels(ADS1115_COMP_0_GND); //comment line/change parameter to change channel

  /* Set number of conversions after which the alert pin asserts
   * - or you can disable the alert 
   *  
   *  ADS1115_ASSERT_AFTER_1  -> after 1 conversion
   *  ADS1115_ASSERT_AFTER_2  -> after 2 conversions
   *  ADS1115_ASSERT_AFTER_4  -> after 4 conversions
   *  ADS1115_DISABLE_ALERT   -> disable comparator / alert pin (default) 
   */
  //adc.setAlertPinMode(ADS1115_ASSERT_AFTER_1); //uncomment if you want to change the default

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
  // adc.setConvRate(ADS1115_8_SPS); //uncomment if you want to change the default

  /* Set continuous or single shot mode:
   * 
   *  ADS1115_CONTINUOUS  ->  continuous mode
   *  ADS1115_SINGLE     ->  single shot mode (default)
   */
  adc.setMeasureMode(ADS1115_CONTINUOUS); //comment line/change parameter to change mode

   /* Choose maximum limit or maximum and minimum alert limit (window) in Volt - alert pin will 
   *  assert when measured values are beyond the maximum limit or outside the window 
   *  Upper limit first: setAlertLimit_V(MODE, maximum, minimum)
   *  In max limit mode the minimum value is the limit where the alert pin assertion will be  
   *  cleared (if not latched)  
   * 
   *  ADS1115_MAX_LIMIT
   *  ADS1115_WINDOW
   * 
   */
  //adc.setAlertModeAndLimit_V(ADS1115_MAX_LIMIT, 3.0, 1.5); //uncomment if you want to change the default
  
  /* Enable or disable latch. If latch is enabled the alert pin will assert until the
   * conversion register is read (getResult functions). If disabled the alert pin assertion will be
   * cleared with next value within limits. 
   *  
   *  ADS1115_LATCH_DISABLED (default)
   *  ADS1115_LATCH_ENABLED
   */
  //adc.setAlertLatch(ADS1115_LATCH_ENABLED); //uncomment if you want to change the default

  /* Sets the alert pin polarity if active:
   *  
   * ADS1115_ACT_LOW  ->  active low (default)   
   * ADS1115_ACT_HIGH ->  active high
   */
  //adc.setAlertPol(ADS1115_ACT_LOW); //uncomment if you want to change the default
 
  /* With this function the alert pin will assert, when a conversion is ready.
   * In order to deactivate, use the setAlertLimit_V function  
   */
  //adc.setAlertPinToConversionReady(); //uncomment if you want to change the default

//  Serial.println("ADS1115 Example Sketch - Continuous Mode");
//  Serial.println("All values in volts");
//  Serial.println();
}

float readNTC (void)
{
    float V = 0.0;
    double temp = 0.0;
    V = readChannel(ADS1115_COMP_0_GND);

    float R = (Rc * V) / (3.3 - V);
    float logR  = log(R);
    float R_th = 1.0 / (A + B * logR + C * logR * logR * logR );

    float kelvin = R_th - V*V/(K * R)*1000;
    float celsius = kelvin - 273.15;
    
    return celsius;
}

float readChannel(ADS1115_MUX channel) {
  float voltage = 0.0;
  adc.setCompareChannels(channel);
  voltage = adc.getResult_V(); // alternative: getResult_mV for Millivolt
  return voltage;
}

/*void updateText(){
  display.fillRect(3, 0, 115, 20, SH110X_BLACK);
  display.fillRect(3, 24, 115, 20, SH110X_BLACK);
  display.fillRect(3, 48, 115, 20, SH110X_BLACK);
  display.fillRect(125, 0, 3, 64, SH110X_BLACK);
}*/