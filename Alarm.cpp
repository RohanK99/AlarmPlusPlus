#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <string>
#include <fstream>
#include <thread>
#include <ctype.h>
#include <fcntl.h>
#include <ugpio/ugpio.h>
#include <curl/curl.h>
#include <sstream>

using namespace std;

/*  A
    ___
   F|     |B
 |  G  |
   ___
   E|     |C
 |  D  |
    ___   */

#define A 19
#define B 11
#define C 4
#define D 6
#define E 45
#define F 3
#define G 18
#define speakerPin SPEAKER_PIN_HERE

// ---------------------------------------------- LEDS --------------------------------------- //
//defins the pins that will be used for flashing the leds
#define RED 9
#define BLUE 1
#define YELLOW 3
#define GREEN 2

//initalizes the pins controlling the leds
void initPinsLED()
{

	//Requests access to the pins

    gpio_request(11, NULL);

    gpio_request(1, NULL);

    gpio_request(2, NULL);

    gpio_request(3, NULL);

    //Sets the direction of all of the pins to output, so that the pins can be written to
    gpio_direction_output(1,1);

    gpio_direction_output(2,1);

    gpio_direction_output(3,1);

    gpio_direction_output(11,1);

    gpio_set_value(1,0);
    gpio_set_value(2,0);
    gpio_set_value(3,0);
    gpio_set_value(11,0);

}

void Pwm (char pinNum[], char frequency[], char duty[])
{
	string spinNum = string(pinNum);
	string sfrequency = string(frequency);
	string sduty = string(duty);
	string command = "fast-gpio pwm "+spinNum+" "+sfrequency+" "+sduty;
	system(command.c_str());
}

//clears all of the light, ie writes all of the pins to low
void clearLights()
{
	system("fast-gpio set 1 0");
	system("fast-gpio set 2 0");
	system("fast-gpio set 3 0");
	system("fast-gpio set 11 0");

}

//fades an led from dark to bright to dark quickly
//takes a char* as a parameter, not an int!
void fastFade(char pinNum[])
{
	//fades to bright
	for(int x=0; x<100; x++)
	{
		stringstream ss;
		ss << x;
		string str = ss.str();
		char *cstr = new char[str.length()+1];//converts the int x to a char*
		strcpy(cstr, str.c_str());
		Pwm(pinNum,"50", cstr);//write to the pwm function
		usleep(10000);//sleeps for 0.015 second
		delete [] cstr;
	}

	//fades to dark
	for(int x=100; x<2; x--)
	{
		stringstream ss;
		ss << x;
		string str = ss.str();
		char *cstr = new char[str.length()+1];
		strcpy(cstr, str.c_str());
		Pwm(pinNum,"50", cstr);
		usleep(10000);
		delete [] cstr;
	}

}

//fades an led from dark to bright to dark more slowly
//Takes a char* parameter for pinNum, not int!
void slowFade(char pinNum[])
{
	//fades the leds from dark to bright
	for(int x=0; x<100; x++)
	{
		stringstream ss;
		ss << x;
		string str = ss.str();
		char *cstr = new char[str.length()+1];
		strcpy(cstr, str.c_str());
		Pwm(pinNum,"50", cstr);
		usleep(50000);
		delete [] cstr;
	}

	//fades the leds from bright to dark
	for(int x=100; x<2; x--)
	{
		stringstream ss;
		ss << x;
		string str = ss.str();
		char *cstr = new char[str.length()+1];
		strcpy(cstr, str.c_str());
		Pwm(pinNum,"50", cstr);
		usleep(50000);
		delete [] cstr;
	}

}

//blinks and led quickly
void fastBlink(int pinNum)
{

	gpio_set_value(pinNum,1);
	usleep(100000);
	gpio_set_value(pinNum,0);
	usleep(100000);
	gpio_set_value(pinNum,1);

}

