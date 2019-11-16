/*
* Description : basic atc class definition. The display manager is included in the ATC 8000 pro system! WOW!
* Date : 12/11/2019
*
*/

#ifndef C_ATC_H
#define C_ATC_H

#include <sys/neutrino.h>
#include <inttypes.h>
#include <sys/syspage.h>

//for pow()
#include <math.h>
//calls for system... on neutrino the sh command is "clear"
#include <stdlib.h>
//printf
#include <stdio.h>

#include <vector>
#include <algorithm> //for sorting purpose

#include "s_tracker.h"

#include "c_radar.h"
#include "c_environment.h"
#include "c_airplane.h"

class c_atc {

public:
	c_atc();
	~c_atc();

	//return TRUE if running. FALSE if stop running (end simulation)
	bool run();

	void draw_gui();

	//initiate radar before using it else segfault... for now
	void init_radar(c_environment* env_);

private:
	//called in thread internally only...
	void run_collision_detection();

	//send the trackfile to the radar so he can modify it
	s_tracker* trackfile;

	bool running;

	c_radar* radar;

    uint64_t cps, start_cycle, end_cycle, ncycles;
    //total time the atc is running... its his own personnal clock
    double atc_system_time;
};

#endif //C_ATC_H
