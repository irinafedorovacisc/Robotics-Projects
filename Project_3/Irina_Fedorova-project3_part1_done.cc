#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
using namespace PlayerCc;

// Set up proxies
PlayerClient    robot("localhost");
BumperProxy     bp(&robot,0);
Position2dProxy pp(&robot,0);
PlayerClient    blobfinder("localhost");
BlobfinderProxy bf(&blobfinder,0);

// Variables

player_blobfinder_data_t  blobList;   // Structure holding the blobs found
player_blobfinder_blob_t* theBlobs;   // A point to a list of blobs
player_blobfinder_blob_t  myBlob;     // A single blob

// Allow the program to take charge of the motors (take care now)
pp.SetMotorEnable(true);

// Control variables
bool blobFound = false;
bool running = true;

double turnrate, speed;
int largestBlob = 0;
double largestArea = 0;
// Control loop
while(running) {
	// Read from the proxies
	robot.Read();
	blobfinder.Read();

	//move only if bumpers not pressed

	if ((bp[0] == 0)&&(bp[1] == 0)) {
		if (bf.GetCount() > 0){

			// Blog found objects
			std::cout << bf.GetCount() << " Blobs found! \n";

			for(int i = 0; i < bf.GetCount(); i++) {

				//Determines if blob is red
				if ((short)bf.GetBlob(i).color == 0) {
					blobFound = true;

					if (bf.GetBlob(i).area > largestArea) {
						largestBlob = i;
						largestArea = bf.GetBlob(i).area;
					}
				}
			}

			if (blobFound) {
				std::cout << "Location: " << bf.GetBlob(largestBlob).x << std::endl;
				std::cout << "Area: " << bf.GetBlob(largestBlob).area << std::endl;
      }

			//center the blob
			if (bf.GetBlob(largestBlob).x < 160) {
				turnrate = .2;
			}

			else if (bf.GetBlob(largestBlob).x > 160) {
				turnrate = -.2;
			}

			else {
			turnrate = 0;
			}

			// Move to the blob and stop two feet in front of it
			if(bf.GetBlob(largestBlob).area > 1200) {
				running = false;
				speed = 0;
			}

			// Move At A Steady Pace Towards Blob
			else
			{
			speed = 200 / double(bf.GetBlob(largestBlob).area);
			}

		}

		else {
		  // Here the blobfinder does not see any blobs
			speed = 0;
			turnrate = 0.2;
		}
	}

	// If bumpers are pressed, do nothing.
	else{
		speed = 0;
		turnrate = 0;
	}

	pp.SetSpeed(speed, turnrate);
}
}
