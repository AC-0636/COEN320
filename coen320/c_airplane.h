/*
* Description : basic airplane class definition
* Date : 11/11/2019
*
*/

#ifndef C_AIRPLANE_H
#define C_AIRPLANE_H

#include <math.h>

#include <iostream>

#include <string>
#include <vector>

#define MIN_TARGET_DISTANCE 500

class c_airplane {

public :

	//main constructor and destructor
	c_airplane(float x_, float y_, float z_, float speed_x_, float speed_y_, float speed_z_);
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
	//is the plane in the TRACON? or NOT? isn't the RADAR system supposed to detect it?
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

	//set or unset the plane in hold pattern...
	//hold pattern is...
	void set_on_hold();
	void unset_on_hold();

	inline float get_is_hold() {
		return is_hold;
	}

	//the Message server send a message here... this is similar to a listening port on a computer...
	inline void get_message(std::string message) {
		airplane_log.push_back(std::string("Received message : ") + message.c_str());
	}

	inline std::vector<std::string> get_airplane_log() {
		return airplane_log;
	}

	//set a target to an airplane and set its speed accordingly
	void set_current_target(float target_x_, float target_y_, float target_z_);

	//set target altitude...
	void set_target_altitude(float altitude, bool entry_);

private:
	int id;
	float speed_x, speed_y, speed_z;
	float x, y, z;
	float entry_time;

	bool is_hold;
	float current_target_x, current_target_y, current_target_z;
	float saved_target_x, saved_target_y,saved_target_z;

	//the minimal distance of the plane from the target destination to trigger event (here it will be using to change trajectory when in hold mode)
	const float min_target_distance = 500; //500m
	//holding mode square length
	const float square_length = 2000; //2 km

	//this airplane log
	std::vector<std::string> airplane_log;

	//this is used if the airplane entered the airspace...similar to how cell towers reserve slots for inbound/outbound calls
	bool entry;
	float target_altitude;
	bool is_changing_altitude; //prevents the radar to set back the target altitude to 0 when input a hit
};

#endif //C_AIRPLANE_H
