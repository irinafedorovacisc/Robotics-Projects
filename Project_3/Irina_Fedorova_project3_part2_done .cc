#include <iostream>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

void printLaserData(LaserProxy& sp);
void printRobotData(BumperProxy& bp);

int main(int argc, char *argv[])
{
	int counter = 0;
	double speed, turnrate;
	player_laser_data laser;
        bool Go = true;

	//Set up proxies
	PlayerClient    robot("localhost");
	BumperProxy     bp(&robot,0);
	Position2dProxy pp(&robot,0);
	LaserProxy      sp (&robot, 0);
	pp.SetMotorEnable(true);

	while(Go)
    {
		//print
		robot.Read();
		if (counter > 2)
		printLaserData(sp);
		printRobotData(bp);

		if (bp[0] || bp[1]) { //if bumpers pressed - stop
			speed = 0;
			turnrate = 0;
		}
		else {
			if (sp.MinLeft() > sp.MinRight())
				turnrate = 1;
			else if (sp.MinLeft() < sp.MinRight())
				turnrate = -1;
			else
				turnrate = 0;
			        speed = 1;
//alternatively tried to keep robot in a middle of corridor(if sp.MinLeft() >1.6 go right if less go left,
//but this approach sometimes would fail at the corners)
			if (counter > 2) //used to bypass segmentation fault
			if (sp.GetRange(180) <= 1)
					Go = false;
		}

		//print
std::cout << "Speed: " << speed << std::endl;
std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
		pp.SetSpeed(speed, turnrate);
		counter++;
    }
}


       //print
void printLaserData(LaserProxy& sp)
{

  double maxRange, minLeft, minRight, range, bearing;
  int points;

  maxRange  = sp.GetMaxRange();
  minLeft   = sp.MinLeft();
  minRight  = sp.MinRight();
  points    = sp.GetCount();
  range     = sp.GetRange(180); //mid point 360/2
  bearing   = sp.GetBearing(180);

  std::cout << "Laser says..." << std::endl;
  std::cout << "Maximum distance I can see: " << maxRange << std::endl;
  std::cout << "Number of readings I return: " << points << std::endl;
  std::cout << "Closest thing on left: " << minLeft << std::endl;
  std::cout << "Closest thing on right: " << minRight << std::endl;
  std::cout << "Range of a single point: " << range << std::endl;
  std::cout << "Bearing of a single point: " << bearing << std::endl;

  return;
}

void printRobotData(BumperProxy& bp)
{

  std::cout << "Left  bumper: " << bp[0] << std::endl;
  std::cout << "Right bumper: " << bp[1] << std::endl;

  // Might want to add odometry data here also, but then you'd have
  // to pass the position proxy to the function.

} // End of printRobotData()