//blinks and led slowly
void slowBlink(int pinNum)
{

	gpio_set_value(pinNum,1);
	sleep(1000000);
	gpio_set_value(pinNum,0);
	sleep(1000000);
	gpio_set_value(pinNum,1);

}

//pattern for rain and sleet
//flashes blue rapidly
void Pattern1()
{
	for(int x = 0; x<10; x++)
	{

		fastBlink(1);

	}
	clearLights();

}

//pattern for flurries and snow
//fades dark and light blue
void Pattern2()
{
	for(int x= 0; x<3; x++)
	{
		slowFade("1");
	}
	clearLights();
}

//pattern for clear and sunny
//bright yellow no fade
void Pattern3()
{
	gpio_set_value(3,1);
	usleep(10000000);
        clearLights();
}

//pattern for mostly cloudy/sunny and partly cloudy/sunny
//fade between yellow and green
void Pattern4()
{
	for(int x =0; x<3; x++)
	{
		slowFade("3");
		slowFade("2");
	}
	clearLights();
}

//pattern for cloudy
//solid green
void Pattern5()
{
	gpio_set_value(2,1);
	usleep(10000000);
        clearLights();
}

//pattern for fog and haze
//dim yellow and fast fade red
void Pattern6()
{
	Pwm("3","50","40");
	for(int x= 0; x<5; x++)
	{
		fastFade("11");
	}
	clearLights();
}

//pattern for thunderstorms
//fast flash blue red and yellow
void Pattern7()
{
	for(int x= 0; x<10; x++)
	{
		fastBlink(11);
		fastBlink(3);
		fastBlink(1);
	}
	clearLights();
}

// ------------------------------------------------------ 7-Segment Display ------------------------------------------ //
void initPins()
{

	gpio_request(5, NULL);

	gpio_request(1, NULL);

	gpio_request(2, NULL);

	gpio_request(46, NULL);

	gpio_request(19, NULL);

	gpio_request(11, NULL);

	gpio_request(4, NULL);

	gpio_request(6, NULL);

	gpio_request(45, NULL);

	gpio_request(3, NULL);

	gpio_request(18, NULL);

	gpio_direction_output(19,1);

	gpio_direction_output(11,1);

	gpio_direction_output(4,1);

	gpio_direction_output(6,1);

	gpio_direction_output(45,1);

	gpio_direction_output(3,1);

	gpio_direction_output(18,1);

	gpio_direction_output(5,0);


	gpio_direction_output(1,0);
	gpio_direction_output(2,0);
	gpio_direction_output(46,0);


	gpio_request(speakerPin, NULL);
	gpio_direction_output(speakerPin,1);
	gpio_set_value(speakerPin,1);

}

void clearDisplay()
{
	gpio_set_value(19,1);
	gpio_set_value(11,1);
	gpio_set_value(4,1);
	gpio_set_value(6,1);
	gpio_set_value(45,1);
	gpio_set_value(3,1);
	gpio_set_value(18,1);
}

void setOne()
{
	gpio_set_value(19,1);
	gpio_set_value(11,0);
	gpio_set_value(4,0);
	gpio_set_value(6,1);
	gpio_set_value(45,1);
	gpio_set_value(3,1);
	gpio_set_value(18,1);
}

void setTwo()
{
	gpio_set_value(19,0);
	gpio_set_value(11,0);
	gpio_set_value(4,1);
	gpio_set_value(6,0);
	gpio_set_value(45,0);
	gpio_set_value(3,1);
	gpio_set_value(18,0);
}

void setThree()
{
	gpio_set_value(19,0);
	gpio_set_value(11,0);
	gpio_set_value(4,0);
	gpio_set_value(6,0);
	gpio_set_value(45,1);
	gpio_set_value(3,1);
	gpio_set_value(18,0);
}

void setFour()
{
	gpio_set_value(19,1);
	gpio_set_value(11,0);
	gpio_set_value(4,0);
	gpio_set_value(6,1);
	gpio_set_value(45,1);
	gpio_set_value(3,0);
	gpio_set_value(18,0);
}

