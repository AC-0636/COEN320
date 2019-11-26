/*
* Description : basic airplane class definition
* Date : 11/11/2019
*
*/
#include <math.h>
#ifndef C_AIRPLANE_H
#define C_AIRPLANE_H

#include <iostream>
using namespace std;

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

	//set or unset the plane in hold pattern...
	//hold pattern is...
	void set_on_hold();
	void unset_on_hold();

	//enable altitude change
	void enable_altitude_change();

	//not allowed to set airplane position directly except on instantiation... when setting, the airplane must set its target!
	/*void set_x(float x_);
	void set_y(float y_);
	void set_z(float z_);*/

	//this variable was given as part of the project specifications but is useless.
	//is the plane in the TRACON? or NOT? who gives a fuck WHEN it enters? isn't the RADAR system supposed to detect it?
	void set_entry_time(float entry_time_);

	//set a target to an airplane and set its speed accordingly
	void set_current_target(float target_x_, float target_y_, float target_z_);




	//getters
	int get_id();
	float get_speed_x();
	float get_speed_y();
	float get_speed_z();
	float get_x();
	float get_y();
	float get_z();
	float get_entry_time();
	//get the "old original" target point
	//need this point after unset on holding pattern
	float get_old_target_x();
	float get_old_target_y();
	float get_old_target_z();



private:
	//internal plane time
	float plane_clock;
	int id;
	float speed_x, speed_y, speed_z;
	float x, y, z;
	float entry_time;
	bool holding_status;
	bool change_altitude;
	float current_target_x, current_target_y, current_target_z;
	float old_target_x, old_target_y,old_target_z;

	const float target_distance = 1000;


};

#endif //C_AIRPLANE_H
