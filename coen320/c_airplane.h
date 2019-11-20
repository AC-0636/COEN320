/*
* Description : basic airplane class definition
* Date : 11/11/2019
*
*/

#ifndef C_AIRPLANE_H
#define C_AIRPLANE_H

class c_airplane {

public :

	//main constructor and destructor
	c_airplane(float x_, float y_, float z_);
	~c_airplane();

	//update with timeframe difference between the last time it was updated
	void update(double timeframe);

	//setters
	void set_id(int id_);
	void set_speed_x(float speed_x_);
	void set_speed_y(float speed_y_);
	void set_speed_z(float speed_z_);
	//not allowed to set airplane position directly except on instantiation... when setting, the airplane must set its target!
	/*void set_x(float x_);
	void set_y(float y_);
	void set_z(float z_);*/

	//this variable was given as part of the project specifications but is useless.
	//is the plane in the TRACON? or NOT? who gives a fuck WHEN it enters? isn't the RADAR system supposed to detect it?
	void set_entry_time(float entry_time_);

	//getters
	int get_id();
	float get_speed_x();
	float get_speed_y();
	float get_speed_z();
	float get_x();
	float get_y();
	float get_z();
	float get_entry_time();

private:
	int id;
	float speed_x, speed_y, speed_z;
	float x, y, z;
	float entry_time;
};

#endif //C_AIRPLANE_H
