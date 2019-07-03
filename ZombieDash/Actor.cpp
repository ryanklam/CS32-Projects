#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world)
: GraphObject(imageID, startX, startY, dir, depth, 1.0)
{
    studentworld = world;
}

void Actor::activateIfAppropriate(Actor* a)
{}

ActivatingObject::ActivatingObject(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world)
: Actor(imageID, startX, startY, dir, depth, world)
{}


Wall::Wall(int imageID, double startX, double startY, StudentWorld* world)
: Actor(imageID, startX, startY, right, 0, world)
{}

void Wall::doSomething()
{}

Exit::Exit(int imageID, double startX, double startY, StudentWorld* world)
: ActivatingObject(imageID, startX, startY, right, 1, world)
{}

void Exit::activateIfAppropriate(Actor *a)
{
    if(a->blocksMovement())
        a->useExitIfAppropriate();
}

void Exit::doSomething()
{
    getStudentWorld()->activateOnAppropriateActor(this);
}


Pit::Pit(int imageID, double startX, double startY, StudentWorld* world)
: ActivatingObject(imageID, startX, startY, right, 0, world)
{}

void Pit::doSomething()
{
    getStudentWorld()->activateOnAppropriateActor(this);
}

void Pit::activateIfAppropriate(Actor* a)
{
    if(a->canDieFromPit())
        a->dieFromPitIfAppropriate();
}

Flame::Flame(int imageID, double startX, double startY, Direction dir, StudentWorld* world)
: ActivatingObject(imageID, startX, startY, dir, 0, world)
{}

void Flame::doSomething()
{
    if(isDead())
        return;
    getStudentWorld()->activateOnAppropriateActor(this);
    decLifeTicks();
    if(getLifeTicks() == 0)
        setDead();
}

void Flame::activateIfAppropriate(Actor * a)
{
    if(!a->isDead())
        a->dieFromFlameIfAppropriate();
}

Vomit::Vomit(int imageID, double startX, double startY, StudentWorld* world)
: ActivatingObject(imageID, startX, startY, right, 0, world)
{}

void Vomit::doSomething()
{
    if(isDead())
        return;
    getStudentWorld()->activateOnAppropriateActor(this);
    decLifeTicks();
    if(getLifeTicks() == 0)
        setDead();
}

void Vomit::activateIfAppropriate(Actor* a)
{
    if(a->canBeInfected() && !a->isDead())
    {
        if(!a->canGetGoodies() && !a->isInfected())
            getStudentWorld()->playSound(SOUND_CITIZEN_INFECTED);
        a->getInfected();
    }
}

Landmine::Landmine(int imageID, double startX, double startY, StudentWorld* world)
: ActivatingObject(imageID, startX, startY, right, 1, world)
{}

void Landmine::doSomething()
{
    if(isDead())
        return;
    if(!isActive())
        decreaseSafety();
    if(getSafetyTicks() == 0)
    {
        setActive();
        getStudentWorld()->activateOnAppropriateActor(this);
        return;
    }
}

void Landmine::activateIfAppropriate(Actor* a)
{
    if(!a->canExplode())
    {
        explode();
        setDead();
    }
}

void Landmine::explode()
{
    getStudentWorld()->playSound(SOUND_LANDMINE_EXPLODE);
    getStudentWorld()->createFlame(getX(), getY(), up);
    
    getStudentWorld()->createFlame(getX() - SPRITE_WIDTH, getY(), up);
    getStudentWorld()->createFlame(getX() + SPRITE_WIDTH, getY(), up);
    getStudentWorld()->createFlame(getX(), getY() - SPRITE_HEIGHT, up);
    getStudentWorld()->createFlame(getX(), getY() + SPRITE_HEIGHT, up);
    getStudentWorld()->createFlame(getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up);
    getStudentWorld()->createFlame(getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up);
    getStudentWorld()->createFlame(getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up);
    getStudentWorld()->createFlame(getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up);
    getStudentWorld()->createPit(getX(), getY());
}

void Landmine::dieFromFlameIfAppropriate()
{
    if(!isDead())
    {
        explode();
        setDead();
    }
}

Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* world)
: ActivatingObject(imageID, startX, startY, right, 1, world)
{}

void Goodie::doSomething()
{}

void Goodie::dieFromFlameIfAppropriate()
{
    if(!isDead())
    {
        setDead();
    }
}

