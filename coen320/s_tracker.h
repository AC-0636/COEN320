/*
* Description : definition of a tracker... simple a structure to comprehend a collection of airplane objects... why the need to redefine it for a tracker system? dont need
* BEFORE was included in c_atc system but was moved in its own file because it is used by many processes such as comm_system, radar, display, collision detection..
* Date : 15/11/2019
*
*/

#ifndef S_TRACKER_H
#define S_TRACKER_H

#include "c_airplane.h"

#include <vector>

//main tracker structure
//access to tracker MUST remain between mutexes
struct s_tracker {
	std::vector<c_airplane*> tracked_airplanes;
};

#endif //S_TRACKER_H
