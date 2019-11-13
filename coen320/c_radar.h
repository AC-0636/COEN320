/*
* Description : radar subsystem. The update method will be instantiated as a THREAD. using the ENVIRONMENT, it scans for airplanes and sends a hit list.
* Date : 12/11/2019
*
*/

#ifndef C_RADAR_H
#define C_RADAR_H

//printf
#include <stdio.h>

#include <vector>

#include "c_environment.h"

class c_radar {

public:
	c_radar(c_environment* env_);
	~c_radar();

	void scan();

	std::vector<c_airplane*> get_last_hit_list();

private:
	//pointer to the environment variable
	c_environment* env;

	std::vector<c_airplane*> hit_list;
};

#endif //C_RADAR_H
