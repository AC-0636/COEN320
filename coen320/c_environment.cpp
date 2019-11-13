#include "c_environment.h"

using namespace std;

c_environment::c_environment(TestCase test_) {

	//all in m
	airspace_sealvl = 15000 * 0.3048;
	airspace_width = 100000 * 1.60934;
	airspace_height = 25000 * 0.3048;
	airspace_long = 100000 * 1.60934;

	TestCase test = test_;
	int array_size = sizeof(test.airplane_schedule)/sizeof(int);

	for (int i = 0; i < array_size; i++) {
		c_airplane* airplane = new c_airplane();

		int id = test.airplane_schedule[i];
		float speed_x = test.airplane_schedule[i + 1];
		float speed_y = test.airplane_schedule[i + 2];
		float speed_z = test.airplane_schedule[i + 3];
		float x = test.airplane_schedule[i + 4];
		float y = test.airplane_schedule[i + 5];
		float z = test.airplane_schedule[i + 6];
		float entry_time = test.airplane_schedule[i + 7];

		airplane->set_id(id);

		airplane->set_speed_x(speed_x);
		airplane->set_speed_y(speed_y);
		airplane->set_speed_z(speed_z);
		airplane->set_x(x);
		airplane->set_y(y);
		airplane->set_z(z);
		airplane->set_entry_time(entry_time);
		i += 7;

		airplanes.push_back(airplane);

		printf("added plane id : %i to environment @ position (%.2f, %.2f, %.2f) with initial speed vector (%.2f, %.2f, %.2f).\n",
				id, x, y, z, speed_x, speed_y, speed_z);
	}
}

c_environment::~c_environment() {

	for (unsigned int i = 0; i < airplanes.size(); i++) {
		delete airplanes[i];
	}

	airplanes.clear();
}

void c_environment::update(double timeframe) {

	for (unsigned int i = 0; i < airplanes.size(); i++) {
		airplanes[i]->update(timeframe);
		//printf("id : %i x : %f y : %f z : %f\n", airplanes[i]->get_id(), airplanes[i]->get_x(), airplanes[i]->get_y(),airplanes[i]->get_z());
	}
}

std::vector<c_airplane*> c_environment::get_airplanes_env() {

	return airplanes;
}

int c_environment::get_airspace_sealvl() {

	return airspace_sealvl;
}

int c_environment::get_airspace_width() {

	return airspace_width;
}

int c_environment::get_airspace_height() {

	return airspace_height;
}

int c_environment::get_airspace_long() {

	return airspace_long;
}
