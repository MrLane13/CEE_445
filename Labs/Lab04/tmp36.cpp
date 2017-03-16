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
    float cur_voltage = adc_value * (1.80f/4096.0f);
    float diff_degreesC = (cur_voltage-0.75f)/(0.01f);
    return (25.0f + diff_degreesC);
}

int main(int argc, char* argv[]) {
    int val1 = 0;
    cout << "Starting the temperature sensor program" << endl;
    if(argc>1) val1 = atoi(argv[1]);
    int value = readAnalog(0);
    int temp = getTemperature(value);
    cout << "Th ADC value was " << value << " out of 4096." << endl;
    cout << "The temperature is: " << temp << " degrees Celcius." << endl;
    return 0;
}