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

int main(int argc, char* argv[]) {
    cout << "Starting the readLDR program" << endl;
    int value = readAnalog(0);
    cout << "Th LDR value was " << value << " out of 4095." << endl;
    return 0;
}