void setFive()
{
	gpio_set_value(19,0);
	gpio_set_value(11,1);
	gpio_set_value(4,0);
	gpio_set_value(6,0);
	gpio_set_value(45,1);
	gpio_set_value(3,0);
	gpio_set_value(18,0);
}

void setSix()
{
	gpio_set_value(19,0);
	gpio_set_value(11,1);
	gpio_set_value(4,0);
	gpio_set_value(6,0);
	gpio_set_value(45,0);
	gpio_set_value(3,0);
	gpio_set_value(18,0);
}

void setSeven()
{
	gpio_set_value(19,0);
	gpio_set_value(11,0);
	gpio_set_value(4,0);
	gpio_set_value(6,1);
	gpio_set_value(45,1);
	gpio_set_value(3,1);
	gpio_set_value(18,1);
}

void setEight()
{

	gpio_set_value(19,0);
	gpio_set_value(11,0);
	gpio_set_value(4,0);
	gpio_set_value(6,0);
	gpio_set_value(45,0);
	gpio_set_value(3,0);
	gpio_set_value(18,0);

}

void setNine()
{

	gpio_set_value(19,0);
	gpio_set_value(11,0);
	gpio_set_value(4,0);
	gpio_set_value(6,1);
	gpio_set_value(45,1);
	gpio_set_value(3,0);
	gpio_set_value(18,0);

}

void setZero()
{

	gpio_set_value(19,0);
	gpio_set_value(11,0);
	cout<<"Zero set"<<endl;
	gpio_set_value(4,0);
	gpio_set_value(6,0);
	gpio_set_value(45,0);
	gpio_set_value(3,0);
	gpio_set_value(18,1);

}

void setTime(char tim[])
{
	cout << "SETTING TIME" << endl;
	for(int x = 0; x<4; x++)
	{

		if (x == 0)
		{
			gpio_set_value(5, 1);
			gpio_set_value(1, 0);
			gpio_set_value(2, 0);
			gpio_set_value(46, 0);
		}
		if (x == 1)
		{
			gpio_set_value(5, 0);
			gpio_set_value(1, 1);
			gpio_set_value(2, 0);
			gpio_set_value(46, 0);
		}
		if (x == 2)
		{
			gpio_set_value(5, 0);
			gpio_set_value(1, 0);
			gpio_set_value(2, 1);
			gpio_set_value(46, 0);

		}
		if (x == 3)
		{
			gpio_set_value(5, 0);
			gpio_set_value(1, 0);
			gpio_set_value(2, 0);
			gpio_set_value(46, 1);
		}

		switch(tim[x])
		{
		case '0':

			setZero();
			break;

		case '1':

			setOne();
			break;

		case '2':

			setTwo();
			break;

		case '3':

			setThree();
			break;

		case '4':

			setFour();
			break;

		case '5':

			setFive();
			break;

		case '6':

			setSix();
			break;

		case '7':

			setSeven();
			break;
		case '8':

			setEight();
			break;
		case '9':

			setNine();
			break;
		}
		clearDisplay();
	}
}


struct Alarm {
	time_t alarmTime;
	bool triggered = false;
};

void writeLogFile(string error, string methodName){
	ofstream log("logFile.log", ios_base::app | ios_base::out);
	if(!log.is_open()) {
		cerr << "UNABLE TO OPEN LOG FILE" << endl;
	}
	else{
		time_t timer = time(0);
		string timestamp = asctime(localtime(&timer));
		log << "Error occured at time" << timestamp << ": " << error;
		log << "in method " << methodName << "()." << endl;
		log.close();
	}
}

void playAlarmSound(){//Speaker makes Sound
	gpio_set_value(speakerPin,1);
	usleep(1000000);
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	gpio_set_value(speakerPin,0);
}

