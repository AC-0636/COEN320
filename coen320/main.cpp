/*
* Description : ATC simulator
* Date : 12/11/2019
*
*/

//TODO:
//encapsulate display manager in its own class and create its own timer
//create comm class that generates 2 threads... one for comm_in -> scanf and one for comm_out printf (PRINT A MENU!)
//encapsulate display manager, radar in one thread each
//in comm_in send commands to airplanes -> add airplanes METHOD (easy) Holding Pattern (HARD)
//create collision detection method in ATC system DONE - EXCEPT COMMUNICATE TO COMM_SYSTEM
//all these systems use the trackerfile. trackerfile can only be modified by one job at the time
//for each job, with its timer, if the time it took to do the job is longer than x milliseconds (trackerfile was used by other thread hence delaying the current job...) then send warning
//log data to a file (do that in the ATC main system)
//for the comm system : WE CREATE A CHANNEL In the COMM SYSTEM (ChannelCreate). IN EACH AIRPLANE - WE INSTANTIATE CLIENT with COnnectAttach(). The server is the COMM SYSTEM. he will see what to do with messages

//ALSO CONSIDER RUNNING THE CODE FROM THE SHELL COMMAND LINE...


//printf
#include <stdio.h>

//time, threads and what not
#include <sys/neutrino.h>
#include <inttypes.h>

//for qtime
#include <sys/syspage.h>

#include <vector>
using namespace std;

#include "testcase.h"

#include "c_airplane.h"
#include "c_environment.h"
#include "c_atc.h"

int main() {

	printf("Welcome to ATC simulator 8000 using QNX Proprietary RTOS\n");
	printf("HOST : QNX localhost 7.0.0 x64\n\n");
	printf("Initializing simulation environment...\n\n");

	//here start and end refers to a timeframe (difference in time since last calculations)
    uint64_t cps, start_cycle, end_cycle, ncycles;
    double seconds = 0;
    double total_simulation_time = 0;

    cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
    printf( "This system has %lld cycles/sec.\n\n",cps );

	/*
	 * ------------------------------
	 * 		SIMULATION PARAMETERS
	 * ------------------------------
	 */
    TestCase test;
    c_environment* env = new c_environment(test);

    //snapping delta time frame
	start_cycle = ClockCycles();
    end_cycle = ClockCycles( );
    ncycles = end_cycle - start_cycle;

    seconds = (double) ncycles / cps;

	c_atc* atc = new c_atc();
	atc->init_radar(env);

	//while the systems runs... proceed with simulation
	while (atc->run()) {

		//all operations between start-end cycle
		start_cycle = ClockCycles();

		//update environment
		env->update(seconds);

	    /* snap the time again */
	    end_cycle = ClockCycles( );
	    ncycles = end_cycle - start_cycle;

	    seconds = (double) ncycles / cps;
	    //printf("The cycles in seconds is %1.12f.\n",seconds);

	    //it is not the correct time calculation but whatever.//REMOVE THIS comment before publishing
	    total_simulation_time += seconds;
	    //printf("Total simulation time = %f\n", total_simulation_time);
	}

	//deallocate dynamic memory
	delete env;

	return 0;
}
