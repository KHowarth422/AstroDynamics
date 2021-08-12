#include "modelClasses.h"

Particle::Particle(std::string _ID, 
                   double _mass, 
                   std::pair<double, double> _pos, 
                   std::pair<double, double> _vel) : ID(_ID), mass(_mass), radius(0), net_force(std::make_pair(0, 0)) 
{
    pos.push_back(_pos);
    vel.push_back(_vel);
}

Particle::Particle(std::string _ID, 
                   double _mass, 
                   std::pair<double, double> _pos, 
                   std::pair<double, double> _vel,
                   double _radius) : ID(_ID), mass(_mass), radius(_radius), net_force(std::make_pair(0, 0)) 
{
    pos.push_back(_pos);
    vel.push_back(_vel);
}

bool Particle::operator== (const Particle& other) const {
    return this->ID == other.ID;
}

bool Particle::operator!= (const Particle& other) const {
    return this->ID != other.ID;
}

std::ostream& operator<< (std::ostream& ostr, const Particle& p) {
    // Ostream operator overload to print all information about a particle.
    // Only print the very first and final 4 history entries to avoid clogging data
    ostr << "Particle ID: " << p.ID << std::endl;
    ostr << "Mass: " << p.mass << std::endl;
    ostr << "Position History: [" << "(" << p.pos[0].first << ", " << p.pos[0].second << ") ... ";
    if (p.pos.size() >= 6) {
        for (unsigned int i = p.pos.size() - 5; i < p.pos.size(); i++ ) {
            ostr << "(" << p.pos[i].first << ", " << p.pos[i].second << ")";
            if (i < p.pos.size()-1 ) ostr << ", ";
        }
    }
    ostr << "]" << std::endl;
    ostr << "Velocity History: [" << "(" << p.vel[0].first << ", " << p.vel[0].second << ") ... ";
    if (p.vel.size() >= 6) {
        for (unsigned int i = p.vel.size() - 5; i < p.vel.size(); i++ ) {
            ostr << "(" << p.vel[i].first << ", " << p.vel[i].second << ")";
            if (i < p.vel.size()-1 ) ostr << ", ";
        }
    }
    ostr << "]" << std::endl;
    ostr << "Force Vector: (" << p.net_force.first << ", " << p.net_force.second << ")" << std::endl;
    return ostr;
}

void Particle::addForce(std::pair<double, double> F_vec) { 
    // Takes a pair as input rather than two doubles for convenient use with Frame::getGravitationalForceBetween(),
    // which outputs a pair.
    net_force.first += F_vec.first;
    net_force.second += F_vec.second;
}

void Particle::saveDataToTextFile(double dt) const {
    // Save all position and velocity history to [filename].txt
    assert (pos.size() == vel.size());
    std::ofstream ostr;
    double time = 0;
    ostr.open(ID + ".txt");
    ostr << "Time;Pos_X;Pos_Y;Vel_X;Vel_Y" << std::endl; 
    for (unsigned int i = 0; i < pos.size(); i++) {
        ostr << time << ";" << pos[i].first << ";" << pos[i].second << ";" << vel[i].first << ";" << vel[i].second << std::endl;
        time = time + dt;
    }
}

unsigned int ParticleHasher::operator() (const std::string& IDToHash) const {
    // Generic Hash Function for stl strings.
    // Source: http://www.partow.net/programming/hashfunctions/
    unsigned int hash = 1315423911;
    for(unsigned int i = 0; i < IDToHash.length(); i++) {
        hash ^= ((hash << 5) + IDToHash[i] + (hash >> 2));
    }
    return hash;
}

std::ostream& operator<< (std::ostream& ostr, const Frame& f) {
    // Ostream operator overload to print all Particles in a Frame
    for (const_particle_itr itr = f.particles.begin(); itr != f.particles.end(); itr++ ) {
        ostr << itr->second;
    }
    return ostr;
}

std::pair<particle_itr, bool> Frame::addParticle(const Particle &newParticle) {
    return particles.insert(std::make_pair(newParticle.getID(), newParticle));
}