void Goodie::pickUp(Actor* a)
{
    getStudentWorld()->playSound(SOUND_GOT_GOODIE);
    getStudentWorld()->increaseScore(50);
    a->setDead();
}

GasCanGoodie::GasCanGoodie(int imageID, double startX, double startY, StudentWorld* world)
: Goodie(imageID, startX, startY, world)
{}

void GasCanGoodie::doSomething()
{
    if(isDead())
        return;
    getStudentWorld()->activateOnAppropriateActor(this);
}

void GasCanGoodie::activateIfAppropriate(Actor* a)
{
    if(a->canGetGoodies())
    {
        pickUp(this);
        getStudentWorld()->addFuel();
    }
}

VaccineGoodie::VaccineGoodie(int imageID, double startX, double startY, StudentWorld* world)
: Goodie(imageID, startX, startY, world)
{}

void VaccineGoodie::doSomething()
{
    if(isDead())
        return;
    getStudentWorld()->activateOnAppropriateActor(this);
}

void VaccineGoodie::activateIfAppropriate(Actor* a)
{
    if(a->canGetGoodies())
    {
        pickUp(this);
        getStudentWorld()->addVaccine();
    }
}

LandmineGoodie::LandmineGoodie(int imageID, double startX, double startY, StudentWorld* world)
: Goodie(imageID, startX, startY, world)
{}

void LandmineGoodie::doSomething()
{
    if(isDead())
        return;
    getStudentWorld()->activateOnAppropriateActor(this);
}

void LandmineGoodie::activateIfAppropriate(Actor* a)
{
    if(a->canGetGoodies())
    {
        pickUp(this);
        getStudentWorld()->addMine();
    }
}

Agent::Agent(int imageID, double startX, double startY, StudentWorld* world)
: Actor(imageID, startX, startY, right, 0, world)
{}

Human::Human(int imageID, double startX, double startY, StudentWorld* world)
: Agent(imageID, startX, startY, world)
{}

Penelope::Penelope(int imageID, double startX, double startY, StudentWorld* world)
: Human(imageID, startX, startY, world)
{
    vaccineCount = 0;
    mineCount = 0;
    fuelCount = 0;
}

void Penelope::doSomething()
{
    if(isDead())
        return;
    if(isInfected())
        increaseInfection();
    if(getInfectionCount() == 500)
    {
        setDead();
        getStudentWorld()->playSound(SOUND_PLAYER_DIE);
        getStudentWorld()->decLives();
        return;
    }
    int key;
    getStudentWorld()->getKey(key);
    switch(key)
    {
        case KEY_PRESS_UP:
            setDirection(up);
            if(!getStudentWorld()->blocked(getX(), getY() + 4, this))
                moveTo(getX(), getY() + 4);
            break;
        case KEY_PRESS_DOWN:
            setDirection(down);
            if(!getStudentWorld()->blocked(getX(), getY() - 4, this))
                moveTo(getX(), getY() - 4);
            break;
        case KEY_PRESS_LEFT:
            setDirection(left);
            if(!getStudentWorld()->blocked(getX() - 4, getY(), this))
                moveTo(getX() - 4, getY());
            break;
        case KEY_PRESS_RIGHT:
            setDirection(right);
            if(!getStudentWorld()->blocked(getX() + 4, getY(), this))
                moveTo(getX() + 4, getY());
            break;
        case KEY_PRESS_TAB:
            getStudentWorld()->createLandmine();
            break;
        case KEY_PRESS_SPACE:
            if(getFuelCount() != 0)
            {
                getStudentWorld()->playSound(SOUND_PLAYER_FIRE);
                getStudentWorld()->useFlamethrower(getDirection());
                decFuel();
            }
            break;
        case KEY_PRESS_ENTER:
            if(getVaccineCount() != 0)
            {
                decVaccine();
                clearInfection();
            }
            break;
        default:
            break;
    }
}

bool Penelope::ableToMove(Direction dir)
{
    switch(dir)
    {
        case up:
            if(getY() + 4 > VIEW_HEIGHT)
                return false;
            return true;
            break;
        case down:
            if(getY() - 4 < 0)
                return false;
            return true;
            break;
        case left:
            if(getX() - 4 < 0)
                return false;
            return true;
            break;
        case right:
            if(getX() + 4 > VIEW_WIDTH)
                return false;
            return true;
            break;
        default:
            return true;
            break;
    }
}

bool Penelope::canExit()
{
    return getStudentWorld()->noMoreCitizens();
}

