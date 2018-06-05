# 1. Description
This project is all about localization. First you’ll learn how to make the robot move around when it is localized without having to do anything. Then you’ll move on to using the Player implementation of the particle filter.
# 2. Starting up
1. Login to your computer (user name is student, password is student) open terminal windows, get a copy of the project files (project4.tgz) from the instructor, put this on the Desktop and extract the files.
2. This time your starting point is the file _local-roomba.cc_
3. Start by running it. To do that start up player:
_player world41.cfg_
Then you can compile and run local-roomba in your second window:
_./build local-roomba_
_./local-roomba_
and as before the simulated robot will start moving
4. At first it may look exactly the same as before, but this time you’ll see the robot start to output information like:
We are at
X: | -4.18556
Y: | -7.81444
A: |  -0.785398
---:|:---

These are the coordinates of the robot — it knows where it is.
5. Note that the robot isn’t calculating where it is, it is being told by the simulator Stage.
6. The challenge is to take local-roomba.cc and edit it so that the robot will drive from its starting point to the location X = 5, Y = −3.5.
7. You should do this without using odometry.
Hint: One way to move directly to its destination is to calculate the angle between the robot and its destination using trigonometry, turn so that this angle becomes zero and then drive right to the point.
Hint: If that doesn’t sound like a good solution, you can try turning until the robot is parallel with the x-axis and then moving until the difference between the x coordinate of the robot and the x coordinate of the destination is zero. Then do the same for the y coordinate.
> Hint: Proportional control, which is often effective for this kind of task, has  the robot move more quickly when it is far from the position/orientation it is aiming for, and more slowly as it gets closer. That way the robot doesn’t overshoot its target, but also doesn’t take too long to get into the neighborhood of the target.
8. When you are done, save your program as (your-names)-proj4-part1.cc and make sure you put your name in the comments.
9. You’ll need to submit this to me after you are done with the project.