std::pair<double, double> Frame::getGravitationalForceBetween(const Particle& p1, const Particle& p2) {
    // Abstract: Implement Newton's Law of Universal Gravitation to determine force between two Particles.
    //      Newton's Law of Universal Gravitation is: F = G*(m1*m2/r^2)
    // Postcondition: F_vec is the 2D force vector pointing from p1 to p2, and F_vec has been returned. The force vector from 
    //      p2 to p1 will be the same, but the user will have to reverse the signs of the elements in F_vec to obtain it.
    assert( p1 != p2 ); // quick and dirty checking. We shouldn't ever call this on the same particle.
    double G = 6.67259*pow(10, -11); // Constant of Gravitation
    double p1x = p1.getCurrentPos().first;
    double p1y = p1.getCurrentPos().second;
    double p2x = p2.getCurrentPos().first;
    double p2y = p2.getCurrentPos().second;
    double r = pow(pow(p2x-p1x, 2) + pow(p2y-p1y, 2), 0.5); // distance formula
    double F = G*(p1.getMass()*p2.getMass()/pow(r,2)); // Magnitude of Force Vector
    double theta = atan(std::abs(p2y-p1y)/std::abs(p2x-p1x)); // angle of interest, in radians
    double F_x = F*cos(theta);
    double F_y = F*sin(theta);
    // Ensure direction of returned force vector is correct
    if ( p2y > p1y && p2x > p1x ) return std::make_pair(F_x, F_y);
    else if ( p2y < p1y && p2x > p1x ) return std::make_pair(F_x, -F_y);
    else if ( p2y < p1y && p2x < p1x ) return std::make_pair(-F_x, -F_y);
    else return std::make_pair(-F_x, F_y);
}

void Frame::updateAllForces() {
    // Abstract: Calculate and store the net force incident on each Particle in the Frame.
    // Postcondition: For every Particle in Frame.particles, Particle.net_force is accurate for the system
    //      conditions at the current timestep.

    // State 1 (forces reset): The net force on each Particle has been set to zero.
    for (particle_itr i = particles.begin(); i != particles.end(); i++ ) {
        i->second.setForce(std::make_pair(0,0));
    }

    // State 2 (new forces calculated): The net force on a particle has been calculated for the current timestep.
    // We individually calculate the force between each pair of particles, checking each unique
    // pair only once so as to avoid double-counting.
    for (particle_itr i = particles.begin(); i != particles.end(); i++ ) {
        for (particle_itr j = i; j != particles.end(); j++ ) {
            if (j == i) continue;
            std::pair<double, double> F_i_j = getGravitationalForceBetween(i->second, j->second);
            i->second.addForce(F_i_j);
            j->second.addForce(-F_i_j);
        }
    }
}

void Frame::advanceSingleTimeStep() {
    // Abstract: Perform all required calculations to move forward by one time step.
    // Postcondition: Frame.time has been incremented by dt, and the positions and velocities of all Particles
    //      have been updated to reflect their new state within the Frame.

    // State 1 (forces current): The net force of each Particle has been updated for the current Frame state.
    this->updateAllForces();

    // State 2 (positions/velocities updated): The position and velocity of each Particle has been updated to
    // reflect that Particle's state at time = time + dt. This is accomplished via numerical integration.
    // Generally, with numerical integration, we aim to solve problems of the form dy/dt = f(t, y), where some
    // conditions are known (either Initial Values, or Boundary Values) and the function f() is known. In our case,
    // the function f() is not actually known, since we are solving for the Force values at every step rather than
    // having them all ahead of time. Thus, we are limited to numerical integration methods which depend only on 
    // the value of f() at the current time step. This precludes the possibility of using RK4, unfortunately.
    // We will use the Left Box rule, which means we accept an error term in our calculations which is O(k^2), where
    // k is our timestep size. The two systems of equations we aim to solve are:
    //      1x) dx/dt = V_x         1y) dy/dt = V_y
    //      2x) dV_x/dt = F_x/m     2y) dV_y/dt = F_y/m

    for (particle_itr itr = particles.begin(); itr != particles.end(); itr++ ) {
        // Left Box Rule: V_next = V_current + dt*f(t)
        // f(t) = F/m
        // Equations 2x and 2y solved
        std::pair<double, double> V_next = std::make_pair(0, 0);
        V_next.first = itr->second.getCurrentVel().first + (this->dt)*(itr->second.getNetForce().first)/(itr->second.getMass());
        V_next.second = itr->second.getCurrentVel().second + (this->dt)*(itr->second.getNetForce().second)/(itr->second.getMass());
        itr->second.addVel(V_next);

        // P_next = P_current + dt*f(t)
        // f(t) = V
        // Equations 1x and 1y solved
        std::pair<double, double> P_next = std::make_pair(0, 0);
        P_next.first = itr->second.getCurrentPos().first + (this->dt)*(itr->second.getCurrentVel().first);
        P_next.second = itr->second.getCurrentPos().second + (this->dt)*(itr->second.getCurrentVel().second);
        itr->second.addPos(P_next);
    }

    // State 3 (time incremented): The frame time has been increased by the amount of the time step.
    time = time + dt;
}

void Frame::saveAllParticleDataToTextFiles() const {
    // Save data for each particle in the Frame to a text file with that particle's name
    for (const_particle_itr itr = particles.begin(); itr != particles.end(); itr++) {
        itr->second.saveDataToTextFile(dt);
    }
}

std::pair<double, double> operator- (const std::pair<double, double>& F_vec) {
    // Overload of - operator, used to easily negate both signs in a force vector, such as the one
    // returned by Frame::getGravitationalForceBetween().
    return std::make_pair(-F_vec.first, -F_vec.second);
}