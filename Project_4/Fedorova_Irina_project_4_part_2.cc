/*
* Project 4 part 2 - done by Irina Fedorova Group 8
*  
*
*/

#include <iostream>
#include <cmath>
#include <fstream>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;  

//Function headers
player_pose2d_t readPosition(LocalizeProxy& lp, std::ofstream&); //ofstream added
void printLaserData(LaserProxy& sp, std::ofstream&);
void printRobotData(BumperProxy& bp, player_pose2d_t pose, std::ofstream&);

int main(int argc, char *argv[]) {  
	
	player_pose2d_t pose;   
	player_laser_data laser; 
	PlayerClient robot("localhost");  
	BumperProxy bp(&robot,0);  
	Position2dProxy pp(&robot,0);
	LocalizeProxy lp (&robot, 0);
	LaserProxy sp (&robot, 0);
	
	bool test = false;
	std::ofstream log_file;
	log_file.open("record.txt"); //log_file to record the data
	double speed, turnrate, diffy, diffx, angle;
	
	pp.SetMotorEnable(true);
	pp.SetSpeed(0.2, 0.1);	//set initial values of speed and turn rate

	while (!test) {	//to verify if we localize successfully 
    	robot.Read();
    	pose = readPosition(lp, log_file);
	if (lp.GetHypothCount() > 0)	
        if (lp.GetHypoth(0).alpha > .9 && lp.GetHypothCount() < 3)
	       test = true;	
    }

	if (pose.px < -4 && pose.py < -4) {	//check if localization succeeded
		//print success message to the screen
		std::cout << "Success!  ";
		std::cout << "I am at (" << pose.px << "," << pose.py << ").";
		std::cout << "I am " << lp.GetHypoth(0).alpha*100 << " %/ sure of my location." << std::endl;
	
		//write success message to log file
		log_file << "Success!  ";
		log_file << "I am at (" << pose.px << "," << pose.py << ").";
		log_file << "I am " << lp.GetHypoth(0).alpha*100 << " %/ sure of my location." << std::endl;
				  
	
		while (pose.px < 4.9999 || pose.py < -3.5001) {	//navigate to the position (5.0 , 3.5)
			robot.Read();
			pose = readPosition(lp, log_file);
			printRobotData(bp, pose, log_file);
			printLaserData(sp, log_file);
			
			if (sp.MinLeft() < .5) { //if too close - move away
				turnrate = sp.MinLeft() - 2;
				speed = sp.MinLeft()/2;
			}
			else if (sp.MinRight() < .5) {	
				turnrate = 2 - sp.MinRight();
				speed = sp.MinRight()/2;
			}
			else {
				diffy = -3.5 - pose.py; //otherwise heading to the destination
				diffx = 5 - pose.px;
				angle = atan2(diffy, diffx) - pose.pa;
				
				turnrate = angle;	
				if (angle < 0.0001)	
				speed = sqrt(diffy*diffy+diffx*diffx);

				else speed = 0;
			}
			//print info
			log_file << "Speed: " << speed << std::endl;      
			log_file << "Turn rate: " << turnrate << std::endl << std::endl;
			pp.SetSpeed(speed, turnrate);
		}
		std::cout << "Success!  "; //print to the screen 
		std::cout << "I am at (" << pose.px << "," << pose.py << ")." << std::endl;
		
		//print success message to log_file
		log_file << "Success!  ";
		log_file << "I am at (" << pose.px << "," << pose.py << ")." << std::endl;
	}
	else {
		std::cout << "failed!" << std::endl; //failed, do the reset
	}
	log_file.close();
	return 0;
}


// Function provided by sample code
 
player_pose2d_t readPosition(LocalizeProxy& lp, std::ofstream& log_file) {
	player_pose2d_t pose;
	double weight;
	uint32_t hCount = lp.GetHypothCount();
	
	//Print AMCL data
	log_file << "AMCL gives us " << hCount + 1 << " possible locations:" << std::endl;
	if(hCount > 0){	//check first, to avoid seg fault when booting up
	    for(int i = 0; i <= hCount; i++){
    		pose       = lp.GetHypoth(i).mean;
    		weight     = lp.GetHypoth(i).alpha;
    		log_file << "X: " << pose.px << "\t";
    		log_file << "Y: " << pose.py << "\t";
    		log_file << "A: " << pose.pa << "\t";
    		log_file << "W: " << weight  << std::endl;
		}
		return lp.GetHypoth(0).mean;	//hypothesis
	}
	return pose;
}

/* Function provided by sample code / writing to the log_file added
 */
void printLaserData(LaserProxy& sp, std::ofstream& log_file) {
        log_file << "Laser says..." << std::endl;
	log_file << "Maximum distance I can see: " << sp.GetMaxRange() << std::endl;
	log_file << "Number of readings I return: " << sp.GetCount() << std::endl;
	log_file << "Closest thing on left: " << sp.MinLeft() << std::endl;
	log_file << "Closest thing on right: " << sp.MinRight() << std::endl;
	return;
}

void printRobotData(BumperProxy& bp, player_pose2d_t pose, std::ofstream& log_file) {
	
	log_file << "Left  bumper: " << bp[0] << std::endl;
	log_file << "Right bumper: " << bp[1] << std::endl;
	log_file << "We are at" << std::endl;
	log_file << "X: " << pose.px << std::endl;
	log_file << "Y: " << pose.py << std::endl;
	log_file << "A: " << pose.pa << std::endl;
	return;
}
