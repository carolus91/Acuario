#include <stdint.h>

//Definiciones para los menus

#define MAX_ITEM_LENGTH       20
#define NUM_ITEMS_MAIN        6
#define NUM_ITEMS_DATOS       3
#define NUM_ITEMS_GRAFICOS    3
#define NUM_ITEMS_SALIDAS     4
#define NUM_ITEMS_ENTRADAS    4
#define NUM_ITEMS_RED         3

enum{
	menu_principal,
	menu_datos,
	menu_graficos,
  menu_entradas,
  menu_salidas,
	menu_red,
	menu_info
}menu_name;

enum{
  datos,
  graficos,
  entradas,
  salidas,
  red,
  info
}menu_principal_items;

enum{
  Temperatura_1,
  Temperatura_2,
  PH
}menu_datos_items;

enum{
  Salida_1,
  Salida_2,
  Salida_3,
  Salida_4
}menu_salidas_items;

enum{
  Entrada_1,
  Entrada_2,
  Entrada_3,
  Entrada_4
}menu_entradas_items;

//Definición de los distintos menus
const char main_menu_items [NUM_ITEMS_MAIN][MAX_ITEM_LENGTH] = {
	{ "Datos" },
	{ "Graficos" },
  { "Salidas" },
  { "Entradas" },
	{ "Red" },
	{ "Info" },
};

const char datos_menu_items [NUM_ITEMS_DATOS][MAX_ITEM_LENGTH] = {
	{"Temp. Agua:"},
	{"Temp. Amb:"},
	{"PH:"}
};

const char graficos_menu_items [NUM_ITEMS_GRAFICOS][MAX_ITEM_LENGTH] = {
	{"Temp. Agua:"},
	{"Temp. Amb:"},
	{"PH:"}
};

const char salidas_menu_items [NUM_ITEMS_SALIDAS][MAX_ITEM_LENGTH] = {
	{"Salida 1"},
	{"Salida 2"},
	{"Salida 3"},
  {"Salida 4"}
};

const char entradas_menu_items [NUM_ITEMS_ENTRADAS][MAX_ITEM_LENGTH] = {
	{"Entrada 1"},
	{"Entrada 2"},
	{"Entrada 3"},
  {"Entrada 4"}
};

const char red_menu_items [NUM_ITEMS_RED][MAX_ITEM_LENGTH] = {
	{"RED:"},
	{"IP:"},
	{"STAT:"}
};

class Acuario_Menu{
  public:
    uint8_t getCurrentMenuName(void){
      return current_menu_name;
    }
    
    void setCurrentMenuName(uint8_t menu_name);
    
    uint8_t getPreviousMenuName(void){
      return previous_menu_name;
    }
    
    void setPreviousMenuName(uint8_t menu_name);
    
    uint8_t getCurrentMenuItems(void){
      return current_menu_item_number;
    }
    
    void setCurrentMenuItems(uint8_t items);
    
    uint8_t getCurrentMenuItemSelected(void){
      return current_menu_item_selected;
    }

    void setCurrentMenuItemSelected(uint8_t item_selected);

    void initMenu(void);

    void menuHandleDown(void);
    void menuHandleUp(void);
    void menuHandleEnter(void);
    void menuHandleBack(void);

    void printCurrentMenu(void);
    void updateCurrentMenu(void);
    void refreshCurrentMenu(void);

    void printEditPopup(float data);
    void printDatePopup(uint8_t Day, uint8_t Month, uint8_t Year);
    void printTimePopup(uint8_t Hours, uint8_t Minutes, uint8_t Seconds);
    void printTrailingText(uint8_t item_position, char* text);
    void printTrailingIntNumber(uint8_t item_position, int number);
    void printTrailingFloatNumber(uint8_t item_position, double number, uint8_t dec_pos);

  protected:
    uint8_t current_menu_name;
    uint8_t previous_menu_name;
    uint8_t current_menu_item_number;
    uint8_t current_menu_item_selected;   
    uint8_t editing_flag = 0; 
};
 