#include "c_atc.h"

using namespace std;

c_atc::c_atc(c_environment* env_) {

	env = env_;

	//const std::lock_guard<std::mutex> lock(printf_mutex);

	printf("Initializing ATC system...\n");

	//chid
	//WE DONT NEED INTER PROCESS COMMUNICATION
	chid = ChannelCreate(_NTO_CHF_FIXED_PRIORITY);
	coid_collision = ConnectAttach(0,0,chid,0,0);

	printf("Initializing ATC system time...\n");
	atc_system_time = 0;
	cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
	start_cycle = ClockCycles();
    end_cycle = ClockCycles( );
    ncycles = end_cycle - start_cycle;

    collision_detection_uptime = 0;
	coll_start_cycle = ClockCycles();
    coll_end_cycle = ClockCycles( );
    coll_ncycles = end_cycle - start_cycle;

    terminate_system = false;
    to_draw_gui = false;

    printf("Initializing Alert log...\n");
    //alert log vector

	printf("Initializing Tracker...\n");
	trackfile = new s_tracker();

	//radar = 0; //initialize with init_radar(env_);
	radar = new c_radar(env_, trackfile);

	//create thread here and scan();
	//radar_thread = thread(&c_radar::scan, radar);

	//initialize comm_system
	printf("Initializing communication system...\n");

	running = true;

	printf("ATC system UP and running!\n");

	comm_system = new c_comm_system(chid, trackfile);

	/*comm_thread = thread(&c_comm_system::run_comm_system, comm_system);
	collision_detection_thread = thread(&c_atc::run_collision_detection, this);*/

	//gui_thread = thread(&c_atc::draw_gui, this);
	pthread_create(&message_server_thread, NULL, &c_atc::static_run_message_server, this);
	pthread_create(&gui_draw_thread, NULL, &c_atc::static_draw_gui, this);
	pthread_create(&collision_detection_thread, NULL, &c_atc::static_run_collision_detection, this);
	pthread_create(&radar_thread, NULL, &c_atc::static_radar_scan, radar);
	pthread_create(&comm_thread, NULL, &c_atc::static_run_comm_system, comm_system);

}

c_atc::~c_atc() {

	printf("\nATC System shutting down safely...\n");

	pthread_join(message_server_thread,NULL);
	printf("Message server thread is terminated...\n");

	pthread_join(gui_draw_thread, NULL);
	printf("Display manager thread is terminated...\n");

	pthread_join(collision_detection_thread, NULL);
	//collision_detection_thread.join();
	printf("Collision detection thread is terminated...\n");

	pthread_join(comm_thread, NULL);
	//comm_thread.join();
	printf("Communication subsystem thread is terminated...\n");

	pthread_join(radar_thread, NULL);
	//radar_thread.join();
	printf("Radar scanner shut-off safely...\n\n");

	delete trackfile;
	delete radar;
	delete comm_system;
}

void c_atc::run_message_server() {
	while (1) {

		if (terminate_system == true) {
			return;
		}

		pthread_mutex_lock(&printf_mutex );

		//char 0 is argument 1, char 1 is second argument, or 2 char is plane altitude for example etc...
		//for plane change position [
		char Server_Messages[M_SIZE];

		int rcvid = MsgReceive(chid,Server_Messages,sizeof(Server_Messages),NULL);
		//printf(Server_Messages);

		char message[M_SIZE] = "\n40 OK\n";
		MsgReply(rcvid,EOK, message, sizeof (message));

		//we process the message... BECAUSE THE ATC IS ACTING AS THE SERVER
		if (strcmp(Server_Messages, "show alerts") == 0) {
			printf("Alert log : (%i)\n", alert_log.size());
			for (unsigned int i = 0; i < alert_log.size(); i++) {
				printf("%i. %s\n", i+1, alert_log[i].c_str());
			}
		}

		if (strcmp(Server_Messages, "show airplanes") == 0) {
			//fetch the latest update from the radar... (every 5 seconds the radar updates it)
			trackfile = radar->get_trackfile();
			comm_system->set_trackfile(trackfile);

			vector<c_airplane*> tracked_airplanes = trackfile->tracked_airplanes;
			printf("\ntotal airplanes : %i", tracked_airplanes.size());
			printf("\n%-10s%-10s%-10s%-10s%-10s%-10s%-10s\n", "id", "x", "y", "z","speed_x","speed_y","speed_z");
			for (unsigned int i = 0; i < tracked_airplanes.size(); i++) {
				int id = tracked_airplanes[i]->get_id();
				float x = tracked_airplanes[i]->get_x();
				float y = tracked_airplanes[i]->get_y();
				float z = tracked_airplanes[i]->get_z();
				float speed_x = tracked_airplanes[i]->get_speed_x();
				float speed_y = tracked_airplanes[i]->get_speed_y();
				float speed_z = tracked_airplanes[i]->get_speed_z();
				printf("%-10i%-10.0f%-10.0f%-10.0f%-10.0f%-10.0f%-10.0f\n", id, x,y,z,speed_x,speed_y,speed_z);
			}
		}

		if (strcmp(Server_Messages, "show radar") == 0) {
			to_draw_gui = true;
		}

		if (strcmp(Server_Messages, "COLLISION") == 0) {
			//printf("COLLISION IMMINENT!!!\n");
		}

		if (strcmp(Server_Messages, "exit") == 0) {
			terminate_system = true;
			radar->terminate();
			running = false;
		}

		pthread_mutex_unlock(&printf_mutex );
	}
	return;
}

