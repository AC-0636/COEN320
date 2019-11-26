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
					}
					else {
						cout << "atc_system : - " << v[1] << " is not a valid argument." << endl;
					}
				} else {
					cout << "atc_system : - show - command needs at least 1 argument." << endl;
				}
			}
			else if (v[0] == "hold"){
					char rcvMESS[M_SIZE];
					string str ="hold " + v[1];
					const void * plane_id = str.c_str();
					int replyID = MsgSend(coid,plane_id,M_SIZE,rcvMESS,sizeof(rcvMESS));
			}
			// for unhold pattern
			else if (v[0] == "unhold"){
				char rcvMESS[M_SIZE];
						string str ="unhold " + v[1];
						const void * plane_id = str.c_str();
						int replyID = MsgSend(coid,plane_id,M_SIZE,rcvMESS,sizeof(rcvMESS));
			}
			else if (v[0] == "help") {
				cout << "commands : " << endl;
				cout << "show [arg1] - radar or airplanes or (int)airplane_id or alerts" << endl;
				cout << "set [arg1] [arg2] [arg3] - (int)airplane_id, x or y or z, int(position)" << endl; //z is altitude
				cout << "hold [arg1] - set airplane (int)airplane_id in hold mode" << endl;
				cout << "unhold [arg1] - set airplane (int)airplane_id in normal mode" << endl;
				//cout << "change [arg1] altitude -change airplane (int)airplane_id altitude"<<endl;
				cout << "add [arg1] - add plane with (int)airplane_id" << endl;
				cout << "remove [arg1] - remove plane (int)airplane_id" << endl;
				cout << "send [arg1] [arg2] - send message to (int)airplane_id - arg2 is one word" << endl;
				cout << "exit - terminate the ATC system" << endl;
			} else if (v[0] == "exit") {
				//cout << "EXIT HERE!" << endl;
				char rcvMESS[M_SIZE];
				MsgSend(coid,"exit",M_SIZE,rcvMESS,sizeof(rcvMESS));
				//printf(rcvMESS);
				//terminate the thread
				return;
			}
			else {
				cout << "atc_system : unrecognized command - " << v[0] << " - " << endl;
			}
		}

		pthread_mutex_unlock(&printf_mutex );

		end_cycle = ClockCycles( );
	    ncycles = end_cycle - start_cycle;

	    double delta_Time = (double) ncycles / cps;
	    comm_system_time += delta_Time;
	}
}
