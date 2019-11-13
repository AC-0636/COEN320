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

//printf
#include <stdio.h>

#include <vector>
#include <algorithm> //for sorting purpose

#include "c_radar.h"
#include "c_environment.h"
#include "c_airplane.h"

//main tracker structure
//access to tracker MUST remain between mutexes
struct tracker {
	std::vector<c_airplane*> tracked_airplanes;
};

class c_atc {

public:
	c_atc();
	~c_atc();

	//return TRUE if running. FALSE if stop running (end simulation)
	bool run();

	void draw_gui();

	void init_radar(c_environment* env_);

private:

	tracker* trackfile;

	bool running;

	c_radar* radar;

    uint64_t cps, start_cycle, end_cycle, ncycles;
    //total time the atc is running... its his own personnal clock
    double atc_system_time;
};

#endif //C_ATC_H
