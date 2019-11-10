#include <iostream>
#include <math.h>
#include <thread>
#include "TestCase.h"
#include "Airplane.h"
using namespace std;
void dummy1() {
	Airplane P1(0, -641, 283, 500, 95000, 101589, 10000, 13);
	P1.getPosition();
}
void dummy2 () {
	Airplane P2(1, -223, -630, -526, 71000, 100000, 13000, 16);
	P2.getPosition();
}

int main()
{

	thread t1(dummy1);
	thread t2(dummy2);
	t1.join();
	t2.join();
	
	

	
	


}