void displayTime(string s){
	try{
		char tim[4];
		tim[0] = s[11];//hour_1
		tim[1] = s[12];//hour_2
		tim[2] = s[13];//minute_1
		tim[3] = s[14];//minute_2
		for(int i = 0; i < 20; i++) {
			setTime(tim);
		}
	}
	catch(const std::exception& e) {//Log file error
		writeLogFile(e.what(),"displayTime");
		cerr << e.what() << endl;
	}
}

void updateTime(int timeZoneDifference){
	try{
		time_t timer = time(0) - timeZoneDifference;
		string s = asctime(localtime(&timer));
		//	cout << s[11] << s[12] << s[13] << s[14] << s[15] << s[16] << s[17] << s[18] << endl;
		//std::this_thread::sleep_for(std::chrono::milliseconds(200));
		displayTime(s);
	}
	catch(const std::exception& e) {//Log File Error
		writeLogFile(e.what(),"updateTime");
		cerr << e.what() << endl;
	}
}

void checkAlarm(Alarm alarm, int timeZoneDifference){
	try{
		string alarmTime = asctime(localtime(&alarm.alarmTime));
		time_t currTime = time(0) - timeZoneDifference;
		string currentTime = asctime(localtime(&currTime));
		cout << alarmTime << endl;
		cout << currentTime << endl;
		if((alarmTime[11] == currentTime[11]) && (alarmTime[12] == currentTime[12]) && (alarmTime[14] == currentTime[14])
		   && (alarmTime[15] == currentTime[15])) {
			playAlarmSound();
			cout << "ALARM TRIGGERED" << endl;
			alarm.triggered = true;
		}
	}
	catch(const std::exception& e) {//Log file error
		writeLogFile(e.what(), "checkAlarm");
		cerr << e.what() << endl;
	}

}

string parseline(char line[], const int maxLineLength) {
	try{
		char input;
		string weather;
		bool isColon = false;
		int quoteCounter = 0;

		for (int i = 0; i < maxLineLength; ++i) {
			input = line[i];
			if (!isColon) {

				if (input == ' ' || input == '\t') continue;

				if (input == ':') {
					isColon = true;
					continue;
				}

			} else {

				if (input == '"') {
					quoteCounter += 1;
					if (quoteCounter == 2) {
						break;
					}
					continue;
				}

				if (input >= 'A' && input <= 'z') {
					weather += input;
				}
			}
		}
		return weather;
	}
	catch(const std::exception& e) {
		writeLogFile(e.what(), "parseLine");
		cerr << e.what() << endl;
	}

}

string parseTime(char line[], const int maxLineLength) {
	try{
		string time;
		string fullTime;

		for (int i = 0; i < maxLineLength; ++i) {
			fullTime += line[i];
		}

		time = fullTime.substr(40, 8);
		return time;
	}
	catch(const std::exception& e) {
		writeLogFile(e.what(),"parseTime");
		cerr << e.what() << endl;
	}

}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	try{
		((string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}
	catch(const std::exception& e) {
		writeLogFile(e.what(), "WriteCallBack");
		cerr << e.what() << endl;
	}

}
void returnTimeWeatherCity(string userInput, string& weatherInCity, string& timeInCity){

	try{
		string city = "http://api.wunderground.com/api/1bc93deaf6442978/conditions/q/CA/" + userInput + ".json";
		// ----------------------------- GET JSON OBJECT FROM HTTP GET ----------------------- //
		CURL *curl;
		CURLcode res;
		string readBuffer;

		curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, city.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
		}

		readBuffer.erase(0, 5);
		// Store JSON string in File
		ofstream os("json.txt");
		if (!os) { cerr << "Error writing to json.txt" << endl; }
		else {
			os << readBuffer << endl;
		}

		// ------------------------- PARSE LINE ----------------------- //

		ifstream infile;
		infile.open("json.txt");
		if (!infile.is_open()) {
			writeLogFile("ERROR OPENING JSON FILE", "returnTimeCity.log");
		}

		const int maxLineLength = 200;
		char line[maxLineLength];
		bool done = false;
		int fileLineNumber = 0;
		string weather;
		string time;

		while (!done) {
			++fileLineNumber;
			if (!infile.getline(line, maxLineLength)) {
				if (infile.peek()==std::ifstream::traits_type::eof()) {
					done = true;
				} else
					writeLogFile("ERROR GETTING LINE OF JSON FILE", "returnTimeCity.log");
			}

			if (fileLineNumber == 44) {
				time = parseTime(line, maxLineLength);
			}

			if (fileLineNumber == 85) {
				weather = parseline(line, maxLineLength);
			}
		}

		weatherInCity = weather;
		timeInCity = time;
	}
	catch(const std::exception& e) {
		writeLogFile(e.what(), "returnTimeWeatherCity");
		cerr << e.what() << endl;
	}
}

