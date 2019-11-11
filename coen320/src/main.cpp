#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include "TestCase.h"
#include "Airplane.h"
#define MaxInput 50
using namespace std;

int InputSize;
TestCase InputArray;
vector<Airplane> listOfAirplane;

TestCase::TestCase()
{
	//get input size from array, but there is not right and will be modified in the later part
	InputSize = sizeof(InputArray.airplane_schedule);
}

void dummy2(Airplane plane) {
	plane.getPosition();
	/*if ((plane.X < 0) || (plane.X > 528000) || (plane.Y < 0) || (plane.Y > 528000) || (plane.Z < 15000) || (plane.Z > 40000))
	{
		cout << "Airplane " << plane.ID << " leave the airspace." << endl;
	}*/
}

void getInputFile()
{

	for (int i = 0; i < InputSize; i=i+8)
	{
		if ((InputArray.airplane_schedule[i] == 0) && (i != 0))
		{
			//update the actual array size
			InputSize = i-8;
			i = 9999999;
		}
		else
		{
			Airplane airplane((InputArray.airplane_schedule[i]), (InputArray.airplane_schedule[i + 1]), (InputArray.airplane_schedule[i + 2]),
				(InputArray.airplane_schedule[i + 3]), (InputArray.airplane_schedule[i + 4]), (InputArray.airplane_schedule[i + 5]), (InputArray.airplane_schedule[i + 6]),
				(InputArray.airplane_schedule[i + 7]));
			listOfAirplane.push_back(airplane);
		}
	}
	//cout << InputSize << endl;
}

int main()
{
	TestCase();
	getInputFile();
	thread t1(dummy2,(listOfAirplane[0]));
	//t1.join();
	//t2.join();

	system("pause");
	//return;
}
