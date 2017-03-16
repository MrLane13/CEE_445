#include<iostream>
#include<fstream>
#include<string>
using namespace std;

#define LED0_PATH "/sys/class/leds/beaglebone:green:usr0"

void removeTrigger() {
    // remove the trigger from the LEd
    std::fstream fs;
    fs.open( LED0_PATH "/trigger", std::fstream::out);
    fs << "none";
    fs.close();
}

int main(int argc, char* argv[]) {
    if(argc!=2) {
        cout << "Usage is makeLED and one of: on, off, flash or status" <<endl;
        cout << "e.g. makeLED flash" << endl;
    }
    
    string cmd(argv[1]);
    std::fstream fs;
    cout << "Starting the LED flash program" << endl;
    cout << "The LED Path is: " << LED0_PATH << endl;
    
    // select whether it is on, off or flash or status
    if(cmd=="on") {
        removeTrigger();
        fs.open(LED0_PATH "/brightness", std::fstream::out);
        fs << "1";
        fs.close();
    } else if(cmd=="off") {
        removeTrigger();
        fs.open(LED0_PATH "/brightness", std::fstream::out);
        fs << "0";
        fs.close();
    } else if(cmd=="flash") {
        removeTrigger();
        fs.open(LED0_PATH "/trigger", std::fstream::out);
        fs << "timer";
        fs.open(LED0_PATH "/delay_on", std::fstream::out);
        fs << "50";
        fs.open(LED0_PATH "/delay_off", std::fstream::out);
        fs << "50";
        fs.close();
    } else if(cmd=="status") {
        removeTrigger();
        fs.open(LED0_PATH "/trigger", std::fstream::in);
        string line;
        while(getline(fs,line)) cout << line;
        fs.close();
    } else {
        cout << "Invalid command" << endl;
    }
    cout << "Finish the LED flash program" << endl;
    return 0;
}