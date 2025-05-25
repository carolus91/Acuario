#include <math.h>
#include <Medida.h>


void medida::init(uint8_t tipo, uint8_t canal, uint8_t unidad){
    switch (tipo){
        case temperatura:
            setUnit(unidad);
            setChannel(canal);
        break;
        case pH:
        break;
        default:
        break;
    }
}

void medida::measure(){
    switch (tipo_medida){
        case temperatura:
            //Es un sensor NTC
            if(canal == 1){
                voltage = readChannel(ADS1115_COMP_0_GND);
            }else{
                voltage = readChannel(ADS1115_COMP_1_GND);
            }
            R = (Rc * voltage) / (3.3 - voltage);
            logR  = log(R);
            R_th = 1.0 / (A + B * logR + C * logR * logR * logR );
            kelvin = R_th - voltage*voltage/(K * R)*1000;
            celsius = kelvin - 273.15;
            fahrenheit = (celsius* 9.0/5.0) + 32.0;
            switch (unidad){
                case Celsius:
                    current_value = celsius + getMeasureOffset();
                break;
                case Kelvin:
                    current_value = kelvin + getMeasureOffset();
                break;
                case Fahrenheit:
                    current_value = fahrenheit + (getMeasureOffset() * 9.0/5.0) + 32;
                break;
                default:
                    current_value = celsius;
                break;
            }
        break;
        case pH:
        break;
        default:
        break;
    }
}

float medida::readChannel(ADS1115_MUX channel){
    adc.setCompareChannels(channel);
    /*
    adc.startSingleMeasurement();
    while(adc.isBusy()){
        //TODO: no mola que bloquee el programa.
    }
    */
    voltage = adc.getResult_V(); // alternative: getResult_mV for Millivolt
    return voltage;
}