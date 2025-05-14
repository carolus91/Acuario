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