#include <stdint.h>

//Definiciones para los menus

#define MAX_ITEM_LENGTH  20
#define NUM_ITEMS_MAIN  4
#define NUM_ITEMS_DATOS  4
#define NUM_ITEMS_GRAFICOS  4

enum{
	menu_principal,
	menu_datos,
	menu_graficos,
	menu_red,
	menu_info
}menu_name;

//Definición de los distintos menus
const char main_menu_items [NUM_ITEMS_MAIN][MAX_ITEM_LENGTH] = {
	{ "Datos" },
	{ "Graficos" },
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

 /**********************************************************************/
  /*!
    @brief   Change to the menu in "name"
    @param   name   one of the names defined in the "menu_name" enum
  */
  /**********************************************************************/
void changeMenu(uint8_t name);

 /**********************************************************************/
  /*!
    @brief   Manage the UP button press on any menu.
    @param   max_menu_items  number of menu items in the menu with name "name"
  */
  /**********************************************************************/
void menuHandleUp(uint8_t max_menu_items);

  /**********************************************************************/
  /*!
    @brief   Manage the DOWN button press on any menu.
    @param   max_menu_items  number of menu items in the menu with name "name"
  */
  /**********************************************************************/
void menuHandleDown(uint8_t max_menu_items);

 /**********************************************************************/
  /*!
    @brief   Manage the ENTER button press on any menu.
    @param   name   one of the names defined in the "menu_name" enum
    @param   selected_item  currently selected item
  */
  /**********************************************************************/
void menuHandleEnter(uint8_t name, uint8_t selected_item);

 /**********************************************************************/
  /*!
    @brief   Print current menu in display
    @param   name   one of the names defined in the "menu_name" enum 
    @param   selected_item   currently selected tiem
    @param   max_menu_items  number of menu items in the menu with name "name"
  */
  /**********************************************************************/
void printMenu(uint8_t name, uint8_t selected_item, uint8_t max_menu_items);

/**********************************************************************/
  /*!
    @brief   Update selection in "name" menu
    @param   name   one of the names defined in the "menu_name" enum 
    @param   selected_item   currently selected tiem
    @param   max_menu_items  number of menu items in the menu with name "name"
  */
  /**********************************************************************/
void updateMenu(uint8_t name, uint8_t selected_item, uint8_t max_menu_items);

/**********************************************************************/
  /*!
    @brief   Update selection in "name" menu
    @param   name   one of the names defined in the "menu_name" enum 
  */
  /**********************************************************************/
void menuHandleBack(uint8_t name);