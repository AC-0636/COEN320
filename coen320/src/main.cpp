#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include "TestCase.h"
#include "Airplane.h"
#define MaxInput 50
using namespace std;


TestCase InputArray;
vector<Airplane> listOfAirplane;

void dummy2(Airplane plane) {
	plane.getPosition();
	/*if ((plane.X < 0) || (plane.X > 528000) || (plane.Y < 0) || (plane.Y > 528000) || (plane.Z < 15000) || (plane.Z > 40000))
	{
		cout << "Airplane " << plane.ID << " leave the airspace." << endl;
	}*/
}

void getInputFile()
{
	
	/*for (int i = 0; i < sizeof(InputArray.airplane_schedule); i ++)
	{
		cout << InputArray.airplane_schedule[i] << " " << endl;
	}*/

	for (int i = 0; i < sizeof(InputArray.airplane_schedule); i=i+8)
	{
		Airplane airplane((InputArray.airplane_schedule[i]),(InputArray.airplane_schedule[i+1]),(InputArray.airplane_schedule[i+2]),
			(InputArray.airplane_schedule[i+3]),(InputArray.airplane_schedule[i+4]),(InputArray.airplane_schedule[i+5]),(InputArray.airplane_schedule[i+6]),
			(InputArray.airplane_schedule[i+7]));
		listOfAirplane.push_back(airplane);
	}

	
}

int main()
{
	getInputFile();
	thread t1(dummy2,(listOfAirplane[0]));
	t1.join();
	//t2.join();

	system("pause");
	//return;
}


