#include "c_atc.h"

using namespace std;
using namespace chrono;

c_atc::c_atc(c_environment* env_) {

	env = env_;

	

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
	last_trackfile = trackfile;

	//radar = 0; //initialize with init_radar(env_);
	radar = new c_radar(env_, trackfile);

	//create thread here and scan();
	//radar_thread = thread(&c_radar::scan, radar);

	//initialize comm_system
	printf("Initializing communication system...\n");

	running = true;

	printf("ATC system UP and running!\n");

	comm_system = new c_comm_system(chid, trackfile);

	

	//gui_thread = thread(&c_atc::draw_gui, this);
	pthread_create(&message_server_thread, NULL, &c_atc::static_run_message_server, this);
	pthread_create(&gui_draw_thread, NULL, &c_atc::static_draw_gui, this);
	pthread_create(&collision_detection_thread, NULL, &c_atc::static_run_collision_detection, this);
	pthread_create(&radar_thread, NULL, &c_atc::static_radar_scan, radar);
	pthread_create(&comm_thread, NULL, &c_atc::static_run_comm_system, comm_system);

	//log
	log_file.open("atc_system_log.data", ios::app);

	time_t start = system_clock::to_time_t(system_clock::now());
	ctime(&start);

	log_file << "\n[ATC SESSION : " << start << " ]" << endl;

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

	log_file.close();
}

