/*
* Description : definition of a tracker... simple a structure to comprehend a collection of airplane objects... why the need to redefine it for a tracker system? dont need
* BEFORE was included in c_atc system but was moved in its own file because it is used by many processes such as comm_system, radar, display, collision detection..
* Date : 15/11/2019
*
*/

#ifndef S_TRACKER_H
#define S_TRACKER_H

#define M_SIZE 40

#include "c_airplane.h"

#include <mutex>

#include <vector>

//if terminate is true... break from thread while loop
static bool terminate_system;
extern bool terminate_system;

static pthread_mutex_t printf_mutex = PTHREAD_MUTEX_INITIALIZER;
extern pthread_mutex_t printf_mutex;
//this mutex is defined globally here so we can use it print or use the trackfile
/*static std::mutex printf_mutex;
extern std::mutex printf_mutex;

static std::mutex tracker_mutex;
extern std::mutex tracker_mutex;*/
//use const std::lock_guard<std::mutex> lock(g_i_mutex);
//g_i_mutex is automatically released when lock
// goes out of scope

//main tracker structure
//access to tracker MUST remain between mutexes
struct s_tracker {
	std::vector<c_airplane*> tracked_airplanes;
};

#endif //S_TRACKER_H
