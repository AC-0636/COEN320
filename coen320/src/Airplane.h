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
		//DisplacemenT = sqrt(pow((Next_X - X), 2) + pow((Next_Y - Y), 2) + pow((Next_Z - Z), 2));

	}


	void getPosition() {
		int entryTime = Entry_timE;
		int initialX = X;
		int initialY = Y;
		int initialZ = Z;
		for (int t_clock = 0; t_clock < 50; t_clock++) 
		{
			if (entryTime <= t_clock) 
			{
				X = speed_X * (t_clock - entryTime) + initialX;
				Y = speed_Y * (t_clock - entryTime) + initialY;
				Z = speed_Z * (t_clock - entryTime) + initialZ;

				if (((X>=0)&&(X<=528000)) && ((Y >= 0)&&(Y <= 528000)) && ((Z>=15000)&&(Z <= 40000)))
				{	cout << endl;
					cout << "The plan " << ID << " is actually at " << X << " , " << Y << " , " << Z<<endl;
				}
			}
			else { cout << "."; }
			Sleep(500);
		}
	}


	
};