void Penelope::useExitIfAppropriate()
{
    if(getStudentWorld()->noMoreCitizens())
    {
        getStudentWorld()->playSound(SOUND_LEVEL_FINISHED);
        getStudentWorld()->levelFinished();
    }
}

void Penelope::dieFromFlameIfAppropriate()
{
    if(!isDead())
    {
        getStudentWorld()->playSound(SOUND_PLAYER_DIE);
        getStudentWorld()->decLives();
        setDead();
    }
}

Citizen::Citizen(int imageID, double startX, double startY, StudentWorld* world)
: Human(imageID, startX, startY, world)
{}

void Citizen::doSomething()
{
    if(isDead())
        return;
    if(isInfected())
        increaseInfection();
    if(getInfectionCount() == 500)
    {
        setDead();
        getStudentWorld()->decCitizen();
        getStudentWorld()->playSound(SOUND_ZOMBIE_BORN);
        getStudentWorld()->increaseScore(-1000);
        getStudentWorld()->createZombie(getX(), getY());
        return;
    }
    incrementParalyze();
    if(getParalyze() % 2 == 0)
        return;
    double otherX = 0, otherY = 0, dist_p = 100000, dist_z = 100000;
    bool isThreat;
    getStudentWorld()->locateNearestCitizenTrigger(getX(), getY(), otherX, otherY, dist_p, dist_z, isThreat);
    if(!isThreat && dist_p <= 6400)
    {
        if(getX() == otherX || getY() == otherY) // 6A
        {
            double deltaX = otherX - getX(), deltaY = otherY - getY();
            if(deltaX > 0) // RIGHT
            {
                if(!getStudentWorld()->blocked(getX() + 2, getY(), this))
                {
                    setDirection(right);
                    moveTo(getX() + 2, getY());
                    return;
                }
            }
            else if(deltaX < 0) // LEFT
            {
                if(!getStudentWorld()->blocked(getX() - 2, getY(), this))
                {
                    setDirection(left);
                    moveTo(getX() - 2, getY());
                    return;
                }
            }
            else if(deltaY > 0) // UP
            {
                if(!getStudentWorld()->blocked(getX(), getY() + 2, this))
                {
                    setDirection(up);
                    moveTo(getX(), getY() + 2);
                    return;
                }
            }
            else if(deltaY < 0) // DOWN
            {
                if(!getStudentWorld()->blocked(getX(), getY() - 2, this))
                {
                    setDirection(down);
                    moveTo(getX(), getY() - 2);
                    return;
                }
            }
        }
        
        else if(getX() != otherX && getY() != otherY) // 6B
        {
            double deltaX = otherX - getX(), deltaY = otherY - getY();
            if(deltaX > 0) // RIGHT
            {
                if(deltaY > 0) // UP
                {
                    int dir = randInt(1,2); // 1 = right, 2 = up
                    if(dir == 1)
                    {
                        if(!getStudentWorld()->blocked(getX() + 2, getY(), this))
                        {
                            setDirection(right);
                            moveTo(getX() + 2, getY());
                            return;
                        }
                        if(!getStudentWorld()->blocked(getX(), getY() + 2, this))
                        {
                            setDirection(up);
                            moveTo(getX(), getY() + 2);
                            return;
                        }
                    }
                    if(dir == 2)
                    {
                        if(!getStudentWorld()->blocked(getX(), getY() + 2, this))
                        {
                            setDirection(up);
                            moveTo(getX(), getY() + 2);
                            return;
                        }
                        if(!getStudentWorld()->blocked(getX() + 2, getY(), this))
                        {
                            setDirection(right);
                            moveTo(getX() + 2, getY());
                            return;
                        }
                    }
                }
                else if(deltaY < 0) // DOWN
                {
                    int dir = randInt(1,2); // 1 = right, 2 = down
                    if(dir == 1)
                    {
                        if(!getStudentWorld()->blocked(getX() + 2, getY(), this))
                        {
                            setDirection(right);
                            moveTo(getX() + 2, getY());
                            return;
                        }
                        if(!getStudentWorld()->blocked(getX(), getY() - 2, this))
                        {
                            setDirection(up);
                            moveTo(getX(), getY() - 2);
                            return;
                        }
                    }
                    if(dir == 2)
                    {
                        if(!getStudentWorld()->blocked(getX(), getY() - 2, this))
                        {
                            setDirection(up);
                            moveTo(getX(), getY() - 2);
                            return;
                        }
                        if(!getStudentWorld()->blocked(getX() + 2, getY(), this))
                        {
                            setDirection(right);
                            moveTo(getX() + 2, getY());
                            return;
                        }
                    }
                }
            }
            else if(deltaX < 0) // LEFT
            {
                if(deltaY > 0) // UP
                {
                    int dir = randInt(1,2); // 1 = left, 2 = up
                    if(dir == 1)
                    {
                        if(!getStudentWorld()->blocked(getX() - 2, getY(), this))
                        {
                            setDirection(right);
                            moveTo(getX() - 2, getY());
                            return;
                        }
                        if(!getStudentWorld()->blocked(getX(), getY() + 2, this))
                        {
                            setDirection(up);
                            moveTo(getX(), getY() + 2);
                            return;
                        }
                    }
                    if(dir == 2)
                    {
                        if(!getStudentWorld()->blocked(getX(), getY() + 2, this))
                        {
                            setDirection(up);
                            moveTo(getX(), getY() + 2);
                            return;
                        }
                        if(!getStudentWorld()->blocked(getX() - 2, getY(), this))
                        {
                            setDirection(right);
                            moveTo(getX() - 2, getY());
                            return;
                        }
                    }
                }
                else if(deltaY < 0) // DOWN
                {
                    int dir = randInt(1,2); // 1 = left, 2 = down
                    if(dir == 1)
                    {
                        if(!getStudentWorld()->blocked(getX() - 2, getY(), this))
                        {
                            setDirection(right);
                            moveTo(getX() - 2, getY());
                            return;
                        }
                        if(!getStudentWorld()->blocked(getX(), getY() - 2, this))
                        {
                            setDirection(up);
                            moveTo(getX(), getY() - 2);
                            return;
                        }
                    }
                    if(dir == 2)
                    {
                        if(!getStudentWorld()->blocked(getX(), getY() - 2, this))
                        {
                            setDirection(up);
                            moveTo(getX(), getY() - 2);
                            return;
                        }
                        if(!getStudentWorld()->blocked(getX() - 2, getY(), this))
                        {
                            setDirection(right);
                            moveTo(getX() - 2, getY());
                            return;
                        }
                    }
                }
            }
        }
    }
    else if(isThreat && dist_z <= 6400)
    {
        double upD = 0, downD = 0, leftD = 0, rightD = 0;
        if(!getStudentWorld()->blocked(getX(), getY() + 2, this)) // UP
            getStudentWorld()->locateNearestCitizenThreat(getX(), getY() + 2, upD);
        if(!getStudentWorld()->blocked(getX(), getY() - 2, this)) // DOWN
            getStudentWorld()->locateNearestCitizenThreat(getX(), getY() - 2, downD);
        if(!getStudentWorld()->blocked(getX() - 2, getY(), this)) // LEFT
            getStudentWorld()->locateNearestCitizenThreat(getX() - 2, getY(), leftD);
        if(!getStudentWorld()->blocked(getX() + 2, getY(), this)) // RIGHT
            getStudentWorld()->locateNearestCitizenThreat(getX() + 2, getY(), rightD);
        if(dist_z > upD && dist_z > downD && dist_z > leftD && dist_z > rightD)
            return;
        double maxD[4] = {upD, downD, leftD, rightD};
        double max = maxD[0];
        for(int i = 0; i < 4; i++)
        {
            if(maxD[i] > max)
                max = maxD[i];
        }
        if(max == upD)
        {
            setDirection(up);
            moveTo(getX(), getY() + 2);
            return;
        }
        else if(max == downD)
        {
            setDirection(down);
            moveTo(getX(), getY() - 2);
            return;
        }
        else if(max == leftD)
        {
            setDirection(left);
            moveTo(getX() - 2, getY());
            return;
        }
        else if(max == rightD)
        {
            setDirection(right);
            moveTo(getX() + 2, getY());
            return;
        }
    }
}

