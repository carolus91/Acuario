#include "menus.h"

//Includes para la pantalla OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
#define OLED_ADDRESS        0x3C

//Definición de la pantalla
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/*void updateMenu(uint8_t name, uint8_t selected_item, uint8_t max_menu_items)
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
}*/

/// @brief Asigna el nombre del menú actual
/// @param menu_name uno de los posibles nombres de los menus (enums)
void Acuario_Menu::setCurrentMenuName(uint8_t menu_name)
{
  current_menu_name = menu_name;
}

void Acuario_Menu::setPreviousMenuName(uint8_t menu_name)
{
  previous_menu_name = menu_name;
}

void Acuario_Menu::setCurrentMenuItems(uint8_t items)
{
  current_menu_item_number = items;
}

void Acuario_Menu::setCurrentMenuItemSelected(uint8_t item_selected)
{
  current_menu_item_selected = item_selected;
}

void Acuario_Menu::initMenu(void)
{
    //Inicializa la pantalla
  if(!display.begin(OLED_ADDRESS, true)){
    Serial.println(F("SSD1306 allocation failed"));
   for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  current_menu_name = pantalla_principal;
  previous_menu_name = pantalla_principal;
  current_menu_item_number = NUM_ITEMS_MAIN;
  current_menu_item_selected = datos;
  printCurrentMenu();
}

void Acuario_Menu::menuHandleDown(void)
{

}

void Acuario_Menu::menuHandleUp(void)
{

}

void Acuario_Menu::menuHandleEnter(void)
{

}

void Acuario_Menu::menuHandleBack(void)
{

}

void Acuario_Menu::printCurrentMenu(void)
{
    struct tm timeinfo;
    display.clearDisplay();                               //Clear Display.
    display.setTextColor(SH110X_WHITE);                   //Set text color.
    
    //Display the menu texts:
    switch(current_menu_name){
      case pantalla_principal:
        //Caso especial para la pantalla principal (display de datos)
        display.setTextSize(1);
        //Barra horizontal, arriba irá la hora.
        display.setCursor(15,2);
        getLocalTime(&timeinfo);
        //https://cplusplus.com/reference/ctime/strftime/
        display.println(&timeinfo, "%H:%M  %d/%m/%y");
        //display.println("hh:mm:ss dd:mm:yy"); //Hora
        display.drawFastHLine(0,13,127,SH110X_WHITE); //Primera barra horizontal
        //display.drawFastVLine(0,13,38,SH110X_WHITE); //Barra vertical dividiendo
        display.setCursor(16,21);
        display.print("T1");
        display.setCursor(9,35);
        //display.printf("24.2");
        display.drawFastVLine(41,20,24,SH110X_WHITE); //Barra vertical dividiendo
        display.setCursor(59,21);
        display.print("T2");
        display.setCursor(51,35);
        //display.printf("19.6");
        display.drawFastVLine(85,20,24,SH110X_WHITE); //Barra vertical dividiendo
        display.setCursor(101,21);
        display.print("pH");
        display.setCursor(98,35);
        //display.printf("7.8");
        //display.drawFastVLine(127,13,38,SH110X_WHITE); //Barra vertical dividiendo
        display.drawFastHLine(0,51,127,SH110X_WHITE); //Barra horizontal abajo
        for(uint8_t i = 0; i< 4; i++){
          display.fillRoundRect(6+(14*i),55,9,9,1,SH110X_WHITE); //Rectángulo relleno (entrada activada?)
        }
        display.drawFastVLine(64,51,13,SH110X_WHITE); //Barra vertical divisora
        for(uint8_t i = 0; i< 4; i++){
          display.drawRoundRect(72+(14*i),55,9,9,1,SH110X_WHITE); //Rectángulo vacío (salida desactivada?)
        }
      break;
      case menu_principal:
        for(uint8_t i = 0; i < NUM_ITEMS_MAIN; i++){
            display.setCursor(4,3+(13*i));
            display.println(main_menu_items[i]);
        }
      break;
      case menu_datos:
        for(uint8_t i = 0; i < NUM_ITEMS_DATOS; i++){
            display.setCursor(4,3+(13*i));
            display.println(datos_menu_items[i]);            
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
        for(uint8_t i = 0; i < NUM_ITEMS_RED; i++){
            display.setCursor(4,3+(13*i));
            display.println(red_menu_items[i]);
        }
/*       display.setCursor(4, 3);
        display.println(ssid);
        display.setCursor(4, 17);
        display.println(WiFi.localIP());
        display.setCursor(4, 30);
        if(WiFi.status() != WL_CONNECTED){
          display.println("Sin conexion");
        }else{
          display.println("Conectado");
        }
*/    
      break;
      case menu_info:

      break;
      case menu_entradas:
        for(uint8_t i = 0; i < NUM_ITEMS_ENTRADAS; i++){
            display.setCursor(4,3+(13*i));
            display.println(entradas_menu_items[i]);
        }     
      break;

      default:
      break;
    }
    //Display the lines dividing
    if(current_menu_name != pantalla_principal){
      display.drawFastHLine(0,0,127,SH110X_WHITE);          //First line will be always displayed in list menus.
      for(uint8_t i = 1; i <= current_menu_item_number; i++){
        //display.setCursor(2,1+(10*i));
        display.drawFastHLine(0,(13*i),127,SH110X_WHITE);
        if(current_menu_item_selected == i-1){
          //Wider lanes to remark the selected item
          display.drawFastHLine(0,(1+(13*(i-1))),127,SH110X_WHITE);
          display.drawFastVLine(0,13*(i-1),13,SH110X_WHITE);
          display.drawFastVLine(127,13*(i-1),13,SH110X_WHITE);
          display.drawFastHLine(0,(13*i)-1,127,SH110X_WHITE);
        }
      }
    }
    display.display();
}

void Acuario_Menu::updateCurrentMenu(void)
{
   //Display the lines dividing
    display.drawFastHLine(0,0,127,SH110X_WHITE);          //First line will be always displayed in list menus.
    //TODO: hay que mirar a ver cuántos elementos en total va a tener el menú. Se pueden representar hasta 5
    //por cada pantalla. Si hubiera que cambiar de pantalla (items del 5 en adelante) habría que re-escribir el menú
    //contando con eso.

    for(uint8_t i = 1; i <= current_menu_item_number; i++){
      display.drawFastHLine(0,(13*i),127,SH110X_WHITE);
      if(current_menu_item_selected == i-1){
        //Wider lanes to remark the selected item
        display.drawFastHLine(0,(1+(13*(i-1))),127,SH110X_WHITE);
        display.drawFastVLine(0,13*(i-1),13,SH110X_WHITE);
        display.drawFastVLine(127,13*(i-1),13,SH110X_WHITE);
        display.drawFastHLine(0,(13*i)-1,127,SH110X_WHITE);
      }
    }
    display.display();
}

void Acuario_Menu::refreshCurrentMenu(void)
{

}

void Acuario_Menu::printEditPopup(float data)
{

}

void Acuario_Menu::printDatePopup(uint8_t Day, uint8_t Month, uint8_t Year)
{

}

void Acuario_Menu::printTimePopup(uint8_t Hours, uint8_t Minutes, uint8_t Seconds)
{

}

void Acuario_Menu::printTrailingText(uint8_t item_position, char* text)
{

}

void Acuario_Menu::printTrailingIntNumber(uint8_t item_position, int number)
{

}

void Acuario_Menu::printTrailingFloatNumber(uint8_t item_position, double number, uint8_t dec_pos)
{
  
}