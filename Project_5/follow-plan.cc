/**
 * follow-plan.cc
 * Project 5, done by Irina Fedorova, Group 8.
 *
 **/

#include <iostream>
#include <fstream>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

player_pose2d_t readPosition(LocalizeProxy& lp);
void printRobotData(BumperProxy& bp, player_pose2d_t pose);
void printLaserData(LaserProxy& sp);

int  readPlanLength(void);
void readPlan(double *, int);
void printPlan(double *,int);
void writePlan(double *, int);

int main(int argc, char *argv[])
{

  // Variables
  int counter = 0;
  double speed;            // How fast do we want the robot to go forwards?
  double turnrate;         // How fast do we want the robot to turn?
  player_pose2d_t  pose;   // For handling localization data

  // The set of coordinates that makes up the plan

  int pLength;
  double *plan;

  // Set up proxies. These are the names we will use to connect to
  // the interface to the robot.
  PlayerClient    robot("localhost");
  BumperProxy     bp(&robot,0);
  Position2dProxy pp(&robot,0);
  LocalizeProxy   lp (&robot, 0);
  LaserProxy      sp (&robot, 0);

  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);


  // should stop.
  pLength = readPlanLength(); // Find out how long the plan is from plan.txt
  plan = new double[pLength]; // Create enough space to store the plan
  readPlan(plan, pLength);    // Read the plan from the file plan.txt.
  printPlan(plan,pLength);    // Print the plan on the screen
  writePlan(plan, pLength);   // Write the plan to the file plan-out.txt

  double speed, turnrate,diffx, diffy, angle;
  for(int i = 0; i < pLength; i = i + 2){ // putting in a loop for all pairs in the file


  // Main control loop
  while(std::abs(pose.px)-plan[i] > 0.01) || std::abs(pose.py-plan[i + 1]) > 0.01 )
    {
      // Update information from the robot.
      robot.Read();
      // Read new information about position
      pose = readPosition(lp);
      // Print data on the robot to the terminal
      printRobotData(bp, pose);
      // Print information about the laser. Check the counter first to stop
      // problems on startup
      if(counter > 2){
	printLaserData(sp);
      }

      if(bp[0] || bp[1]){
	speed= 0;
	turnrate= 0;
      }
      else {
	speed=.1;
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
  }
} // end of main()

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


void printLaserData(LaserProxy& sp)
{

  double maxRange, minLeft, minRight, range, bearing;
  int points;

  maxRange  = sp.GetMaxRange();
  minLeft   = sp.MinLeft();
  minRight  = sp.MinRight();
  range     = sp.GetRange(5);
  bearing   = sp.GetBearing(5);
  points    = sp.GetCount();

  //Uncomment this to print out useful laser data
  //std::cout << "Laser says..." << std::endl;
  //std::cout << "Maximum distance I can see: " << maxRange << std::endl;
  //std::cout << "Number of readings I return: " << points << std::endl;
  //std::cout << "Closest thing on left: " << minLeft << std::endl;
  //std::cout << "Closest thing on right: " << minRight << std::endl;
  //std::cout << "Range of a single point: " << range << std::endl;
  //std::cout << "Bearing of a single point: " << bearing << std::endl;

  return;
} // End of printLaserData()

void printRobotData(BumperProxy& bp, player_pose2d_t pose)
{

          // Print out what the bumpers tell us:
          std::cout << "Left  bumper: " << bp[0] << std::endl;
          std::cout << "Right bumper: " << bp[1] << std::endl;
          // Can also print the bumpers with:
          //std::cout << bp << std::endl;

          // Print out where we are
          std::cout << "We are at" << std::endl;
          std::cout << "X: " << pose.px << std::endl;
          std::cout << "Y: " << pose.py << std::endl;
          std::cout << "A: " << pose.pa << std::endl;


} // End of printRobotData()

int readPlanLength(void)
{
          int length;

          std::ifstream planFile;
          planFile.open("plan.txt");

          planFile >> length;
          planFile.close();

          // Some minimal error checking
          if((length % 2) != 0){
            std::cout << "The plan has mismatched x and y coordinates" << std::endl;
            exit(1);
          }

  return length;

} // End of readPlanLength

void readPlan(double *plan, int length)
{
          int skip;

          std::ifstream planFile;
          planFile.open("plan.txt");
          planFile >> skip;

          for(int i = 0; i < length; i++){
              planFile >> plan[i];
          }

       planFile.close();

} // End of readPlan

void printPlan(double *plan , int length)
{
             std::cout << std::endl;
             std::cout << "   x     y" << std::endl;
             for(int i = 0; i < length; i++){
             std::cout.width(5);
             std::cout << plan[i] << " ";

                   if((i > 0) && ((i % 2) != 0)){
                    std::cout << std::endl;
                   }
              }
              std::cout << std::endl;

} // End of printPlan

void writePlan(double *plan , int length)
{
          std::ofstream planFile;
          planFile.open("plan-out.txt");

          planFile << length << " ";
          for(int i = 0; i < length; i++){

            planFile << plan[i] << " ";
          }

        planFile.close();

} // End of writePlan
