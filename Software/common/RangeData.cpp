#include "RangeData.h"


RangeData::RangeData()
{
	dir = 0;
	x_count = 0;
	y_count = 0;
}

RangeData::~RangeData()
{
	
}

int IMUData::storeCommand(char* buf) {
	command = buf;
}

int RangeData::parseData()
{
	sscanf(command, "%f %c %f %f %f %f", &data.tstamp, &data.id, &data.laserL_a, &data.laserS_a, &data.laserL_b, &data.laserS_b);
	return 0;
}

int RangeData::getPosition(IMUData imu)
{	
	if(imu.m_yaw <= 45.0 && imu.m_yaw >= -45.0) { //north
		data.dir = 0;
	} else if (imu.m_yaw <= -45.0 && imu.m_yaw >= -135.0) { //east
		data.dir = 1;
	} else if (imu.m_yaw <= -135.0 && imu.m_yaw >= 135.0) { //south
		data.dir = 2;
	} else { //west
		data.dir = 3;
	}
	if(data.laserL_a != 8190) { //check if reading is valid LONG FRONT
		distance[0] = (data.laserL_a+29.76) * cos((3.1415926535*imu.m_yaw)/180) % 300; //30 - x - 15 = 15 - x 
	} else {
		distance[0] = 316; //impossible number for distance[0], as it's %300
	} 
	if(data.laserL_b != 8190) { //check if reading is valid LONG BACK
		distance[2] = (data.laserL_b+29.76) * cos((3.1415926535*imu.m_yaw)/180) % 300; //x - 15
	} else {
		distance[2] = 316; //impossible number for distance[2], as it's %300
	}
	if(data.laserS_a != 255) { //check if reading is valid SHORT RIGHT
		distance[1] = (data.laserS_a+29.76) * cos((3.1415926535*imu.m_yaw)/180) % 300; //30 - x - 15 = 15 - x 
	} else {
		distance[1] = 316;
	}
	if(data.laserS_b != 255) { //check if reading is valid SHORT LEFT
		distance[3] = (data.laserS_b+29.76) * cos((3.1415926535*imu.m_yaw)/180) % 300; //x - 15
	} else {
		distance[3] = 316;
	}

	/*Convert Readings into Cartersian Coordinates where (0,0) is the center of the cell*/
	distance[(0+data.dir)%4] = 15 - distance[(0+data.dir)%4]; //depending on the rotational orientation, to get the center as (15, 15)
	distance[(1+data.dir)%4] = 15 - distance[(1+data.dir)%4]; //we have to use the same reference, thus making north and east (15 - distance) for the coordinate, it's normally 30 - distance, but because of -15, it becomes 15 - distance 
	distance[(2+data.dir)%4] -= 15; //for south and west it is regularly just subtracting 15 
	distance[(3+data.dir)%4] -= 15;
	coord.x = 0; coord.y = 0; //default center (0,0)
	coord.x_flag = true; coord.y_flag = true; //assume readings are true at first

	for(int i = 0; i < 4; i++) {
		if((data.dir + i) % 2 == 1) { //x coords
			if(distance[(i+data.dir)%4] != 301) {
				coord.x += distance[(i+data.dir)%4];
				if(distance[(i+data.dir)%4] != coord.x){ //If you already went through more than one iteration, now you want to divide by 2
					coord.x /= 2;
				}
			} else {
				++x_count;
				if(x_count >= 2) {
					coord.x_flag = false;
				}
			}
		} else { //y coords
			if(distance[(i+data.dir)%4] != 301) {
				coord.y += distance[(i+data.dir)%4];
				if(distance[(i+data.dir)%4] != coord.y){ //If you already went through more than one iteration, now you want to divide by 2
					coord.y /= 2;
				}
			} else {
				++y_count;
				if(y_count >= 2) {
					coord.x_flag = false;
				}
			}
		}
	}


	return 0;
}
