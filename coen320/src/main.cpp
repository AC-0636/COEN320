#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>
#include "TestCase.h"
#include "Airplane.h"

#define update_view 5

using namespace std;

//thread 1 for plan view display, update every 5s
//thread 2 for log file output every 60s
//thread 3 for safety alarm
//thread 4 for ATC command


void timer_start(function<void(void)> func, unsigned int interval)
{
      while (true)
      {
          func();
          this_thread::sleep_for(chrono::milliseconds(interval));
      }
}

void print_plan()
{
	cout<<"update view every 5s" <<endl;
}

void history_log()
{

}

int main()
{
	timer_start(print_plan,5000);
	while (true);
}
