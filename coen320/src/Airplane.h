#include <time.h>
#include <iostream>
#include <thread>
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
//		DisplacemenT = sqrt(pow((Next_X - X), 2) + pow((Next_Y - Y), 2) + pow((Next_Z - Z), 2));

	}

	void getPosition()
	{
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
				//if (((0 <= X) && (X <= 20)) && ((0 <= Y) && (Y <= 20)) && ((0 <= Z) && (Z <= 20)))
				if (((X>=0)&&(X<=528000)) && ((Y >= 0)&&(Y <= 528000)) && ((Z>=15000)&&(Z <= 40000)))
				{	cout << endl;
					cout << "The plane " << ID << " is actually at " << X << " , " << Y << " , " << Z<< " Time now: "<<(t_clock-entryTime)<<endl;
				}
			}
			else { cout << "."; }
			this_thread::sleep_for(std::chrono::milliseconds(200));
			cout << t_clock << endl;
		}
	}


private:
	int DisplacemenT;


};
