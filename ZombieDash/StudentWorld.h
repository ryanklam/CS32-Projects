#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <list>
#include <sstream>
#include <iomanip>  // defines the manipulator setw
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(string assetPath);
        ~StudentWorld();
        virtual int init();
        virtual int move();
        virtual void cleanUp();
        void level();
        bool blocked(double playerX, double playerY, Actor* a);
        bool overlaps(double x, double y);
        // For each actor overlapping a, activate a if appropriate.
        void activateOnAppropriateActor(Actor* a);
        void levelFinished();
        void decCitizen() { numCitizens--; }
        bool noMoreCitizens() { return numCitizens <= 0; }
        void addFuel() { m_penelope->addFuel(); }
        void addVaccine() { m_penelope->addVaccine(); }
        void addMine() { m_penelope->addMine(); }
        void createLandmine();
        void createVaccine(double x, double y);
        void createPit(double x, double y);
        void createFlame(double x, double y, Direction dir);
        void createVomit(double x, double y);
        void createZombie(double x, double y);
        bool isFlameBlockedAt(double x, double y);
        void useFlamethrower(Direction dir);
        bool locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distP, double& distZ, bool& isThreat);
        bool locateNearestCitizenThreat(double x, double y, double& distZ);
        bool locateNearestVomitTrigger(double& targetX, double& targetY, Actor* a);
        Actor* locateNearestTarget(int dist, Actor* a);
        string getString(int num)
        {
            ostringstream oss;
            oss << num;
            return oss.str();
        }
    private:
        Penelope* m_penelope;
        list<Actor*> actors;
        int numCitizens;
        int tickStatus;
};

#endif // STUDENTWORLD_H_
