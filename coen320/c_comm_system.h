/*
* Description : basic communication system class definition.
* Date : 15/11/2019
*
*/

#ifndef C_COMM_SYSTEM_H
#define C_COMM_SYSTEM_H

#include <sys/neutrino.h>

#include "s_tracker.h"

//printf
#include <stdio.h>

class c_comm_system {

public :

	c_comm_system(s_tracker* trackfile_);
	~c_comm_system();

	//used as threads...
	void run_comm_in_system();
	void run_comm_out_system();

private:

	//a pointer to a trackfile... when the atc system is initialized, its sends the pointer here... he saves a copy
	s_tracker* trackfile;

	//the channel of communication for IPC
	//we dont need it for now... maybe we would do it if we really need?
	int chid;
};

#endif //C_COMM_SYSTEM_H
