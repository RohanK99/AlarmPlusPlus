#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <ugpio/ugpio.h>

using namespace std;

     /*
      Written by Kash Pirani
          A
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

//setTime will very quickly display digits on the display in sequence
//setTime needs to be run multiple times a second to display an apparent static number. Ie. put in a for loop

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
