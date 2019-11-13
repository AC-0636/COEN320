#include "c_radar.h"

using namespace std;

c_radar::c_radar(c_environment* env_) {

	printf("\nInitializing radar subsystem from ATC...\n");

	env = env_;

	printf("Radar subsytem operationnal!\n");
}

c_radar::~c_radar() {}

void c_radar::scan() {

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
					hit_list.push_back(env_planes[i]);
					//printf(" plane id : %i\n",env_planes[i]->get_id());
				}
			}
		}
	}

	//printf("radar subsystem scanned airspace and hit %i planes.\n", hit_list.size());
	return;
}

std::vector<c_airplane*> c_radar::get_last_hit_list() {

	return hit_list;
}