void c_atc::run_message_server() {
	while (1) {

		if (terminate_system == true) {
			return;
		}

		pthread_mutex_lock(&printf_mutex );

		trackfile = radar->get_trackfile();
		comm_system->set_trackfile(trackfile);

		//check if plane has exited the air space...
		for (int i = 0; i < trackfile->tracked_airplanes.size(); i++) {
			bool found = false;
			for (int j = 0; j < last_trackfile->tracked_airplanes.size(); j++) {
				if (trackfile->tracked_airplanes[i]->get_id() == last_trackfile->tracked_airplanes[j]->get_id()) {
					found = true;
					break;
				}
			}
			if (found == false) {
				string message_to_log = "airplane with id " +  to_string(trackfile->tracked_airplanes[i]->get_id()) + " has exited the airspace...";
				alert_log.push_back(message_to_log);
				log_file << message_to_log << endl;
			}
		}

		//char 0 is argument 1, char 1 is second argument, or 2 char is plane altitude for example etc...
		//for plane change position [
		char Server_Messages[M_SIZE];

		int rcvid = MsgReceive(chid,Server_Messages,sizeof(Server_Messages),NULL);
		//printf(Server_Messages);

		char message[M_SIZE] = "\n40 OK\n";
		MsgReply(rcvid,EOK, message, sizeof (message));

		//dont forget that the size of the message is 40 bytes... all that is more is not considered!
		vector<string> command_items =  explode(Server_Messages, " ");
		//help debugging!
		/*for (unsigned int item = 0; item < command_items.size(); item++) {
			printf("%s\n", command_items[item].c_str());
		}*/

		//TODO NOW : process server message SEND COMMAND TO AIRPLANE OK
		//ALSO SHOW AIRPLANE_ID FOR DETAILS ABOUT THE AIRPLANE OK
		//ADD HASH VALUE TO AIRPLANE TO PROTECT IT FROM ADS-B HIJACKING...
		//PREPARE HOLD AND UNHOLD METHODS... OK
		//IN ALERT LOG, OUTPUT WHEN PLANE EXIT...
		
		//CHANGE CLOCK TO REAL TIME CLOCK

		
		//make change altitude

		if (strcmp(command_items[0].c_str(), "altitude") == 0) {
			if (command_items.size() < 3) {
				printf("atc_system : missing parameters for command - altitude -\n");
			} else {
				int db_id = 0;
				int altitude = 0;
				try {
					db_id = stoi(command_items[1].c_str());
					altitude = stoi(command_items[2].c_str());
				}
				catch (invalid_argument const &e) {
					printf("atc_system : [Message Server] arguments are invalid, value must be an integer...\n");
				}
				catch (out_of_range const &e) {
					printf("atc_system : [Message Server] arguments are invalid, integer value is out_of_range...\n");
				}
				if (db_id != 0) {
					if (db_id <= trackfile->tracked_airplanes.size()) {
						trackfile->tracked_airplanes[db_id - 1]->set_target_altitude(altitude, false); //not entry change altitude... this is command activated
						printf("atc_system : [Message Server] airplane %i new target altitude is %im.\n", db_id, altitude);
						string message_to_log = "atc_system : [Message Server] airplane " + to_string(db_id) + " new target altitude is " + to_string(altitude) + "m";
						log_file << message_to_log << endl;
					} else {
						printf("atc_system : [Message Server] db_id is listed from the trackfile, please select airplane from command - show airplanes - \n");
					}
				} else {
					printf("atc_system : [Message Server] db_id is listed from the trackfile, please select airplane from command - show airplanes - \n");
				}
			}
		}

		if (strcmp(command_items[0].c_str(), "hold") == 0 || strcmp(command_items[0].c_str(), "unhold") == 0 || strcmp(command_items[0].c_str(), "airplane") == 0) {
			bool show_airplane_data = false;
			if (strcmp(command_items[0].c_str(), "airplane") == 0) {
				show_airplane_data = true;
			}
			if (command_items.size() < 2) {
				if (show_airplane_data == true) {
					printf("atc_system : missing parameter for command - airplane -\n");
				} else {
					printf("atc_system : missing parameter for commands - hold/unhold -\n");
				}
			} else {
				int db_id = 0;
				try {
					int i = stoi(command_items[1].c_str());
					//cout << i << '\n';
					db_id = i;
				}
				catch (invalid_argument const &e) {
					printf("atc_system : [Message Server] argument 1 is an invalid, value must be an integer...\n");
				}
				catch (out_of_range const &e) {
					printf("atc_system : [Message Server] argument 1 is an invalid, integer value is out_of_range...\n");
				}

				if (db_id != 0) {
					if (db_id <= trackfile->tracked_airplanes.size()) {
						if (strcmp(command_items[0].c_str(), "hold") == 0) {
							trackfile->tracked_airplanes[db_id-1]->set_on_hold();
							printf("atc_system : [Message Server] airplane %i is in holding pattern.\n", db_id);
							string message_to_log = "atc_system : [Message Server] airplane " + to_string(db_id) + " is in holding pattern.";
							log_file << message_to_log << endl;

						} else if (strcmp(command_items[0].c_str(), "unhold") == 0) {
							trackfile->tracked_airplanes[db_id-1]->unset_on_hold();
							printf("atc_system : [Message Server] airplane %i is unholded.\n", db_id);
							string message_to_log = "atc_system : [Message Server] airplane " + to_string(db_id) + " is unholded.";
							log_file << message_to_log << endl;
						} else if (show_airplane_data == true) {
							printf("\n%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s\n", "db_id", "id", "x", "y", "z","speed_x","speed_y","speed_z", "is_hold");
							vector<c_airplane*> tracked_airplanes = trackfile->tracked_airplanes;
							int i = db_id - 1;
							int id = tracked_airplanes[i]->get_id();
							float x = tracked_airplanes[i]->get_x();
							float y = tracked_airplanes[i]->get_y();
							float z = tracked_airplanes[i]->get_z();
							float speed_x = tracked_airplanes[i]->get_speed_x();
							float speed_y = tracked_airplanes[i]->get_speed_y();
							float speed_z = tracked_airplanes[i]->get_speed_z();
							bool is_hold = tracked_airplanes[i]->get_is_hold();
							printf("%-10i%-10i%-10.0f%-10.0f%-10.0f%-10.0f%-10.0f%-10.0f%-10i\n", i+1, id, x,y,z,speed_x,speed_y,speed_z,is_hold);

							vector<string> airplane_log = tracked_airplanes[i]->get_airplane_log();
							if (airplane_log.size() > 0) {
								int h = airplane_log.size();
								printf("Airplane log (%i): \n", h);
								for (unsigned int p = 0; p < h; p++) {
									printf("%i. %s\n", p+1, airplane_log[p].c_str());
								}
							}
						}
					} else {
						printf("atc_system : [Message Server] db_id is listed from the trackfile, please select airplane from command - show airplanes - \n");
					}
				} else {
					printf("atc_system : [Message Server] db_id is listed from the trackfile, please select airplane from command - show airplanes - \n");
				}
			}
		}

		if (strcmp(command_items[0].c_str(), "send") == 0) {
			//send id message
			if (command_items.size() < 3) {
				printf("atc_system : missing parameters for command - send -\n");
			} else {
				int set_counter = 0;
				int db_id = 0;
				string message = "";
				for (unsigned items = 1; items < command_items.size(); items++) {
					if (items > 3) {
						break;
					}
					if (items == 1) {
						try {
							int i = stoi(command_items[items].c_str());
							//cout << i << '\n';
							db_id = i;
							set_counter++;
						}
						catch (invalid_argument const &e) {
							printf("atc_system : [Message Server] argument %i is an invalid, value must be an integer...\n", items);
						}
						catch (out_of_range const &e) {
							printf("atc_system : [Message Server] argument %i is an invalid, integer value is out_of_range...\n", items);
						}
					}
					if (items == 2) {
						message = command_items[2].c_str();
						set_counter++;
					}
				}

				if (set_counter == 2) {
					if (db_id <= trackfile->tracked_airplanes.size()) {
						//bool set_reply = PRINTF IF AIRPLANE SUCCEDDED OPERATON OR NOT...
						trackfile->tracked_airplanes[db_id - 1]->get_message(message.c_str());
						printf("atc_system : [Message Server] confirmed sent message to %i '%s'.\n", db_id, message.c_str());
						string message_to_log = "atc_system : [Message Server] confirmed sent message to " + to_string(db_id) + " " + message;
						log_file << message_to_log << endl;
					} else {
						printf("atc_system : [Message Server] db_id is listed from the trackfile, please select airplane from command - show airplanes - \n");
					}
				}
			}
		}

		if (strcmp(Server_Messages, "add_remove_airplane") == 0) {
			printf("atc_system : cannot execute add/remove airplane command, only available in simulation mode!\n");
		}

		//here we have to process the command to get the parameters that will be variable...
		//first command...
		if (strcmp(command_items[0].c_str(), "set") == 0) {
			//check if arguments exists...
			//set db_id x y z... 5 items...
			if (command_items.size() < 5) {
				printf("atc_system : missing parameters for command - set -\n");
			} else {
				//just count if all parameters where set... if not don't send command from the ATC system to the airplane...
				int set_counter = 0;
				//YEAH. Whatever... not recommended but we make things quick...
				int db_id = 0;
				float x = 0;
				float y = 0;
				float z = 0;
				//start at 1 because 0 is "set"
				for (unsigned items = 1; items < command_items.size(); items++) {
					//note that this part may contain a format string vulnerability...
					//if we catch that the user didnt inputed an integer.. we catch an exception...
					if (items > 4) {
						//we just fetch all them for fun... but the rest we dont need them...
						break;
					}
					try {
						//some strings are considered ints... they have a conversion, should study stoi method more...
						int i = stoi(command_items[items].c_str());
						//cout << i << '\n';
						switch (items) {
						case 1: //airplane id
							db_id = i;
							set_counter++;
							break;
						case 2: //x
							x = (float)i;
							set_counter++;
							break;
						case 3:
							y = (float)i;
							set_counter++;
							break;
						case 4:
							z = (float)i;
							set_counter++;
							break;
						default:
							break;
						}
					}
					catch (invalid_argument const &e) {
						printf("atc_system : [Message Server] argument %i is an invalid, value must be an integer...\n", items);
					}
					catch (out_of_range const &e) {
						printf("atc_system : [Message Server] argument %i is an invalid, integer value is out_of_range...\n", items);
					}
				}

				//EFFECTIVELY AFFECT SET COMMAND... HERE...
				//HERE (GET AIRPLANES FROM TRACKFILE... SET X Y Z
				if (set_counter == 4) {
					if (db_id < trackfile->tracked_airplanes.size()) {
						//bool set_reply = PRINTF IF AIRPLANE SUCCEDDED OPERATON OR NOT...
						trackfile->tracked_airplanes[db_id - 1]->set_current_target(x,  y, z);
						printf("atc_system : [Message Server] confirmed new target for airplane %i (%.2f, %.2f, %.2f).\n", db_id, x, y, z);
						string message_to_log = "atc_system : [Message Server] confirmed new target for airplane " + to_string(db_id) + " (" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
						log_file << message_to_log << endl;
					} else {
						printf("atc_system : [Message Server] db_id is listed from the trackfile, please select airplane from command - show airplanes - \n");
					}
				}
			}
		}

		//we process the message... BECAUSE THE ATC IS ACTING AS THE SERVER
		//TODO ... limit output to 5 latest alerts... maybe add command param to show n columns of alerts...
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
			printf("\n%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s\n", "db_id", "id", "x", "y", "z","speed_x","speed_y","speed_z","is_hold");
			for (unsigned int i = 0; i < tracked_airplanes.size(); i++) {
				int id = tracked_airplanes[i]->get_id();
				float x = tracked_airplanes[i]->get_x();
				float y = tracked_airplanes[i]->get_y();
				float z = tracked_airplanes[i]->get_z();
				float speed_x = tracked_airplanes[i]->get_speed_x();
				float speed_y = tracked_airplanes[i]->get_speed_y();
				float speed_z = tracked_airplanes[i]->get_speed_z();
				bool is_hold = tracked_airplanes[i]->get_is_hold();
				printf("%-10i%-10i%-10.0f%-10.0f%-10.0f%-10.0f%-10.0f%-10.0f%-10i\n", i+1, id, x,y,z,speed_x,speed_y,speed_z,is_hold);
			}
		}

		if (strcmp(Server_Messages, "show radar") == 0) {
			trackfile = radar->get_trackfile();
			comm_system->set_trackfile(trackfile);
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

		last_trackfile = trackfile;
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

			//make output atomic...
			string output = "";

			output += "\n        0km                                                          100km";
			output += "\nx     - ";
			output += "|------------------------------------------------------------|\n";
			//printf("\n        0km                                                          100km");
			//printf("\nx     - ");
			//printf("|------------------------------------------------------------|\n");
			//for every row we check if there is a plane on the x and draw them with printf
			for (int i = 20; i >= 0; i--) { //10
				//printf("      ");
				if (i == 20) {
					output += "100km - ";
					//printf("100km - ");
				} else if (i == 1) {
					output += "5km   - ";
					//printf("5km   - ");
				} else if (i == 0) {
					output += "0km   - ";
					//printf("0km   - ");
				} else {
					output += to_string(i*5);
					output += "km  - ";
					//printf("%ikm  - ",i*5);
				}
				output += "|";
				//printf("|");
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
							output += "*";
							//printf("*");
							found = true;
							break; //skip all other planes... we already marked the spot
						}
					}
					if (found == false) {
						output += " ";
						//printf(" "); //a space instead of a plane
					}
				}
				output += "| ";
				//printf("| ");
				int m = positions_of_planes_on_x_axis.size();
				if (m != 0)
					output += to_string(m);
					//printf("%i",m);
				output += "\n";
				//printf("\n");
			}
			output += "x     - ";
			output += "|------------------------------------------------------------|\n";

			printf(output.c_str());
			//printf("x     - ");
			//printf("|------------------------------------------------------------|\n");
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

		//this clock and system update simulates as if we using a very precise and instrumented collision detection system..
		if (collision_detection_uptime >= 10.0) {
			collision_detection_uptime = 0;
			//minimal distance trigger_detection... is a radius...
			float minimal_distance_trigger = 10000; //m... so its 10km...
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
						//trigger collision system -> send message... the communication system will handle dispatching the commands
						//printf("PLANE %i is at %f from %i!\n", i, distance, j);
						int id1 = tracked_airplanes[i]->get_id();
						int id2 = tracked_airplanes[j]->get_id();
						string str = "collision alert : plane id ";
						str += to_string(id1);
						str += " is at ";
						str += to_string((int)(trunc(distance)));
						str += "m from plane id ";
						str += to_string(id2) + ".";
						//printf("%s\n",str.c_str());
						alert_log.push_back(str.c_str());
						log_file << str << endl;
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
