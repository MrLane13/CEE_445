#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

#define LDR_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"

int readAnalog(int number) {
    stringstream ss;
    ss << LDR_PATH << number << "_raw";
    fstream fs;
    fs.open(ss.str().c_str(), fstream::in);
    fs >> number;
    fs.close();
    return number;
}

float getTemperature(int adc_value) {
    //float cur_voltage = adc_value * (5.0f/2048.0f);
    float cur_voltage = adc_value * (1.80f/4096.0f);
    float diff_degreesC = (cur_voltage-0.75f)/(0.01f);
    return (25.0f + diff_degreesC);
}

int main(int argc, char* argv[]) {
    int value = readAnalog(0);
    int temp = getTemperature(value);
    //cout << temp;   // This is in degrees Celsius
    int myVar = temp*1.8;
    int finalTempF = myVar+32;
    cout << finalTempF;// This is in degrees Fahrenheit
    //cout << "Th ADC value was " << value << " out of 4096." << endl;
    //cout << "The temperature is: " << temp << " degrees Celcius." << endl;
    //cout << "The temperature is: " << temp*(9/5)+32 << " degrees F" << endl;
    return 0;
}
