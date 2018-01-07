#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <ugpio/ugpio.h>
#include <sstream>

using namespace std;

//defins the pins that will be used for flashing the leds
#define RED 9
#define BLUE 1
#define YELLOW 3
#define GREEN 2



//initalizes the pins controlling the leds
void initPins()
{

  //sets all of the available pins on the omega to act as gpio pins
  system("omega2-ctrl gpiomux set i2c gpio");

  system("omega2-ctrl gpiomux set uart0 gpio");

  system("omega2-ctrl gpiomux set uart1 gpio");

  system("omega2-ctrl gpiomux set uart2 gpio");

  system("omega2-ctrl gpiomux set pwm0 gpio");

  system("omega2-ctrl gpiomux set pwm1 gpio");

  system("omega2-ctrl gpiomux set refclk gpio");

  system("omega2-ctrl gpiomux set spi_s gpio");

  system("omega2-ctrl gpiomux set spi_cs1 gpio");

  system("omega2-ctrl gpiomux set i2s gpio");

  system("omega2-ctrl gpiomux set ephy gpio");

  system("omega2-ctrl gpiomux set wled gpio");


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

//uses fast gpio to write a pwm signal to a pin
void Pwm (char pinNum[], char frequency[], char duty[])
{

	string spinNum = string(pinNum);
	string sfrequency = string(frequency);
	string sduty = string(duty);
	string command = "fast-gpio pwm "+spinNum+" "+sfrequency+" "+sduty;
	system(command.c_str());
}



//sets the values of the pins to HIGH. I.e lights up the respective leds
void setRed() {
  gpio_set_value(RED, 1);
}

void setBlue() {
  gpio_set_value(BLUE, 1);
}

void setGreen() {
  gpio_set_value(GREEN, 1);
}

void setYellow() {
  gpio_set_value(YELLOW, 1);
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
