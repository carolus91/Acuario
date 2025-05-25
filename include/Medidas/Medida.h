#include <stdint.h>
#include <ADS1115_WE.h>

extern ADS1115_WE adc;

enum{
    temperatura,
    pH
}tipo_medida;

enum{
    Celsius,
    Kelvin,
    Fahrenheit
}unidades;

class medida{
    public:
        void init(uint8_t tipo, uint8_t channel, uint8_t unidad);
        void measure();
        void setMeasureOffset(float offset);
        float getMeasureOffset(void){
            return offset;
        }
        float getMeasureValue(void){
            return current_value;
        }
        void setUnit(uint8_t unit){
            unidad = unit;
        }
        uint8_t getUnit(void){
            return unidad;
        }
        void setChannel(uint8_t channel){
            canal = channel;
        }
        uint8_t getChannel(void){
            return canal;
        }
    protected:
        uint8_t unidad;
        float current_value = 0;
        float offset = 0;
        uint8_t tipo_medida;
        uint8_t canal;  
        float readChannel(ADS1115_MUX channel);
        float voltage = 0.0;
        const int Rc = 10000;
        const float A = 1.11492089e-3;
        const float B = 2.372075385e-4;
        const float C = 6.954079529e-8;
        const float K = 2.5; //factor de disipacion en mW/C
        float R;
        float logR;
        float R_th;
        float kelvin;
        float celsius;
        float fahrenheit;
};