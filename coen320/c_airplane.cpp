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
}

c_airplane::~c_airplane() {}

void c_airplane::update(double timeframe) {

	x += timeframe * speed_x;
	y += timeframe * speed_y;
	z += timeframe * speed_z;

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
