#include <stdint.h>
#include "time.h"

enum{
    SALIDA_MANUAL,
    SALIDA_AUTO,
}output_mode;

enum{
    OFF,
    ON
}output_status;



class salida{
    public:
        void init(uint8_t pin_number, uint8_t modo_salida);
        void setCurrentStatus(uint8_t status){
            current_status = status;
        }
        uint8_t getCurrentStauts(void){
            return current_status;
        }
    protected:
        uint8_t current_status = OFF;
        uint8_t 
        struct tm auto_next_ON_time;
        struct tm auto_next_OFF_time;
};