void Citizen::useExitIfAppropriate()
{
    getStudentWorld()->playSound(SOUND_CITIZEN_SAVED);
    getStudentWorld()->increaseScore(500);
    getStudentWorld()->decCitizen();
    setDead();
}

void Citizen::dieFromFlameIfAppropriate()
{
    if(!isDead())
    {
        getStudentWorld()->playSound(SOUND_CITIZEN_DIE);
        getStudentWorld()->increaseScore(-1000);
        getStudentWorld()->decCitizen();
        setDead();
    }
}

Zombie::Zombie(int imageID, double startX, double startY, StudentWorld* world)
: Agent(imageID, startX, startY, world)
{}

void Zombie::doSomething()
{}

DumbZombie::DumbZombie(int imageID, double startX, double startY, StudentWorld* world)
: Zombie(imageID, startX, startY, world)
{}

void DumbZombie::doSomething()
{
    if(isDead())
        return;
    incrementParalyze();
    if(getParalyze() % 2 == 0)
        return;
    double targetX = 0, targetY = 0;
    if(getStudentWorld()->locateNearestVomitTrigger(targetX, targetY, this))
    {
        int rand = randInt(1, 3);
        if(rand < 2)
        {
            getStudentWorld()->createVomit(targetX, targetY);
            getStudentWorld()->playSound(SOUND_ZOMBIE_VOMIT);
            return;
        }
    }
    if(getMovementPlan() == 0)
    {
        setMovementPlan(randInt(3, 10));
        int rand = randInt(1, 4);
        switch(rand)
        {
            case 1:
                setDirection(up);
                break;
            case 2:
                setDirection(down);
                break;
            case 3:
                setDirection(left);
                break;
            case 4:
                setDirection(right);
                break;
        }
    }
    double dest_x = 0, dest_y = 0;
    Direction dir = getDirection();
    switch(dir)
    {
        case up:
            dest_x = getX();
            dest_y = getY() + 1;
            if(!getStudentWorld()->blocked(dest_x, dest_y, this))
            {
                moveTo(dest_x, dest_y);
                decrementMovementPlan();
                return;
            }
            break;
        case down:
            dest_x = getX();
            dest_y = getY() - 1;
            if(!getStudentWorld()->blocked(dest_x, dest_y, this))
            {
                moveTo(dest_x, dest_y);
                decrementMovementPlan();
                return;
            }
            break;
        case left:
            dest_x = getX() - 1;
            dest_y = getY();
            if(!getStudentWorld()->blocked(dest_x, dest_y, this))
            {
                moveTo(dest_x, dest_y);
                decrementMovementPlan();
                return;
            }
            break;
        case right:
            dest_x = getX() + 1;
            dest_y = getY();
            if(!getStudentWorld()->blocked(dest_x, dest_y, this))
            {
                moveTo(dest_x, dest_y);
                decrementMovementPlan();
                return;
            }
            break;
    }
    setMovementPlan(0);
}

