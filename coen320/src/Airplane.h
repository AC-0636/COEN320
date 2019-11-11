#include <time.h>
#include <iostream>
#include <Windows.h>
using namespace std;
class Airplane {

public:
	int ID;
	int speed_X;
	int speed_Y;
	int speed_Z;
	int X;
	int Y;
	int Z;
	int Entry_timE;

	Airplane(int id, int speed_x, int speed_y, int speed_z, int x, int y, int z, int entry_time)
	{
		ID = id;
		speed_X = speed_x;
		speed_Y = speed_y;
		speed_Z = speed_z;
		X = x;
		Y = y;
		Z = z;
		Entry_timE = entry_time;
		//assume test for position for 5s in the future
		Next_X = 5 * (speed_x * x);
		Next_Y = 5 * (speed_y * y);
		Next_Z = 5 * (speed_z * z);
		DisplacemenT = sqrt(pow((Next_X - X), 2) + pow((Next_Y - Y), 2) + pow((Next_Z - Z), 2));

	}

	int getData(char item)
	{
		if (item == 'X')
		{
			return Next_X;
		}
		else if (item == 'Y')
		{
			return Next_Y;
		}
		else if (item == 'Z')
		{
			return Next_Z;
		}
		else if (item == 'D')
		{
			return DisplacemenT;
		}
	}
	void getPosition() {
		/*This method updates the position of the plane in the space every time unit.
		*/
		int entryTime = Entry_timE;
		int initialX = X;
		int initialY = Y;
		int initialZ = Z;
		for (int t_clock = 0; t_clock <= 20; t_clock++) {
			if (entryTime <= t_clock) {
				//if (0 <= X <= 528000 && 0 <= Y <= 528000 && 15000 <= Z <= 40000) {
				X = speed_X * (t_clock - entryTime) + initialX;
				Y = speed_Y * (t_clock - entryTime) + initialY;
				Z = initialZ;// when the plane enters the space, it maintains its altitude (page 4 of project.pdf)
				//check if the plans are withing space limit
				if (((0 <= X) && (X <= 20)) && ((0 <= Y) && (Y <= 20)) && ((0 <= Z) && (Z <= 20))) {
				cout << endl;
				cout << "Time (" << t_clock << "): The plan " << ID << " is at (" << X << " , " << Y << " , " << Z<<")";
				Sleep(500);
				}
				}
			else {
				Sleep(500);
				cout << "."; 
				}
			
			
		}//end clock

	}//end getPosition
	void collisionDetection(Airplane b) {
		/*This method checks in advance if the minimum distance is respected; otherwise, it will
		display a WARNING with the planes concerned, time, and space details
		*/
		int entryT = 0;
		int entryTimeA = Entry_timE;
		int planeID = ID;
		int initialXA = X;
		int initialYA = Y;
		int initialZA = Z;
		int entryTimeB = b.Entry_timE;
		int initialXB = b.X;
		int initialYB = b.Y;
		int initialZB = b.Z;
		int minHorizontalSeparation = 4;
		int minVerticalSeparation = 3;
		float distanceH = 20.0;
		float distanceV = 10.0;
		
		if (entryTimeA > entryTimeB)
			entryT = entryTimeA;
		else entryT = entryTimeB;
		Z = initialZA; // when the plane enters the space, it maintains its altitude (page 4 of project.pdf)
		b.Z = initialZB; 
		distanceV = abs(Z - b.Z);
			for (int i = entryT; i <= 20+entryT; i = i++){
				X = speed_X * (i - entryTimeA) + initialXA;
				Y = speed_Y * (i - entryTimeA) + initialYA;
				b.X = b.speed_X * (i - entryTimeB) + initialXB;
				b.Y = b.speed_Y * (i - entryTimeB) + initialYB;
				distanceH=horizontalSeperationDistance(X-b.X,Y-b.Y);
				if (distanceH <= minHorizontalSeparation && distanceV<=minVerticalSeparation) {
					cout << "WARNING: RISK OF COLLISION!!!\nTime: "<<i<<" between plan "<< planeID<<" at ("<<X<<" , "<<Y<< " , " << Z<<") and plan "<< b.ID<<" at ("<<b.X <<" , "<<b.Y<< " , " << b.Z<<"). \nHorizontal DISTANCE = "<<distanceH<< ". Vertical DISTANCE = "<<distanceV<<endl<<endl;
					
				}
			}//end for i
			
	}//end of collisionDetection()

private:
	int Next_X;
	int Next_Y;
	int Next_Z;
	int DisplacemenT;

	float horizontalSeperationDistance(int xCoordinate, int yCoordinate) {
		return  sqrt( pow(xCoordinate,2) +pow(yCoordinate,2));
	}
	
};

