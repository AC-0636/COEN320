/*
* Description : everything that is external to the ATC system. It simulates the environment with simulate()
* Date : 12/11/2019
*
*/

#ifndef C_ENVIRONMENT_H
#define C_ENVIRONMENT_H

//printf
#include <stdio.h>

#include <vector>

#include "c_airplane.h"
#include "testcase.h"

class c_environment {

public:

	//use a testcase to initialize the environment.
	c_environment(TestCase test_);
	~c_environment();

	//instead of update must be environment thread...
	void update(double timeframe);

	std::vector<c_airplane*> get_airplanes_env();

	int get_airspace_sealvl();
	int get_airspace_width();
	int get_airspace_height();
	int get_airspace_long();

private:

	//all airplanes... that are IN or OUT of the airspace... it just simulates the environment. DO YOU GET IT?
	std::vector<c_airplane*> airplanes;

	int airspace_sealvl; //ft
	int airspace_width; //miles
	int airspace_height; //ft
	int airspace_long; //miles
};

#endif //C_ENVIRONMENT_H
