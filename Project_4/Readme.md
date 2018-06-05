# 1. Description

This project is all about localization. First you’ll learn how to make the robot move around when it is localized without having to do anything. Then you’ll move on to using the Player implementation of the particle filter.

# 2. Starting up

1. Login to your computer (user name is student, password is student) open terminal windows, get a copy of the project files (project4.tgz) from the instructor, put this on the Desktop and extract the files.
2. This time your starting point is the file _local-roomba.cc_
3. Start by running it. To do that start up player:
_player world41.cfg_\
Then you can compile and run local-roomba in your second window:\
_./build local-roomba_\
_./local-roomba_\
and as before the simulated robot will start moving
4. At first it may look exactly the same as before, but this time you’ll see the robot start to output information like:
We are at:

| X        | Y          | A  |
|:------------- |:-------------|:-----|
| -4.18556       | -7.81444 | -0.785398 |


These are the coordinates of the robot — it knows where it is.
5. Note that the robot isn’t calculating where it is, it is being told by the simulator Stage.
6. The challenge is to take local-roomba.cc and edit it so that the robot will drive from its starting point to the location X = 5, Y = −3.5.
7. You should do this without using odometry.
Hint: One way to move directly to its destination is to calculate the angle between the robot and its destination using trigonometry, turn so that this angle becomes zero and then drive right to the point.
Hint: If that doesn’t sound like a good solution, you can try turning until the robot is parallel with the x-axis and then moving until the difference between the x coordinate of the robot and the x coordinate of the destination is zero. Then do the same for the y coordinate.
> Hint: Proportional control, which is often effective for this kind of task, has  the robot move more quickly when it is far from the position/orientation it is aiming for, and more slowly as it gets closer. That way the robot doesn’t overshoot its target, but also doesn’t take too long to get into the neighborhood of the target.


# 3. Now real localization

1. The second part of this project is to use a laser with the simulated robot to really localize (where the robot computes its position).
2. This time you need to run:
_player world42.cfg_
You’ll see the same world as before, but this time the robot has a laser.
3. Compile and run the controller:
_./build real-local_
_./real-local_
and watch the robot run.
4. The robot, as it has at the start of previous projects, will trundle off to the south-east.
5. However, the display shows some new things.
6. The display is now dominated by a series of these:

| X | Y | A  | W  |
|:-----|:-----|:-----|:-----|
|4.05511    |4.68217    |-0.116743    | 0.005 |
|-2.45264	|2.15322	|0.076456     | 0.0035|
|2.55298	|0.762078   |-0.0410866   | 0.0031|
|-5.30263   |4.97457    |-0.0395662   | 0.0029|
|6.02264    |0.729982   |0.00262671   | 0.0024|
|4.01215    |-3.67916   |-0.0180902   | 0.0022|
|-0.675089  |5.43747    |-0.0741516   | 0.0018|

7.	Each of these is a hypothesis about where the robot is, with its x coordinate, y coordinate, angle and weight.
8.	The weight is an estimate of how likely the robot is to be at that location.

# 4.	It’s all about configuration

1.	Take a look at the configuration file world42.cfg.
2.	The difference between this configuration file and the others we used before is in the last element.
3.	The AMCL driver is a version of the particle filter that we talked about in the lecture on localization.
4.	All those possible positions you see on the screen are (more or less) the particles.

# 5.	Know where you are

1.	You should notice the number of hypotheses reduces as the robot runs.
2.	Your first task is to modify the controller so that the robot localizes properly.
3.	That means so that the robot ends up with just one or two hypotheses.
4.	If there is one, then clearly localization has figured out where it thinks the robot is.
5.	If there are two, then as long as one has a probability of very close to 1, localization thinks it has figured out where the robot is.
6.	Your job is to get the robot so that localization not only has one or two hypotheses, but that at least one of them is reasonable. As in it is close to where the robot really is (which you can see from the simulator).
7.	Don’t expect localization to be correct, but it should be approximately correct.
8.	You will probably want to make the robot moves in different ways to do this — moving robots localize quicker than stationary ones, and having the robot rotate, so that its sensors “see” different bits of the world, usually helps as well.

# 6.	Know that you know where you are

1.	When localization is generating just a couple of hypotheses, it has figured out where it thinks the robot is.
2.	But does the robot control program, the bit in main, know that the position reported by the localization module is the right position?
3.	Modify the controller so that when localization is down to a few hypotheses, the controller stops the robot moving.
4.	Have the controller print out a message from main that says:
_I am at:_
and then the location and then:
_I am N percent sure of my position where N is the weight of the most likely hypothesis._
5.	To do this you will have to modify the function readPosition since right now this just returns the location in the last hypothesis returned from localization (which is typically not the most likely one).
6.	Once your controller stops the robot when the robot is localized, you can add the code from your solution to part 1, so that the robot can navigate to the destination point.
7.	When you are done, save your program as (your-names)-proj4-part2.cc and make sure you put your name in the comments.
8.	You’ll need to submit this to me after you are done with the project.
# 7.	Handling Proxies
All the relevant commands for the localization and laser proxies are demonstrated in the code.