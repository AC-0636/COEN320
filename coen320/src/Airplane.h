
class Airplane{

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
		Next_X = 5*(speed_x*x);
		Next_Y = 5*(speed_y*y);
		Next_Z = 5*(speed_z*z);
		DisplacemenT = sqrt(pow((Next_X-X),2)+pow((Next_Y-Y),2)+pow((Next_Z-Z),2));

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
		else if (item == 'D' )
		{
			return DisplacemenT;
		}
	}

private:
	int Next_X;
	int Next_Y;
	int Next_Z;
	int DisplacemenT;


};
