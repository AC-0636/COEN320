/*
* Description : radar subsystem. The update method will be instantiated as a THREAD. using the ENVIRONMENT, it scans for airplanes and sends a hit list.
* Date : 12/11/2019
*
*/

#ifndef C_RADAR_H
#define C_RADAR_H

//printf
#include <stdio.h>

#include "s_tracker.h"

#include <mutex>

#include <vector>

#include "c_environment.h"

class c_radar {

public:
	c_radar(c_environment* env_, s_tracker* trackfile_);
	~c_radar();

	void scan();

	//std::vector<c_airplane*> get_last_hit_list();
	inline void terminate() {
		term = true;
	}

	inline s_tracker* get_trackfile() {
		return trackfile;
	}

private:
	//pointer to the environment variable
	c_environment* env;

	//a pointer to a trackfile... when the atc system is initialized, its sends the pointer here... he saves a copy
		s_tracker* trackfile;

	//std::vector<c_airplane*> hit_list;
	bool term;
};

#endif //C_RADAR_H
