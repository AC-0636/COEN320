#include "c_atc.h"

using namespace std;

c_atc::c_atc() {
	printf("\nInitializing ATC system...\n");

	printf("\nInitializing ATC system time...\n");
	atc_system_time = 0;
	cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
	start_cycle = ClockCycles();
    end_cycle = ClockCycles( );
    ncycles = end_cycle - start_cycle;


	printf("\nInitializing Tracker...\n");
	trackfile = new s_tracker();

	radar = 0; //initialize with init_radar(env_);

	running = true;

	printf("ATC system UP and running!\n");
}

c_atc::~c_atc() {

	delete trackfile;
	delete radar;
}

bool c_atc::run() {

	//here just run its own timer... the modules are runned in their init... or maybe just call logging_system...

	start_cycle = ClockCycles();

	//implement this as a thread and run it from constructor join in destructor
	//to do in radar -> scale job time to 15 seconds... here since its a fake software defined radar - we can hit a scan millions per seconds
	radar->scan();
	trackfile->tracked_airplanes = radar->get_last_hit_list();

	//put in a thread and run it onces per 10 seconds
	if (atc_system_time >= 3) {
		draw_gui();
		atc_system_time = 0;
	}

	end_cycle = ClockCycles( );
    ncycles = end_cycle - start_cycle;

    double delta_Time = (double) ncycles / cps;
    atc_system_time += delta_Time;

	return running;
}

void c_atc::init_radar(c_environment* env_) {

	radar = new c_radar(env_);

	//create thread here and scan();
}

void c_atc::draw_gui() {
	//according to qnx... the system from stdlib
	//hihi hi
	//system("vi");
	printf("\n|------------------------------------------------------------|\n");
	//for every row we check if there is a plane on the x and draw them with printf
	for (int i = 20; i > 0; i--) { //10
		printf("|");
		vector<int> positions_of_planes_on_x_axis;
		for (unsigned int j = 0; j < trackfile->tracked_airplanes.size(); j++) {
			//since its all meters and we want to draw a dot for each plane at the (x,y) position, we divide all meters by 1000 to get km.
			int y = (int)((trackfile->tracked_airplanes[j]->get_y() / 1000) / (100/20)); //10
			//printf("%i\n",y);
			if (y < (i+1) && y > (i-1)) { //1
				int x = (int)trackfile->tracked_airplanes[j]->get_x() / 1000;
				positions_of_planes_on_x_axis.push_back(x);
			}
		}
		//draw them on the axis then close the line with '|'
		//but before... we sort them...
		sort(positions_of_planes_on_x_axis.begin(), positions_of_planes_on_x_axis.end());
		//i decided to scale this radar 10:3... i mean 10km each line and 3km each row
		for (int i = 0; i < 60; i++) {
			bool found = false;
			for (unsigned int j = 0; j < positions_of_planes_on_x_axis.size(); j++) {
				int x = (int)(positions_of_planes_on_x_axis[j] / (100/60));
				//printf("x : %i\n", x);
				if (x == i) {
					printf("*");
					found = true;
					break; //skip all other planes... we already marked the spot
				}
			}
			if (found == false) {
				printf(" "); //a space instead of a plane
			}
		}
		/*for (unsigned int j = 0; j < positions_of_planes_on_x_axis.size(); j++) {
			x = (int)(positions_of_planes_on_x_axis[j] / 10);
			//printf("x : %i\n", x);
			for (int k = 0; k < x; k++) {
				printf("-"); //print an empty space
			}
			printf("*"); //this is the airplane or airplaneS
		}
		while (x < 10) {
			printf("-"); //space after all the airplanes where drawn...
			x++;
		}*/
		printf("|\n");
	}
	printf("|------------------------------------------------------------|\n");
}

//15/11/2019
//very simple system... no need for sweep and pruning or complex collision detection...
//simply verify for all planes in the sector and check their distance difference...
//in a circle, of X km... send message to communication system c_communication_system
void c_atc::run_collision_detection() {

	//minimal distance trigger_detection... is a radius...
	float minimal_distance_trigger = 5; //km

	//dont forget to add mutex because it is gonna use the trackfile...
	int num_tracked_airplanes = trackfile->tracked_airplanes.size();
	for (int i = 0; i < num_tracked_airplanes; i++) {
		for (int j = 0; j < num_tracked_airplanes; j++) {

			//airplane i
			float xi = trackfile->tracked_airplanes[i]->get_x();
			float yi = trackfile->tracked_airplanes[i]->get_y();
			float zi = trackfile->tracked_airplanes[i]->get_z();

			//airplane j
			float xj = trackfile->tracked_airplanes[j]->get_x();
			float yj = trackfile->tracked_airplanes[j]->get_y();
			float zj = trackfile->tracked_airplanes[j]->get_z();

			float distance = pow(xi - xj, 2) + pow(yi - yj,2) + pow(zi - zj,2);
			if (distance <= minimal_distance_trigger) {
				//trigger collision system -> send message... the communication system will handle dispatching the commands to the fuckers
			}
		}
	}
}