void DumbZombie::dieFromFlameIfAppropriate()
{
    if(!isDead())
    {
        int rand = randInt(1, 10);
        if(rand == 1)
        {
            int rand = randInt(1, 4);
            switch(rand)
            {
                case 1: // up
                    if(!getStudentWorld()->overlaps(getX(), getY() + SPRITE_HEIGHT))
                        getStudentWorld()->createVaccine(getX(), getY() + SPRITE_HEIGHT);
                    break;
                case 2: // down
                    if(!getStudentWorld()->overlaps(getX(), getY() - SPRITE_HEIGHT))
                        getStudentWorld()->createVaccine(getX(), getY() - SPRITE_HEIGHT);
                    break;
                case 3: // left
                    if(!getStudentWorld()->overlaps(getX() - SPRITE_WIDTH, getY()))
                        getStudentWorld()->createVaccine(getX() - SPRITE_WIDTH, getY());
                    break;
                case 4: // right
                    if(!getStudentWorld()->overlaps(getX() + SPRITE_WIDTH, getY()))
                        getStudentWorld()->createVaccine(getX() + SPRITE_WIDTH, getY());
                    break;
                    
            }
        }
        getStudentWorld()->playSound(SOUND_ZOMBIE_DIE);
        getStudentWorld()->increaseScore(1000);
        setDead();
    }
}

void DumbZombie::dieFromPitIfAppropriate()
{
    if(!isDead())
    {
        getStudentWorld()->playSound(SOUND_ZOMBIE_DIE);
        getStudentWorld()->increaseScore(1000);
        setDead();
    }
}

SmartZombie::SmartZombie(int imageID, double startX, double startY, StudentWorld* world)
: Zombie(imageID, startX, startY, world)
{}