void outputWeather(string weather) {
	if (weather == "rain" || weather == "sleet") {
		Pattern1();
	}
	if (weather == "flurries" || weather == "snow") {
		Pattern2();
	}
	if (weather == "clear" || weather == "sunny") {
		Pattern3();
	}
	if (weather == "mostlycloudy" || weather == "partlycloudy" || weather == "mostlysunny" || "partlycloudy") {
		Pattern4();
	}
	if (weather == "cloudy") {
		Pattern5();
	}
	if (weather == "fog" || weather == "hazy") {
		Pattern6();
	}
	if (weather == "thunderstorms") {
		Pattern7();
	}
}

int main(const int argc, const char* argv[]) {
	initPins();
	initPinsLED();
	bool stop = false;
	Alarm alarm;
	alarm.alarmTime = 0;

	time_t currentTime = time(0);
	struct tm* sTime = localtime(&currentTime);


	string cityWeather = "";
	string cityTime = "";
	int timeZoneDifference = 0;

	if(argc == 1) {
		//no alarm
		cout << " NO ALARM SET" << endl;
		cout << " NO CITY SET" << endl;
	}
	else if(argc == 2) {// ./Time city, no alarm
		cout << " NO ALARM SET" << endl;
		string userInput = argv[1];
		returnTimeWeatherCity(userInput,cityWeather,cityTime);
		struct tm cTime;
		cTime.tm_hour = stoi(cityTime.substr(0,2));
		cTime.tm_min = stoi(cityTime.substr(3,2));
		cTime.tm_sec = stoi(cityTime.substr(6,2));

		timeZoneDifference = (sTime->tm_hour - cTime.tm_hour)*3600;

	}
	else if(argc == 3) {//./Time city HH:mm:DD:MM:YYYY, sets alarm for a date where HH:mm is in 24 hour time
		// ./Time city timeUntilAlarm
		string userInput = argv[1];
		string alarmDate = argv[2];
		returnTimeWeatherCity(userInput,cityWeather,cityTime);
		struct tm cTime;
		cTime.tm_hour = stoi(cityTime.substr(0,2));
		cTime.tm_min = stoi(cityTime.substr(3,2));
		cTime.tm_sec = stoi(cityTime.substr(6,2));
		timeZoneDifference = (sTime->tm_hour - cTime.tm_hour)*3600;


		struct tm aTime;
		aTime.tm_year = stoi(alarmDate.substr(12,4)) - 1900;
		aTime.tm_mon = stoi(alarmDate.substr(9,2)) - 1;
		aTime.tm_mday = stoi(alarmDate.substr(6,2));
		aTime.tm_hour = stoi(alarmDate.substr(0,2));
		aTime.tm_min = stoi(alarmDate.substr(3,4));

		alarm.alarmTime = mktime(&aTime);

	}
	outputWeather(cityWeather);

	do {
		updateTime(timeZoneDifference);
		if(alarm.alarmTime != 0 && !alarm.triggered) {
			checkAlarm(alarm, timeZoneDifference);
		}
	} while(!stop);
	cout << "FINISHED" << endl;
	return 0;
}
