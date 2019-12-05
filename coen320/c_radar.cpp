#include "c_radar.h"

using namespace std;

c_radar::c_radar(c_environment* env_,s_tracker* trackfile_) {

	trackfile = trackfile_;

	terminate_system = false;
	term = false;

	printf("\nInitializing radar subsystem from ATC...\n");

	//start with a first scan!!!
	radar_system_time = 3;
	total_radar_system_time = 0;
	cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
	start_cycle = ClockCycles();
    end_cycle = ClockCycles( );
    ncycles = end_cycle - start_cycle;

	env = env_;

	printf("Radar subsytem operationnal!\n");
}

c_radar::~c_radar() {}

void c_radar::scan() {

	while (1) {

		if (terminate_system == true) {
			//printf("SCAN TERMINATED!\n");
			return;
		}
		if (term == true) {
			return;
		}

		start_cycle = ClockCycles();

		std::vector<c_airplane*> hit_list;

		pthread_mutex_lock( &printf_mutex );
		//tracker_mutex.lock();

		if (radar_system_time >= 10) {
			radar_system_time = 0;
			//clear the vector (without deleting pointers!)
			hit_list.clear();

			vector<c_airplane*> env_planes = env->get_airplanes_env();

			int airspace_sealvl = env->get_airspace_sealvl();
			int airspace_width = env->get_airspace_width();
			int airspace_height = env->get_airspace_height();
			int airspace_long = env->get_airspace_long();

			for (unsigned int i = 0; i < env_planes.size(); i++) {

				float plane_x = env_planes[i]->get_x();
				float plane_y = env_planes[i]->get_y();
				float plane_z = env_planes[i]->get_z();

				//is it in altitude?
				if (plane_z < airspace_height+airspace_sealvl && plane_z > airspace_sealvl) {
					//is it in y? assuming airspace starts at 0
					if (plane_y < airspace_long && plane_y > 0) {
						//is it in x?
						if (plane_x < airspace_width && plane_x > 0) {
							//it is in hit!
							//SEND A MESSAGE TO TELL THE AIRPLANE TO BE IN CONSTANT ALTITUDE!
							env_planes[i]->set_target_altitude(env_planes[i]->get_z(), true);
							hit_list.push_back(env_planes[i]);
							//printf(" plane id : %i\n",env_planes[i]->get_id());
						}
					}
				}
			}

			string str = "[ -- ";
			str += to_string((int)(trunc(total_radar_system_time)));
			//we could output more data into the logger... but this is going to be available in the next version
			str += "radar@atc_system : updated trackfile with new radar input successfully! -- ]";
			//printf("%s\n",str.c_str());
			update_log.push_back(str.c_str());
			//printf("\n[ -- %.3f radar@atc_system : updated trackfile with new radar input successfully! -- ]\n", total_radar_system_time);
			printf("\n[ -- radar@atc_system : updated trackfile with radar output successfully! -- ]\n");
			trackfile->tracked_airplanes = hit_list;
		}

		last_hit_list = hit_list;

		pthread_mutex_unlock( &printf_mutex );
		//tracker_mutex.unlock();

		end_cycle = ClockCycles( );
	    ncycles = end_cycle - start_cycle;

	    double delta_Time = (double) ncycles / cps;
	    radar_system_time += delta_Time;
	    total_radar_system_time += delta_Time;
	}
	return;
}


