#include <iostream>
#include <math.h>
#include <thread>
#include "TestCase.h"
#include "Airplane.h"

using namespace std;

void displayPlanPosition (Airplane plane) {
	
	plane.getPosition();
}
void collisionDet(Airplane a1, Airplane a2){
	a1.collisionDetection(a2);
}
TestCase::TestCase() {

}

int main()
{
	
	Airplane P1(0, 1, 0, 0, 0, 10, 10, 0);
	Airplane P2(1, -1, 0, 0, 20, 12, 10, 25);
	//thread t0(collisionDet,P1,P2);
	collisionDet(P1,P2);
	thread t1(displayPlanPosition,P1);
	thread t2(displayPlanPosition, P2);

	//t0.join();
	t1.join();
	t2.join();
	
	
	/*
	
	*/
	
}


