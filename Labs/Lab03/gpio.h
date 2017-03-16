/* GPIO.h  */

#ifndef GPIO_H_
#define GPIO_H_
#include<string>
#include<fstream>
using std::string;
using std::ofstream;

#define GPIO_PATH "/sys/class/gpio/"

namespace exploringBB {

typedef int (*CallbackType)(int);
enum GPIO_DIRECTION{ INPUT, OUTPUT };
enum GPIO_VALUE{ LOW=0, HIGH=1 };
enum GPIO_EDGE{ NONE, RISING, FALLING, BOTH };

class GPIO {
private:
	int number, debounceTime;
	string name, path;

public:
	GPIO(int number); //constructor will export the pin
	virtual int getNumber() { return number; }

	// General Input and Output Settings
	// virtual functions are functions in the Base class 
	//that you can override in derived classed
	//example: animal eats generic food and cat eats generic food
	//but if cat eats rats. we can use virtual void eat() --
	virtual int setDirection(GPIO_DIRECTION);
	virtual GPIO_DIRECTION getDirection();
	virtual int setValue(GPIO_VALUE);
	virtual int toggleOutput();
	virtual GPIO_VALUE getValue();
	virtual int setActiveLow(bool isLow=true);  //low=1, high=0
	virtual int setActiveHigh(); //default
	//software debounce input (ms) - default 0
	virtual void setDebounceTime(int time) { this->debounceTime = time; }

	// Advanced OUTPUT: Faster write by keeping the stream alive (~20X)
	virtual int streamOpen();
	virtual int streamWrite(GPIO_VALUE);
	virtual int streamClose();

	virtual int toggleOutput(int time); //threaded invert output every X ms.
	virtual int toggleOutput(int numberOfTimes, int time);
	virtual void changeToggleTime(int time) { this->togglePeriod = time; }
	virtual void toggleCancel() { this->threadRunning = false; }

	// Advanced INPUT: Detect input edges; threaded and non-threaded
	virtual int setEdgeType(GPIO_EDGE);
	virtual GPIO_EDGE getEdgeType();
	virtual int waitForEdge(); // waits until button is pressed
	virtual int waitForEdge(CallbackType callback); // threaded with callback
	virtual void waitForEdgeCancel() { this->threadRunning = false; }

	virtual ~GPIO();  //destructor will unexport the pin

private:
	int write(string path, string filename, string value);
	int write(string path, string filename, int value);
	string read(string path, string filename);
	int exportGPIO();
	int unexportGPIO();
	ofstream stream;
	pthread_t thread;
	CallbackType callbackFunction;
	bool threadRunning;
	int togglePeriod;  //default 100ms
	int toggleNumber;  //default -1 (infinite)
	friend void* threadedPoll(void *value);
	friend void* threadedToggle(void *value);
};

void* threadedPoll(void *value);
void* threadedToggle(void *value);

} /* namespace exploringBB */

#endif /* GPIO_H_ */