bool c_atc::run() {

	return running;
}

void c_atc::draw_gui() {
	//according to qnx... the system from stdlib
	//hihi hi
	//system("vi");
	while (1) {

		//nap(100);

		if (terminate_system == true) {
			return;
		}

		start_cycle = ClockCycles();
		/*if (atc_system_time >= 5) {
			atc_system_time = 0;
		} else {
			//continue;
		}*/

		pthread_mutex_lock(&printf_mutex );
		//tracker_mutex.lock();

		if (to_draw_gui == true) {
			to_draw_gui = false;

			printf("\n        0km                                                          100km");
			printf("\nx     - ");
			printf("|------------------------------------------------------------|\n");
			//for every row we check if there is a plane on the x and draw them with printf
			for (int i = 20; i >= 0; i--) { //10
				//printf("      ");
				if (i == 20) {
					printf("100km - ");
				} else if (i == 1) {
					printf("5km   - ");
				} else if (i == 0) {
					printf("0km   - ");
				} else {
					printf("%ikm  - ",i*5);
				}
				printf("|");
				vector<int> positions_of_planes_on_x_axis;
				for (unsigned int j = 0; j < trackfile->tracked_airplanes.size(); j++) {
					//since its all meters and we want to draw a dot for each plane at the (x,y) position, we divide all meters by 1000 to get km.
					int y = (int)((trackfile->tracked_airplanes[j]->get_y() / 1000) / (100/20)); //10
					//printf("%i\n",y);
					if (y < (i+1) && y > (i-1)) { //1
						int x = (int)trackfile->tracked_airplanes[j]->get_x();
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
						int x = (int)((positions_of_planes_on_x_axis[j] / 1000) * 0.6);
						//printf("x : %i\n", x);
						if (x == i) {
							//printf("X : %i ",x);
							printf("*");
							found = true;
							break; //skip all other planes... we already marked the spot
						}
					}
					if (found == false) {
						printf(" "); //a space instead of a plane
					}
				}
				printf("| ");
				int m = positions_of_planes_on_x_axis.size();
				if (m != 0)
					printf("%i",m);
				printf("\n");
			}
			printf("x     - ");
			printf("|------------------------------------------------------------|\n");
		}
		//fflush(stdout);

		pthread_mutex_unlock( &printf_mutex );
		//tracker_mutex.unlock();

		end_cycle = ClockCycles( );
	    ncycles = end_cycle - start_cycle;

	    double delta_Time = (double) ncycles / cps;
	    atc_system_time += delta_Time;
	}

	return;
}

//15/11/2019
//very simple system... no need for sweep and pruning or complex collision detection...
//simply verify for all planes in the sector and check their distance difference...
//in a circle, of X km... send message to communication system c_communication_system
void c_atc::run_collision_detection() {

	//replace the while(1) with while(simulate) (bool, mutex)
	while (1) {

		if (terminate_system == true) {
			return;
		}

		coll_start_cycle = ClockCycles();

		pthread_mutex_lock(&printf_mutex );
		//tracker_mutex.lock();

		//mutex.lock();

		if (collision_detection_uptime >= 5.0) {
			collision_detection_uptime = 0;
			//minimal distance trigger_detection... is a radius...
			float minimal_distance_trigger = 5000; //m
			//printf("a");
			//dont forget to add mutex because it is gonna use the trackfile...
			vector<c_airplane*> tracked_airplanes = trackfile->tracked_airplanes;
			int num_tracked_airplanes = tracked_airplanes.size();
			//printf("%i\n", num_tracked_airplanes);
			for (int i = 0; i < num_tracked_airplanes; i++) {
				for (int j = 0; j < num_tracked_airplanes; j++) {

					//same plane... continue
					if (i == j) {
						break;
					}

					//airplane i
					float xi = tracked_airplanes[i]->get_x();
					float yi = tracked_airplanes[i]->get_y();
					float zi = tracked_airplanes[i]->get_z();

					//airplane j
					float xj = tracked_airplanes[j]->get_x();
					float yj = tracked_airplanes[j]->get_y();
					float zj = tracked_airplanes[j]->get_z();

					float distance = sqrt(pow((xi - xj), 2) + pow((yi - yj),2) + pow((zi - zj),2));
					//printf("%f\n", distance);
					if (distance <= minimal_distance_trigger) {
						//trigger collision system -> send message... the communication system will handle dispatching the commands to the fuckers
						//printf("PLANE %i is at %f from %i!\n", i, distance, j);
						int id1 = tracked_airplanes[i]->get_id();
						int id2 = tracked_airplanes[j]->get_id();
						string str = "collision alert : plane id ";
						str += to_string(id1);
						str += " is at ";
						str += to_string(distance);
						str += " from plane id ";
						str += to_string(id2) + ".";
						//printf("%s\n",str.c_str());
						alert_log.push_back(str.c_str());
						//fflush(stdout);
						//char rcvMESS[M_SIZE];
						//int replyID = MsgSend(coid_collision,"COLLISION",M_SIZE,rcvMESS,sizeof(rcvMESS));
					}
				}
			}
		}

		pthread_mutex_unlock( &printf_mutex );
		//tracker_mutex.unlock();

		//mutex.unlock();

		coll_end_cycle = ClockCycles( );
	    coll_ncycles = end_cycle - start_cycle;

	    double delta_Time = (double) ncycles / cps;
	    collision_detection_uptime += delta_Time;
	}
}
