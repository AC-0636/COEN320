/*
* Description : basic communication system class definition.
* Date : 15/11/2019
*
*/

#ifndef C_COMM_SYSTEM_H
#define C_COMM_SYSTEM_H

#include <sys/neutrino.h>
#include <inttypes.h>
#include <sys/syspage.h>

#include "s_tracker.h"

//nap
#include <unix.h>

//cin, cout
#include <iostream>

//printf
#include <stdio.h>

//explode()
#include <string>
#include <vector>
#include <sstream>
#include <utility>

//explode method... similar to php explode()
std::vector<std::string> explode(std::string str, std::string sep);

class c_comm_system {

public :

	c_comm_system(int chid_, s_tracker* trackfile_);
	~c_comm_system();

	//used as threads...
	void run_comm_system();

	inline void set_trackfile(s_tracker* trackfile_) {
		trackfile = trackfile_;
	}

private:

    uint64_t cps, start_cycle, end_cycle, ncycles;
    //total time the atc is running... its his own personnal clock
    double comm_system_time;

	//a pointer to a trackfile... when the atc system is initialized, its sends the pointer here... he saves a copy
	s_tracker* trackfile;

	int chid;
	int coid;
};

#endif //C_COMM_SYSTEM_H
