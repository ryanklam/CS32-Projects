#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
    public:
        Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
        ~Actor(){}
        virtual void doSomething() = 0;
        virtual bool canExit() { return false; }
        bool isDead() { return dead; }
    
        void setDead() { dead = true; }
    
        virtual void getInfected() {};
    
        StudentWorld* getStudentWorld() { return studentworld; }
    
        virtual bool canGetGoodies() { return false; }
    
        virtual bool canBeActivated() { return false; }
    
        virtual bool canInfect() { return false; }
    
        virtual bool blocksMovement() { return false; };
    
        // If this is an activated object, perform its effect on a (e.g., for an
        // Exit have a use the exit).
        virtual void activateIfAppropriate(Actor* a);
    
        // If this object can die by falling into a pit or burning, die.
        virtual bool canDieFromFlame() { return true; }
        
        virtual bool canDieFromPit() { return false; }
    
        // Does this object block flames?
        virtual bool blocksFlame() { return false; }
    
        virtual bool canBeInfected() { return false; }
    
        virtual bool canExplode() { return false; }
        virtual void explode() {}
    
        virtual void useExitIfAppropriate() {};
        virtual void dieFromFlameIfAppropriate() {};
        virtual void dieFromPitIfAppropriate() {};
    
        virtual bool isInfected() { return false; };

    private:
        bool dead = false;
        StudentWorld* studentworld;
};

class Wall: public Actor
{
    public:
        Wall(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual bool blocksMovement() { return true; }
        virtual bool blocksFlame() { return true; }
        virtual bool canDieFromFlame() { return false; }

};


class ActivatingObject: public Actor
{
    public:
        ActivatingObject(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
        virtual void doSomething() = 0;
        virtual bool canBeActivated() { return true; }
        virtual bool canExit() { return false; }

};


class Exit: public ActivatingObject
{
    public:
        Exit(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual void activateIfAppropriate(Actor* a);
        virtual bool blocksFlame() { return true; }
        virtual bool canDieFromFlame() { return false; }
};

class Pit: public ActivatingObject
{
    public:
        Pit(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual void activateIfAppropriate(Actor* a);
        virtual bool canDieFromFlame() { return false; }
};

class Flame: public ActivatingObject
{
    public:
        Flame(int imageID, double startX, double startY, Direction dir, StudentWorld* world);
        virtual void doSomething();
        virtual void activateIfAppropriate(Actor* a);
        virtual bool canDieFromFlame() { return false; }
        int getLifeTicks() { return lifeTicks; }
        void decLifeTicks() { lifeTicks--; }
    
    private:
        int lifeTicks = 2;
};

class Vomit: public ActivatingObject
{
    public:
        Vomit(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual void activateIfAppropriate(Actor* a);
        virtual bool canDieFromFlame() { return false; }
        int getLifeTicks() { return lifeTicks; }
        void decLifeTicks() { lifeTicks--; }
    
    private:
        int lifeTicks = 2;
};

class Landmine: public ActivatingObject
{
    public:
        Landmine(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual void activateIfAppropriate(Actor* a);
        virtual bool canExplode() { return true; }
        bool isActive() { return active; }
        void decreaseSafety() { safetyTicks--; }
        int getSafetyTicks() { return safetyTicks; }
        void setActive() { active = true; }
        virtual void explode();
        virtual void dieFromFlameIfAppropriate();
    
    private:
        int safetyTicks = 30;
        bool active = false;
};

class Goodie: public ActivatingObject
{
    public:
        Goodie(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual void dieFromFlameIfAppropriate();
        void pickUp(Actor* a);
};

class GasCanGoodie: public Goodie
{
    public:
        GasCanGoodie(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual void activateIfAppropriate(Actor* a);
};

class VaccineGoodie: public Goodie
{
    public:
        VaccineGoodie(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual void activateIfAppropriate(Actor* a);
};

class LandmineGoodie: public Goodie
{
    public:
        LandmineGoodie(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual void activateIfAppropriate(Actor* a);
};

class Agent: public Actor
{
    public:
        Agent(int imageID, double startX, double startY, StudentWorld* world);
        virtual bool ableToMove(Direction dir) { return false; }
        virtual bool blocksMovement() { return true; }
        virtual bool canDieFromPit() { return true; }
        virtual bool canExit() { return false; }

};

class Human: public Agent
{
    public:
        Human(int imageID, double startX, double startY, StudentWorld* world);
        int getInfectionCount() { return infectionCount; }
        virtual bool isInfected() { return infected; }
        void increaseInfection() { infectionCount++; }
        virtual void getInfected() { infected = true; }
        void clearInfection() { infected = false; infectionCount = 0; }
        virtual bool canBeInfected() { return true; }
    
    private:
        bool infected = false;
        int infectionCount = 0;
};

class Penelope: public Human
{
    public:
        Penelope(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual bool ableToMove(Direction dir);
        virtual bool canExit();
        void addFuel() { fuelCount += 5; }
        void decFuel() { fuelCount--; }
        int getFuelCount() { return fuelCount; }
        void addVaccine() { vaccineCount++; }
        int getVaccineCount() { return vaccineCount; }
        void addMine() { mineCount += 2; }
        void decMine() { mineCount--; }
        void decVaccine() { vaccineCount--; }
        int getMineCount() { return mineCount; }
        virtual bool canGetGoodies() { return true; }
        virtual void useExitIfAppropriate();
        virtual void dieFromFlameIfAppropriate();
        virtual void dieFromPitIfAppropriate() { dieFromFlameIfAppropriate(); }

    private:
        int vaccineCount;
        int mineCount;
        int fuelCount;
};

class Citizen: public Human
{
    public:
        Citizen(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual bool canExit() { return true; }
        int getParalyze() { return paralyzeTick; }
        void incrementParalyze() { paralyzeTick++; }
        virtual void useExitIfAppropriate();
        virtual void dieFromFlameIfAppropriate();
        virtual void dieFromPitIfAppropriate() { dieFromFlameIfAppropriate(); }
    
    private:
        int paralyzeTick = 0;
};

class Zombie: public Agent
{
    public:
        Zombie(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual bool canInfect() { return true; }
        int getParalyze() { return paralyzeTick; }
        void incrementParalyze() { paralyzeTick++; }
        int getMovementPlan() { return movementPlan; }
        void setMovementPlan(int mv) { movementPlan = mv; }
        void decrementMovementPlan() { movementPlan--; }

    private:
        int paralyzeTick = 0;
        int movementPlan = 0;

};

class DumbZombie: public Zombie
{
    public:
        DumbZombie(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual void dieFromFlameIfAppropriate();
        virtual void dieFromPitIfAppropriate();
};

class SmartZombie: public Zombie
{
    public:
        SmartZombie(int imageID, double startX, double startY, StudentWorld* world);
        virtual void doSomething();
        virtual void dieFromFlameIfAppropriate();
        virtual void dieFromPitIfAppropriate() { dieFromFlameIfAppropriate(); }

};

#endif // ACTOR_H_
