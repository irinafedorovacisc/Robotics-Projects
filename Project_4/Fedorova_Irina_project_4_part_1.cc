#include <iostream>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

/**
 * Function headers
 *
 **/

player_pose2d_t readPosition(LocalizeProxy& lp);
void printRobotData(BumperProxy& bp, player_pose2d_t pose);

/**
 * main()
 *
 **/

int main(int argc, char *argv[])
{


  // Variables
  int counter = 0;
  double speed;            // How fast do we want the robot to go forwards?
  double turnrate;         // How fast do we want the robot to turn?
  player_pose2d_t  pose;   // For handling localization data

  //extra pointers for avoid function
  double *p = &speed;
  double *p2 = &turnrate;

  // Set up proxies. These are the names we will use to connect to
  // the interface to the robot.
  PlayerClient    robot("localhost");
  BumperProxy     bp(&robot,0);
  Position2dProxy pp(&robot,0);
  LocalizeProxy   lp (&robot, 0);

  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);

  // Main control loop
  while(true)
    {
      robot.Read();
      pose = readPosition(lp);
      printRobotData(bp, pose);
      double angle = pose.pa;

      if(angle < 0.001 && pose.px < 0.0){
            speed = 0;
            turnrate = 0.5;
      }
       else if(   (pose.px>4.0)&&( pose.px <= 5.0)  )
            {
            speed= (double)(1.50/(pose.px));
            turnrate = 0.0;
            }
      else if( angle > 0.01 && pose.px <= 5.0)
      {
            speed = 1.5;
            turnrate = 0.0;
      }

      else if (( pose.px > 5.0) && (angle > 0 && angle < 1.5))
      {
            speed = 0.;
            turnrate = 0.2;
      }

      else if ((angle > 1.4) && (pose.py <=-3.5))
      {
            speed = 1.5;
            turnrate = 0.0;
      }
      else if ((pose.py <=-2.5) && (pose.py >=-3.5))
      {
            speed= (double)(-1.50/(pose.py));
            turnrate = 0.0;
      }

      else if(bp[0] || bp[1]){
        speed= 0;
        turnrate= 0;
      }
      else {
        speed=.0;
        turnrate = 0;
      }

      // What are we doing?
      std::cout << "Speed: " << speed << std::endl;
      std::cout << "Turn rate: " << turnrate << std::endl << std::endl;

      // Send the commands to the robot
      pp.SetSpeed(speed, turnrate);
      // Count how many times we do this
      counter++;
    }

} // end of main()

//here is modified code from the part 2
/**
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



**/



/**
 * readPosition()
 *
 * Read the position of the robot from the localization proxy.
 *
 * The localization proxy gives us a hypothesis, and from that we extract
 * the mean, which is a pose.
 *
 **/

player_pose2d_t readPosition(LocalizeProxy& lp)
{

  player_localize_hypoth_t hypothesis;
  player_pose2d_t          pose;
  uint32_t                 hCount;

  // Need some messing around to avoid a crash when the proxy is
  // starting up.

  hCount = lp.GetHypothCount();

  if(hCount > 0){
    hypothesis = lp.GetHypoth(0);
    pose       = hypothesis.mean;
  }

  return pose;
} // End of readPosition()


/**
 *  printRobotData
 *
 * Print out data on the state of the bumpers and the current location
 * of the robot.
 *
 **/

void printRobotData(BumperProxy& bp, player_pose2d_t pose)
{

  // Print out what the bumpers tell us:
  std::cout << "Left  bumper: " << bp[0] << std::endl;
  std::cout << "Right bumper: " << bp[1] << std::endl;

  // Print out where we are
  std::cout << "We are at" << std::endl;
  std::cout << "X: " << pose.px << std::endl;
  std::cout << "Y: " << pose.py << std::endl;
  std::cout << "A: " << pose.pa << std::endl;


} // End of printRobotData()

