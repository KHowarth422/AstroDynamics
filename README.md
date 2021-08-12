# AstroDynamics
C++ code for time-marching Newton's law to simulate particles moving under gravitational force from one another. Can be applied for simulating planetary motion, playing with the three-body problem, etc.

**modelClasses.cpp & .h**
> C++ files containing the data structures and functions used to model the particles. The overall structure is a single Frame which uses a hashmap to contain any number of Particles. Each Particle has some two-dimensional position within the Frame, as well as some velocity, mass, etc. Additionally, each Particle is subject to gravitational forces from each other Particle in the same Frame. This gravitational force, along with user-specified initial velocity of each Particle, is what causes the Particles to move.

**main.cpp**
> C++ script for creating a few different Frames and time-marching all particles within the Frame over some user-specified duration. Default usage after compiling:
>> ./main.out -testcase
>
> Where testcase can be either "-earth", "-three_body", or "-solar". Or, to print debug information:
>> ./main.out -debug
>
> Note that this script may take a long time to run and produce a large amount of output data, depending on the timestep and duration of simulation chosen. The output data is saved as .txt files which contain the position information for each particle over the entire duration of simulation.

**plotFrame.m, plotSolar.m, plotThreeBody.m**
> MATLAB scripts for plotting the trajectories of each body in 2D space. As an example, here is the trajectory output from a four-body simulation:

![four-body](https://user-images.githubusercontent.com/66851249/129276750-0ae20e64-ffce-442f-993c-27226021b177.png)