void SmartZombie::doSomething()
{
    if(isDead())
        return;
    incrementParalyze();
    if(getParalyze() % 2 == 0)
        return;
    double targetX = 0, targetY = 0;
    if(getStudentWorld()->locateNearestVomitTrigger(targetX, targetY, this))
    {
        int rand = randInt(1, 3);
        if(rand < 2)
        {
            getStudentWorld()->createVomit(targetX, targetY);
            getStudentWorld()->playSound(SOUND_ZOMBIE_VOMIT);
            return;
        }
    }
    if(getMovementPlan() == 0)
    {
        setMovementPlan(randInt(3, 10));
        int dist = 0;
        Actor* nearest = getStudentWorld()->locateNearestTarget(dist, this);
        if(nearest != nullptr)
        {
            if(dist > 80)
            {
                switch(randInt(1, 4))
                {
                    case 1:
                        setDirection(up);
                        break;
                    case 2:
                        setDirection(down);
                        break;
                    case 3:
                        setDirection(left);
                        break;
                    case 4:
                        setDirection(right);
                        break;
                }
            }
            else
            {
                if(getX() == nearest->getX() || getY() == nearest->getY())
                {
                    int deltaX = nearest->getX() - getX(), deltaY = nearest->getY() - getY();
                    if(deltaX > 0) // RIGHT
                    {
                        if(!getStudentWorld()->blocked(getX() + 2, getY(), this))
                        {
                            setDirection(right);
                        }
                    }
                    else if(deltaX < 0) // LEFT
                    {
                        if(!getStudentWorld()->blocked(getX() - 2, getY(), this))
                        {
                            setDirection(left);
                        }
                    }
                    else if(deltaY > 0) // UP
                    {
                        if(!getStudentWorld()->blocked(getX(), getY() + 2, this))
                        {
                            setDirection(up);
                        }
                    }
                    else if(deltaY < 0) // DOWN
                    {
                        if(!getStudentWorld()->blocked(getX(), getY() - 2, this))
                        {
                            setDirection(down);
                        }
                    }
                }
                else if(getX() != nearest->getX() && getY() != nearest->getY())
                {
                    int deltaX = nearest->getX() - getX(), deltaY = nearest->getY() - getY();
                    int rand = randInt(1, 2);
                    if(deltaX > 0 && deltaY > 0)
                    {
                        switch(rand)
                        {
                            case 1:
                                setDirection(right);
                                break;
                            case 2:
                                setDirection(up);
                                break;
                        }
                    }
                    if(deltaX > 0 && deltaY < 0)
                    {
                        switch(rand)
                        {
                            case 1:
                                setDirection(right);
                                break;
                            case 2:
                                setDirection(down);
                                break;
                        }
                    }
                    if(deltaX < 0 && deltaY > 0)
                    {
                        switch(rand)
                        {
                            case 1:
                                setDirection(left);
                                break;
                            case 2:
                                setDirection(up);
                                break;
                        }
                    }
                    if(deltaX < 0 && deltaY < 0)
                    {
                        switch(rand)
                        {
                            case 1:
                                setDirection(left);
                                break;
                            case 2:
                                setDirection(down);
                                break;
                        }
                    }
                }
            }
        }
    }
    double dest_x = 0, dest_y = 0;
    Direction dir = getDirection();
    switch(dir)
    {
        case up:
            dest_x = getX();
            dest_y = getY() + 1;
            if(!getStudentWorld()->blocked(dest_x, dest_y, this))
            {
                moveTo(dest_x, dest_y);
                decrementMovementPlan();
                return;
            }
            break;
        case down:
            dest_x = getX();
            dest_y = getY() - 1;
            if(!getStudentWorld()->blocked(dest_x, dest_y, this))
            {
                moveTo(dest_x, dest_y);
                decrementMovementPlan();
                return;
            }
            break;
        case left:
            dest_x = getX() - 1;
            dest_y = getY();
            if(!getStudentWorld()->blocked(dest_x, dest_y, this))
            {
                moveTo(dest_x, dest_y);
                decrementMovementPlan();
                return;
            }
            break;
        case right:
            dest_x = getX() + 1;
            dest_y = getY();
            if(!getStudentWorld()->blocked(dest_x, dest_y, this))
            {
                moveTo(dest_x, dest_y);
                decrementMovementPlan();
                return;
            }
            break;
    }
    setMovementPlan(0);
}

void SmartZombie::dieFromFlameIfAppropriate()
{
    if(!isDead())
    {
        getStudentWorld()->playSound(SOUND_ZOMBIE_DIE);
        getStudentWorld()->increaseScore(2000);
        setDead();
    }
}
