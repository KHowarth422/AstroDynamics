#include "modelClasses.h"

int main(int argc, char* argv[]) {
    // TODO: Add better input parsing
    // I should be able to toggle between different models by passing in an input flag

    bool debug = false;
    bool earth = false;
    bool three_body = false;
    bool solar = false;
    if (argc == 1) {
        std::cout << "Not enough runtime arguments!" << std::endl;
        return 0;
    } else if (argc == 2) {
        if (!strcmp(argv[1], "-debug")) {
            // -debug flag toggles all the debugging print statements
            debug = true;
        } else if (!strcmp(argv[1], "-earth")) {
            earth = true;
        } else if (!strcmp(argv[1], "-three_body")) {
            three_body = true;
        } else if (!strcmp(argv[1], "-solar")) {
            solar = true;
        } else {
            std::cout << "Incorrect runtime argument: " << argv[1] << std::endl;   
            return 0;        
        }
    } else {
        std::cout << "Too many runtime arguments!" << std::endl;
        return 0;
    }

    if (debug) {
        // Debugging workflow for Particle and Frame classes and methods

        Particle p1 = Particle("Ship", 10000, std::make_pair(1.25, 2.5), std::make_pair(0, 0));
        Particle p2 = Particle("Sun", 1.989*pow(10,30), std::make_pair(0, 0), std::make_pair(0, 0));
        Particle p3 = Particle("Planet 2", 5000000, std::make_pair(15, 25), std::make_pair(0, 2));
        Particle p4 = Particle("Planet 3", 15000000, std::make_pair(100, 20), std::make_pair(0, 0));

        // Test Particle::operator==
        Particle p1eq = Particle("Ship", 5, std::make_pair(0.25, 3.5), std::make_pair(0, -5));
        assert (p1 == p1eq);

        // Test insertion into Frame
        Frame F = Frame(1);
        std::pair<std::unordered_map<std::string, Particle, ParticleHasher>::iterator, bool> res = F.addParticle(p1);
        std::pair<std::unordered_map<std::string, Particle, ParticleHasher>::iterator, bool> res2 = F.addParticle(p2);
        std::pair<std::unordered_map<std::string, Particle, ParticleHasher>::iterator, bool> res3 = F.addParticle(p3);
        std::pair<std::unordered_map<std::string, Particle, ParticleHasher>::iterator, bool> res4 = F.addParticle(p4);

        assert (res.second);
        assert (res2.second);
        assert (res3.second);
        assert (res4.second);

        // Use iterator
        assert ( (res.first)->first == "Ship" );
        assert ( (res.first)->second.getID() == "Ship" );

        // Check Frame::operator[]
        assert ( F["Ship"] == p1 );
        assert ( F["Sun"] == p2 );
        assert ( F["Planet 2"] == p3 );
        assert ( F["Planet 3"] == p4 );

        // Check adding positions/velocities/forces
        p1.addPos(std::make_pair(1.5, 3));
        p1.addVel(std::make_pair(13, 1));
        p1.addForce(std::make_pair(5, 6));
        p1.addForce(std::make_pair(-2, 2));

        // Check Particle::operator<<
        // Expected printout:
        /* 
         * Particle ID: Ship
         * Mass: 10000
         * Position: [(1.25, 2.5) ...]
         * Velocity: [(0, 0) ...]
         * Force Vector: (3, 8)
         */
        std::cout << "Testing Particle::operator<<" << std::endl;
        std::cout << p1 << std::endl;

        // Check Frame::getGravitationalForceBetween() and operator- for pair<double, double>
        // From manual calculations, F_vec = (2.26519 * 10^21, 3.39778 * 10^21)
        p1.addPos(std::make_pair(10, 15));
        std::pair<double, double> F_p1_p2 = F.getGravitationalForceBetween(p1, p2);
        std::pair<double, double> F_p2_p1 = -F_p1_p2;
        assert (F_p1_p2.first == -F_p2_p1.first);
        assert (F_p1_p2.second == -F_p2_p1.second);
        assert (std::abs(std::abs(F_p1_p2.first) - 2.26519*pow(10, 21))/(2.26519*pow(10, 21)) < 0.001 );
        assert (std::abs(std::abs(F_p1_p2.second) - 3.39778*pow(10, 21))/(3.39778*pow(10, 21)) < 0.001 );
        
        // Check Frame::updateAllForces() and Frame::operator<<
        F.updateAllForces();
        std::cout << "All particles after Frame::updateAllForces():" << std::endl;
        std::cout << F << std::endl;

        // Debugging force at different positions
        // t2x > t1x
        // t2y > t1y
        Particle t1 = Particle("t1", 5*pow(10,0), std::make_pair(0, 0), std::make_pair(0,0));
        Particle t2 = Particle("t2", 5*pow(10,10), std::make_pair(10, 10), std::make_pair(0,0));
        std::pair<double, double> T1 = F.getGravitationalForceBetween(t1, t2);
        std::cout << "t2x > t1x, t2y > t1y: (" << T1.first << ", " << T1.second << ")" << std::endl;

        // t2x < t1x
        // t2y > t1y
        Particle t3 = Particle("t3", 5*pow(10,0), std::make_pair(0, 0), std::make_pair(0,0));
        Particle t4 = Particle("t4", 5*pow(10,10), std::make_pair(-10, 10), std::make_pair(0,0));
        std::pair<double, double> T2 = F.getGravitationalForceBetween(t3, t4);
        std::cout << "t2x < t1x, t2y > t1y: (" << T2.first << ", " << T2.second << ")" << std::endl;

        // t2x < t1x
        // t2y < t1y
        std::pair<double, double> T3 = F.getGravitationalForceBetween(t2, t1);
        std::cout << "t2x < t1x, t2y < t1y: (" << T3.first << ", " << T3.second << ")" << std::endl;

        // t2x > t1x
        // t2y < t1y
        std::pair<double, double> T4 = F.getGravitationalForceBetween(t4, t3);
        std::cout << "t2x > t1x, t2y < t1y: (" << T4.first << ", " << T4.second << ")" << std::endl;
    }

    // ========================================================================
    // Actual Modeling
    // ========================================================================

    if (earth) {
        // Model the earth orbiting around the sun
        Frame F2 = Frame(1);
        Particle p5 = Particle("Earth", 5.972*pow(10,24), std::make_pair(0, 149600000000), std::make_pair(29780, 0));
        Particle p6 = Particle("Sun", 1.989*pow(10,30), std::make_pair(0, 0), std::make_pair(0, 0));
        F2.addParticle(p5);
        F2.addParticle(p6);

        // Number of seconds in a year
        double secs = 3.154*pow(10,7);
        // Number of steps to achieve that
        double steps = secs/1;

        // One year period
        for (int i = 0; i < steps; i++) {
            F2.advanceSingleTimeStep();
        }

        if (debug) {
            std::cout << "\nAll particles after " << steps << " time steps:" << std::endl;
            std::cout << F2 << std::endl;
        }
        F2.saveAllParticleDataToTextFiles();
        std::cout << "Earth and Sun data saved to text file.\n";
    }

    if (three_body) {
        // Model a three body problem
        Frame F3(100);
        Particle three_body_1("Sun_1", 1.989*pow(10,30), std::make_pair(0, 0), std::make_pair(0, -20000));
        Particle three_body_2("Sun_2", 1.989*pow(10,30), std::make_pair(149600000000*1.5, 0), std::make_pair(-5000, 20000));
        Particle three_body_3("Planet", 1.989*pow(10,22), std::make_pair(149600000000*0.5, 149600000000*0.5), std::make_pair(30000, -30000));
        Particle three_body_4("Planet_2", 1.989*pow(10,22), std::make_pair(149600000000*0.55, 149600000000*0.5), std::make_pair(0, 25000));
        F3.addParticle(three_body_1);
        F3.addParticle(three_body_2);
        F3.addParticle(three_body_3);
        F3.addParticle(three_body_4);

        for (int i = 0; i < 3.154*pow(10,7)*0.5; i++) {
            F3.advanceSingleTimeStep();
        }

        F3.saveAllParticleDataToTextFiles();
        std::cout << "Three Body Problem data saved to text file.\n";
    }

    if (solar) {
        // Model the 8 planets of the solar system (sorry pluto)
        // Quick and dirty, using avg values. Source: https://nssdc.gsfc.nasa.gov/planetary/factsheet/
        // TODO: Understand the orbital parameters and create more accurate simulation
        Frame F4(10);
        Particle solar_0("Sun", 1.989*pow(10,30), std::make_pair(0, 0), std::make_pair(0, 0));
        Particle solar_1("Mercury", 3.285*pow(10,23), std::make_pair(57904197000, 0), std::make_pair(0, 47360));
        Particle solar_2("Venus", 4.8675*pow(10,24), std::make_pair(108683828350, 0), std::make_pair(0, 35020));
        Particle solar_3("Earth", 5.9724*pow(10,24), std::make_pair(151625954300, 0), std::make_pair(0, 29780));
        Particle solar_4("Mars", 0.64171*pow(10,24), std::make_pair(-207930000000, 0), std::make_pair(0, -24070));
        Particle solar_5("Jupiter", 1898.19*pow(10,24), std::make_pair(770300000000, 0), std::make_pair(0, 13060));
        Particle solar_6("Saturn", 568.34*pow(10,24), std::make_pair(1494400000000, 0), std::make_pair(0, 9680));
        Particle solar_7("Uranus", 86.813*pow(10,24), std::make_pair(2960500000000, 0), std::make_pair(0, 6800));
        Particle solar_8("Neptune", 102.413*pow(10,24), std::make_pair(4476400000000, 0), std::make_pair(0, 5430));
        F4.addParticle(solar_0);
        F4.addParticle(solar_1);
        F4.addParticle(solar_2);
        F4.addParticle(solar_3);
        F4.addParticle(solar_4);
        F4.addParticle(solar_5);
        F4.addParticle(solar_6);
        F4.addParticle(solar_7);
        F4.addParticle(solar_8);

        // Simulate for ten earth orbits
        for (int i = 0; i < 3.154*pow(10,7); i++) {
            F4.advanceSingleTimeStep();
        }

        F4.saveAllParticleDataToTextFiles();
        std::cout << "Solar System data saved to text file.\n";
    }
}