#include "c_comm_system.h"

c_comm_system::c_comm_system(s_tracker* trackfile_) {

	//saves a copy of the pointer... USE MUTEX because if not race condition, crash or failures
	trackfile = trackfile_;

	//create thread here. join in destructor.
	//one thread for comm_in system and one for comm_out

	//chid
	//WE DONT NEED INTER PROCESS COMMUNICATION
	chid = ChannelCreate(_NTO_CHF_FIXED_PRIORITY);
}

c_comm_system::~c_comm_system() {}

void c_comm_system::run_comm_in_system() {

}

void c_comm_system::run_comm_out_system() {

	printf("__  __");

}
