#include "c_airplane.h"

using namespace std;

//must set initial speed when created so we can calculate its target...
c_airplane::c_airplane(float x_, float y_, float z_, float speed_x_, float speed_y_, float speed_z_) {
	id = 0;
	speed_x = speed_x_;
	speed_y = speed_y_;
	speed_z = speed_z_;
	x = x_;
	y = y_;
	z = z_;
	entry_time = 0;

	is_hold = false;

	//set current target... far away in straight line using direction
	const float target_distance = 200000; //200km away... or whatever...
	//so this is a unit vector of the speed multiplied by the distance... so the total length to reach the target is 200km away
	float speed_module = sqrt(pow(speed_x,2)+ pow(speed_y,2)+ pow(speed_z,2));
	current_target_x = (speed_x/speed_module) * target_distance;
	current_target_y = (speed_y/speed_module) * target_distance;
	current_target_z = (speed_z/speed_module) * target_distance;

	saved_target_x = 0;
	saved_target_y = 0;
	saved_target_z = 0;

	target_altitude = z;
	is_changing_altitude = false;

	entry = false;
}

c_airplane::~c_airplane() {}

void c_airplane::update(double timeframe) {

	x += timeframe * speed_x;
	y += timeframe * speed_y;

	//ajust altitude with target altitude...
	if (is_changing_altitude == true) {
		float alt_diff = target_altitude - z;
		if (abs((int)alt_diff) < 50) { //about 50m difference from target altitude
			is_changing_altitude = false;
			speed_z = 0;
		}
		z += timeframe * speed_z;
	}

	if (is_hold == true) {
		float dist_x = current_target_x - x;
		float dist_y = current_target_y - y;
		float dist_z = current_target_z - z;
		float dist = sqrt(pow(dist_x,2)+pow(dist_y,2)+pow(dist_z,2));

		//if the plane is near the target distance... calculate a new target by rotating it
		if (dist < MIN_TARGET_DISTANCE) {
			//calculate new target
			//calculate unit vector and rotate vector 90 degree clockwise:(x,y) -> (y,-x), no change in z
			float unit_new_x = dist_y/dist;
			float unit_new_y = -dist_x/dist;
			//float unit_new_z = dist_z/dist;
			//asume distance to new target is 2km, no change in z
			float target_x = unit_new_x * square_length + x;
			float target_y = unit_new_y * square_length + y;
			//float target_z = unit_new_z;

			

			set_current_target(target_x,target_y,z);
		}
	}

	return;
}

void c_airplane::set_target_altitude(float altitude, bool entry_) {
	if (entry_ == true) {
		if (entry == false) {
			entry = true;
		} else {
			return;
		}

	}
	if (is_changing_altitude == false) {
		is_changing_altitude = true;
		current_target_z = altitude;
		if (entry_ == false) {
			target_altitude = altitude;
			set_current_target(current_target_x, current_target_y, current_target_z);
			float alt_diff = target_altitude - z;
			if (alt_diff > 0) {
				speed_z = 200; //altitude change safely at this speed
			} else {
				speed_z = -200;
			}
			airplane_log.push_back("New target altitude set to " + to_string(trunc(altitude)) + "m");
		} else {
			//speed_z = 0;
		}
	}
}

void c_airplane::set_on_hold(){
	is_hold = true;
	//save the target and go on hold mode..
	saved_target_x = current_target_x;
	saved_target_y = current_target_y;
	saved_target_z = current_target_z;

	float dist_x = current_target_x - x;
	float dist_y = current_target_y - y;
	float dist_z = current_target_z - z;
	float dist = sqrt(pow(dist_x,2)+pow(dist_y,2)+pow(dist_z,2));

	//calculate new target
	//calculate unit vector and rotate vector 90 degree clockwise:(x,y) -> (y,-x), no change in z
	float unit_new_x = dist_y/dist;
	float unit_new_y = -dist_x/dist;
	float unit_new_z = dist_z/dist;
	//asume distance to new target is 2km, no change in z
	float target_x = unit_new_x * square_length + x;
	float target_y = unit_new_y * square_length + y;
	float target_z = unit_new_z;
	set_current_target(target_x,target_y,target_z);

	airplane_log.push_back("Airplane is set on hold");
}


void c_airplane::unset_on_hold(){
	is_hold = false;
	//return to last target
	set_current_target(saved_target_x, saved_target_y, saved_target_z);

	airplane_log.push_back("Plane is unholded");
}

void c_airplane::set_current_target(float target_x_, float target_y_, float target_z_){
	current_target_x = target_x_;
	current_target_y = target_y_;
	current_target_z = target_z_;

	//calculate speed change
    float dist_x = current_target_x - x;
	float dist_y = current_target_y - y;
	float dist_z = current_target_z - z;
	float dist = sqrt(pow(dist_x,2)+pow(dist_y,2)+pow(dist_z,2));

	float speed_module = sqrt(pow(speed_x,2)+ pow(speed_y,2)+ pow(speed_z,2));
	float unit_new_x = dist_x/dist;
	float unit_new_y = dist_y/dist;
	float unit_new_z = dist_z/dist;

	speed_x = unit_new_x * speed_module;
	speed_y = unit_new_y * speed_module;
	speed_z = unit_new_z * speed_module;
}

/*
 *
 * setters
 *
 */

void c_airplane::set_id(int id_) {

	id = id_;
}

void c_airplane::set_speed_x(float speed_x_) {

	speed_x = speed_x_;
}

void c_airplane::set_speed_y(float speed_y_) {

	speed_y = speed_y_;
}

void c_airplane::set_speed_z(float speed_z_) {

	speed_z = speed_z_;
}

/*void c_airplane::set_x(float x_) {

	x = x_;
}

void c_airplane::set_y(float y_) {

	y = y_;
}

void c_airplane::set_z(float z_) {

	z = z_;
}*/

void c_airplane::set_entry_time(float entry_time_) {

	entry_time = entry_time_;
}

/*
 *
 * getters
 *
 */

int c_airplane::get_id() {

	return id;
}

float c_airplane::get_speed_x() {

	return speed_x;
}

float c_airplane::get_speed_y() {

	return speed_y;
}

float c_airplane::get_speed_z() {

	return speed_z;
}

float c_airplane::get_x() {

	return x;
}

float c_airplane::get_y() {

	return y;
}

float c_airplane::get_z() {

	return z;
}

float c_airplane::get_entry_time() {

	return entry_time;
}
