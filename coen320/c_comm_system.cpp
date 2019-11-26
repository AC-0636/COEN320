#include "c_comm_system.h"

using namespace std;

//simply separate a string with the delimiter... similar to php explode() method
vector<string> explode(string str, string sep) {

    char* cstr=const_cast<char*>(str.c_str());
    char* current;

    vector<string> arr;

    current=strtok(cstr,sep.c_str());

    while(current!=NULL){

        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}

c_comm_system::c_comm_system(int chid_, s_tracker* trackfile_) {

	chid = chid_;
	coid = ConnectAttach(0,0,chid,0,0);

	//saves a copy of the pointer... USE MUTEX because if not race condition, crash or failures
	trackfile = trackfile_;

	//create thread here. join in destructor.
	//one thread for comm_in system and one for comm_out

	comm_system_time = 0;
	cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
	start_cycle = ClockCycles();
    end_cycle = ClockCycles( );
    ncycles = end_cycle - start_cycle;
}

c_comm_system::~c_comm_system() {}

void c_comm_system::run_comm_system() {

	while (1) {

		start_cycle = ClockCycles();

		if (terminate_system == true) {
			return;
		}

		pthread_mutex_lock(&printf_mutex);

		//gives a second after any input processed...
		nap(600);

		printf("#controller@atc_system > ");
		string input;

		//flush the input buffer...
		cin >> ws;
		getline(cin, input);

		vector<string> v = explode(input, string(" "));

		//command processor...
		if (v.size() > 0) {
			if (v[0] == "show") {
				if (v.size() > 1) {
					if (v[1] == "radar") {
						//sending show radar message to server
						char rcvMESS[M_SIZE];
						int replyID = MsgSend(coid,"show radar",M_SIZE,rcvMESS,sizeof(rcvMESS));
						//printf(rcvMESS);
					} else if (v[1] == "airplanes") {
						char rcvMESS[M_SIZE];
						int replyID = MsgSend(coid,"show airplanes",M_SIZE,rcvMESS,sizeof(rcvMESS));
						//printf(rcvMESS);
					} else if (v[1] == "alerts") {
						char rcvMESS[M_SIZE];
						int replyID = MsgSend(coid,"show alerts",M_SIZE,rcvMESS,sizeof(rcvMESS));
					} else {
						cout << "atc_system : - " << v[1] << " is not a valid argument." << endl;
					}
				} else {
					cout << "atc_system : - show - command needs at least 1 argument." << endl;
				}
			} else if (v[0] == "set" || v[0] == "send" || v[0] == "hold" || v[0] == "unhold" || v[0] == "airplane" || v[0] == "altitude") { //variable commands
				string full_command = "";
				for (int n = 0; n < v.size(); n++) {
					//set db_id x y z
					full_command += v[n].c_str();
					full_command += " "; //space delimiter
				}
				//debug...
				//printf("full_command : %s\n",full_command.c_str());
				char rcvMESS[M_SIZE];
				int replyID = MsgSend(coid,full_command.c_str(),M_SIZE,rcvMESS,sizeof(rcvMESS));
			} else if (v[0] == "add" || v[0] == "remove") {
				//we pretty dont care of the reply everytime we send commands with the IPC
				char rcvMESS[M_SIZE];
				int replyID = MsgSend(coid,"add_remove_airplane",M_SIZE,rcvMESS,sizeof(rcvMESS));
			} else if (v[0] == "help") {
				string output = "";
				output += "commands : \n";
				output += "show [arg1] - radar or airplanes or alerts\n";
				output += "airplane [arg1] - details about airplane db_id\n";
				output += "set [arg1] [arg2] [arg3] [arg4] - (int)db_id, x, y, z, - sets new target for airplane\n";
				output += "altitude [arg1] [arg2] - db_id, (int)altitude - sets new target altitude\n";
				output += "hold [arg1] - set airplane (int)db_id in hold mode\n";
				output += "unhold [arg1] - set airplane (int)db_id in normal mode\n";
				output += "add [arg1] - add plane with (int)db_id\n";
				output += "remove [arg1] - remove plane (int)db_id\n";
				output += "send [arg1] [arg2] - send message to (int)db_id - arg2 is one word\n";
				output += "exit - safely terminate the ATC system\n";
				printf("%s",output.c_str());
			} else if (v[0] == "exit") {
				//cout << "EXIT HERE!" << endl;
				char rcvMESS[M_SIZE];
				MsgSend(coid,"exit",M_SIZE,rcvMESS,sizeof(rcvMESS));
				//printf(rcvMESS);
				//terminate the thread
				//dont forget to unlock mutex here...????
				return;
			}
			else {
				cout << "atc_system : unrecognized command - " << v[0] << " - " << endl;
			}
		}

		pthread_mutex_unlock(&printf_mutex );

		//clock calculation...
		end_cycle = ClockCycles( );
	    ncycles = end_cycle - start_cycle;

	    double delta_Time = (double) ncycles / cps;
	    comm_system_time += delta_Time;
	}
}
