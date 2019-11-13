/*
 * TestCase.h
 *
 *	Description: Test case for the demo of ATC project.
 *			     Instantiate and use member array.
 *
 */

#ifndef SRC_TESTCASE_H_
#define SRC_TESTCASE_H_

 /*
  * Class name: TestCase
  *
  * Description: Provides int array containing data of incoming aircraft provided by the radar.
  *
  * */
class TestCase {
public:
	//each line is an airplane: ID, speed_x, speed_y, speed_z, x, y, z, entry time.
	int airplane_schedule[160] = { 0, -641, 283, 500, 95000, 101589, 10000, 13,
	1, -223, -630, -526, 71000, 100000, 13000, 16,
	-1, -180, -446, -186, 41000, 100000, 6000, 31,
	3, 474, -239, 428, 38000, 0, 14000, 44,
	-1, -535, 520, 360, 95000, 100000, 1000, 49,
	-1, -164, -593, -501, 83000, 100000, 12000, 67,
	6, 512, 614, 520, 86000, -1571, 9000, 87,
	7, -275, 153, -399, 47000, 100000, 12000, 103,
	8, -129, 525, -300, 92000, 100000, 1000, 123,
	9, 437, 574, 339, 32000, 0, 8000, 129,
	10, 568, 538, 192, 50000, 0, 4000, 133,
	11, 347, 529, -599, 83000, -1799, 10000, 140,
	12, -512, -482, 578, 35000, 100000, 4000, 142,
	13, 619, -280, 194, 74000, 0, 10000, 157,
	14, -141, 427, -321, 41000, 102251, 11000, 165,
	15, -199, 242, -205, 56000, 100000, 4000, 172,
	16, 315, -197, -365, 77000, 0, 1000, 187,
	17, -138, 455, 602, 23000, 102290, 14000, 199,
	18, -150, 557, -356, 38000, 100000, 1000, 204,
	19, 194, 184, 598, 35000, 0, 2000, 221,
	};
	/*int airplane_schedule[140] = { 0, 10000, 10000, 0, 50000, 20000, 10000, 15,
		1, -10000, -10000, 0, 50000, 20000, 10000, 16,
		2, 0, 15000, 0, 50000, 20000, 10000, 31,
		3, 12000, 12000, 0, 50000, 20000, 10000, 44,
		4, -10000, -10000, 0, 60000, 120000, 10000, 16,
		5, 0, -15000, 0, 50000, 40000, 10000, 31,
		6, -12000, -12000, 0, 40000, 50000, 10000, 44,
		7, -10000, -10000, 0, 30000, 60000, 10000, 16,
		8, -10000, -15000, 0, 20000, 20000, 70000, 31,
		9, -12000, 0, 0, 10000, 20000, 10000, 44
	};*/

	TestCase();
};

#endif /* SRC_TESTCASE_H_ */
