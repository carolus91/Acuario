#include "menus.h"

//Includes para la pantalla OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64

//Definición de la pantalla
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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

/// @brief Asigna el nombre del menú actual
/// @param menu_name uno de los posibles nombres de los menus (enums)
void Acuario_Menu::setCurrentMenuName(uint8_t menu_name)
{

}

void Acuario_Menu::setPreviousMenuName(uint8_t menu_name)
{

}

void Acuario_Menu::setCurrentMenuItems(uint8_t items)
{

}

void Acuario_Menu::setCurrentMenuItemSelected(uint8_t item_selected)
{

}

void Acuario_Menu::initMenu(void)
{

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
    display.clearDisplay();                               //Clear Display.
    display.setTextColor(SH110X_WHITE);                   //Set text color.
    
    //Display the menu texts:
    switch(current_menu_name){
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