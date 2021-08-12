#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath> // for pow() and trig functions
#include <cassert>

// ==========================================================================================
// Class representing a single point with some mass, position, and velocity.
// Each particle should have a unique string identifier.
class Particle {
public:
    // Constructor
    // Default constructor should never be called. So, use "__NULL__" to mark mistakenly constructed Particles.
    // Additionally, default copy constructor is fine.
    Particle() : ID("__NULL__") {};
    Particle(std::string _ID, double _mass, std::pair<double, double> _pos, std::pair<double, double> _vel);
    Particle(std::string _ID, double _mass, std::pair<double, double> _pos, std::pair<double, double> _vel, double _radius);

    // Getters
    const std::string&        getID() const { return ID; }
    double                    getMass() const { return mass; }
    std::pair<double, double> getCurrentPos() const { return pos.back(); }
    std::pair<double, double> getCurrentVel() const { return vel.back(); }
    std::pair<double, double> getNetForce() const { return net_force; }

    // Setters
    // No setters for ID and mass, since those should stay constant
    // No setters for pos and vel since they are growing incrementally by adding entries
    void setForce(std::pair<double, double> F_vec) { net_force = F_vec; }

    // Operators
    bool operator== (const Particle& other) const;
    bool operator!= (const Particle& other) const;
    friend std::ostream& operator<< (std::ostream& ostr, const Particle& p);
    
    // Member functions
    void addPos(std::pair<double, double> newPos) { pos.push_back(newPos); }
    void addVel(std::pair<double, double> newVel) { vel.push_back(newVel); }
    void addForce(std::pair<double, double> F_vec);
    void saveDataToTextFile(double dt) const;

private:
    // We store vectors for pos and vel so we have access to a Particle's entire history/path
    std::string ID;
    double mass;                                // in kg
    double radius;                              // optional, to specify a planet's surface for launching off it, in meters
    std::vector<std::pair<double, double>> pos; // in meters from some as of now arbitrary location
    std::vector<std::pair<double, double>> vel; // in meters per second
    std::pair<double, double> net_force;        // in Newtons
};

// ==========================================================================================
// Functor class to be used as hash function with Frame::particle hash table
class ParticleHasher {
public:
    unsigned int operator() (const std::string& IDToHash) const;
};

// ==========================================================================================
// Class representing the collection of particles in 2D space. It tracks how that space changes with time due to 
// forces between each particle. Should be able to impose boundaries on space somehow.

// Useful typedefs
typedef std::unordered_map<std::string, Particle, ParticleHasher>::iterator particle_itr;
typedef std::unordered_map<std::string, Particle, ParticleHasher>::const_iterator const_particle_itr;

class Frame {
public:
    // Constructor
    Frame() : time(0), dt(1) {}
    Frame(double _dt) : time(0), dt(_dt) {}

    // Getters
    double getTime() const { return time; }
    double getDt() const { return dt; }

    // No Setters since time and timestep remain constant.

    // Operators
    Particle operator[] (const std::string& particleName ) { return particles[particleName]; }
    friend std::ostream& operator<< (std::ostream& ostr, const Frame& f);

    // Member Functions
    std::pair<particle_itr, bool> addParticle(const Particle& newParticle);
    std::pair<double, double> getGravitationalForceBetween(const Particle& p1, const Particle& p2);
    void updateAllForces();
    void advanceSingleTimeStep();
    void saveAllParticleDataToTextFiles() const;

private:
    double time;
    const double dt;
    std::unordered_map<std::string, Particle, ParticleHasher> particles; // hashtable between a Particle's ID and that Particle.
};

// ==========================================================================================
// Helper Functions

std::pair<double, double> operator- (const std::pair<double, double>& F_vec);