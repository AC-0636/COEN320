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

//thread
#include <thread>

#include <pthread.h>

//cin, cout
#include <iostream>

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
#include "c_comm_system.h"
#include "c_environment.h"
#include "c_airplane.h"

class c_atc {

public:
	c_atc(c_environment* env_);
	~c_atc();

	//return TRUE if running. FALSE if stop running (end simulation)
	bool run();

	void draw_gui();

	//initiate radar before using it else segfault... for now
	//void init_radar(c_environment* env_);

	//called in thread internally only...
	void run_collision_detection();

	void run_message_server();


	static void* static_draw_gui(void* p) {
		static_cast<c_atc*>(p)->draw_gui();
		return NULL;
	}

	static void* static_run_collision_detection(void* p) {
		static_cast<c_atc*>(p)->run_collision_detection();
		return NULL;
	}

	static void* static_radar_scan(void* p) {
		static_cast<c_radar*>(p)->scan();
		return NULL;
	}

	static void* static_run_comm_system(void* p) {
		static_cast<c_comm_system*>(p)->run_comm_system();
		return NULL;
	}

	static void* static_run_message_server(void* p) {
		static_cast<c_atc*>(p)->run_message_server();
		return NULL;
	}

	inline void set_env(c_environment* env_) {
		env = env_;
	}

private:
	c_environment* env;

	//send the trackfile to the radar so he can modify it
	s_tracker* trackfile;

	bool running;

	c_radar* radar;
	c_comm_system* comm_system;

    uint64_t cps, start_cycle, end_cycle, ncycles;
    //total time the atc is running... its his own personnal clock
    double atc_system_time;

    uint64_t coll_start_cycle, coll_end_cycle, coll_ncycles;
    //total time the atc is running... its his own personnal clock
    double collision_detection_uptime;

	//the channel of communication for IPC
	//we dont need it for now... maybe we would do it if we really need?
	int chid;
	int coid_collision;

	//we dont need the display manager to be a thread because it is included in the ATC system (aka it's the main process)
	/*std::thread radar_thread;
	std::thread comm_thread;
	std::thread collision_detection_thread;*/
	//std::thread gui_thread;
	pthread_t message_server_thread;
	pthread_t gui_draw_thread;
	pthread_t comm_thread;
	pthread_t radar_thread;
	pthread_t collision_detection_thread;

	std::mutex mutex;

	bool to_draw_gui;

	std::vector<std::string> alert_log;
};

#endif //C_ATC_H
