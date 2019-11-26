#include "c_airplane.h"

c_airplane::c_airplane(float x_, float y_, float z_) {
	id = 0;
	speed_x = 0;
	speed_y = 0;
	speed_z = 0;
	x = x_;
	y = y_;
	z = z_;
	entry_time = 0;
	plane_clock = 0;
	//current target is straight line....
	current_target_x = x + 100000;
	current_target_y = y + 100000;
	current_target_z = z;
	holding_status = false;
	change_altitude = false;
	old_target_x = current_target_x;
	old_target_y = current_target_y;
	old_target_z = current_target_z;
}

c_airplane::~c_airplane() {}

void c_airplane::update(double timeframe) {

	plane_clock += timeframe;

	x += timeframe * speed_x;
	y += timeframe * speed_y;
	z += timeframe * speed_z;


	if (holding_status == true)
	{
		float dist_x = current_target_x - x;
		float dist_y = current_target_y - y;
		float dist_z = current_target_z - z;
		float dist = sqrt(pow(dist_x,2)+pow(dist_y,2)+pow(dist_z,2));

		//target distance
		if (dist < target_distance)
		{
			//calculate new target
			//calculate unit vector and rotate vector 90 degree clockwise:(x,y) -> (y,-x), no change in z
			float unit_new_x = dist_y/dist;
			float unit_new_y = -dist_x/dist;
			//float unit_new_z = dist_z/dist;
			//asume distance to new target is 20k, no change in z
			float target_x = unit_new_x * 20000;
			float target_y = unit_new_y * 20000;
			//float target_z = unit_new_z;
			set_current_target(target_x,target_y,z);
		}
	}
	else //unset on hold mode, pass old target location to set_current_target function
	{
		float reset_target_x = get_old_target_x();
		float reset_target_y = get_old_target_y();
		float reset_target_z = get_old_target_z();
		set_current_target(reset_target_x,reset_target_y,reset_target_z);
	}


	if (change_altitude == true)
	{
		float dist_x = current_target_x - x;
		float dist_y = current_target_y - y;
		float dist_z = current_target_z - z;
		float dist = sqrt(pow(dist_x,2)+pow(dist_y,2)+pow(dist_z,2));

		//float unit_new_x = dist_x/dist;
		//float unit_new_y = dist_y/dist;
		float unit_new_z = dist_z/dist;

		//assume new altitude is current altitude *5000
		//no change in x and y
		float target_z = unit_new_z * 5000;
		set_current_target(current_target_x,current_target_y,target_z);
		// change speed_z to 0 if the plane reach the target altitude
		if (z>= target_z)
		{
			speed_z=0;
			change_altitude = false;
		}

	}

	return;
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

void c_airplane::set_on_hold(){
	holding_status = true;
	cout << "plane "<< get_id()<<" enter holding pattern..."<<endl;
}


void c_airplane::unset_on_hold(){
	holding_status = false;
	cout << "plane "<< get_id()<<" leave holding pattern..."<<endl;
}

void c_airplane::enable_altitude_change(){
	change_altitude = true;
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
float c_airplane::get_old_target_x(){
	return old_target_x;
}
float c_airplane::get_old_target_y(){
	return old_target_y;
}
float c_airplane::get_old_target_z(){
	return old_target_z;
}
