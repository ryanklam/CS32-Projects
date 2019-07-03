#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>  // defines the manipulator setw
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_penelope = nullptr;
    numCitizens = 0;
    tickStatus = GWSTATUS_CONTINUE_GAME;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    m_penelope = nullptr;
    numCitizens = 0;
    tickStatus = GWSTATUS_CONTINUE_GAME;
    level();
    return tickStatus;
}

int StudentWorld::move()
{
    m_penelope->doSomething();
    list<Actor*>::iterator a = actors.begin();
    while(a != actors.end())
    {
        if(!(*a)->isDead())
        {
            (*a)->doSomething();
            if(m_penelope->isDead())
                return GWSTATUS_PLAYER_DIED;
        }
        a++;
    }
    a = actors.begin();
    while(a != actors.end())
    {
        if((*a)->isDead())
        {
            delete (*a);
            a = actors.erase(a);
        }
        a++;
    }
    ostringstream scorestring;
    if(getScore() < 0)
    {
        scorestring.fill('0');
        scorestring << '-' << setw(5) << (getScore() * -1);
    }
    else
    {
        scorestring.fill('0');
        scorestring << setw(6) << getScore();
    }
    
    setGameStatText("Score: " + scorestring.str() + "  Level: " + getString(getLevel())  + "  Lives: " + getString(getLives()) +            "  Vaccines: " + getString(m_penelope->getVaccineCount()) + "  Flames: " + getString(m_penelope->getFuelCount()) + "  Mines: " + getString(m_penelope->getMineCount()) + "  Infected: " + getString(m_penelope->getInfectionCount()));
    return tickStatus;
}

void StudentWorld::cleanUp()
{
    if(m_penelope != nullptr)
    {
        delete m_penelope;
        m_penelope = nullptr;
    }
    list<Actor*>::iterator a = actors.begin();
    while(a != actors.end())
    {
        delete (*a);
        a = actors.erase(a);
    }
}


bool StudentWorld::overlaps(double x, double y)
{
    int xDiff = m_penelope->getX() - x;
    int yDiff = m_penelope->getY() - y;
    if(((xDiff * xDiff) + (yDiff * yDiff) <= 100))
        return true;
    list<Actor*>::iterator i = actors.begin();
    while(i != actors.end())
    {
        xDiff = (*i)->getX() - x;
        yDiff = (*i)->getY() - y;
        if(((xDiff * xDiff) + (yDiff * yDiff) <= 100))
            return true;
        i++;
    }
    return false;
}

bool StudentWorld::blocked(double playerX, double playerY, Actor* a)
{
    list<Actor*>::iterator x = actors.begin();
    while(x != actors.end())
    {
        if((*x)->blocksMovement() && (*x) != a)
        {
            if((playerX >= (*x)->getX() && playerX <= (*x)->getX() + SPRITE_WIDTH - 1) && (playerY >= (*x)->getY() && playerY <= (*x)->getY() + SPRITE_HEIGHT - 1))
                return true;
            
            if((playerX + SPRITE_WIDTH - 1 >= (*x)->getX() && playerX + SPRITE_WIDTH - 1 <= (*x)->getX() + SPRITE_WIDTH - 1) && (playerY >= (*x)->getY() && playerY <= (*x)->getY() + SPRITE_HEIGHT - 1))
                return true;
            
            if((playerX >= (*x)->getX() && playerX <= (*x)->getX() + SPRITE_WIDTH - 1) && (playerY + SPRITE_HEIGHT - 1 >= (*x)->getY() && playerY + SPRITE_HEIGHT - 1 <= (*x)->getY() + SPRITE_HEIGHT - 1))
                return true;
            
            if((playerX + SPRITE_WIDTH - 1 >= (*x)->getX() && playerX + SPRITE_WIDTH - 1 <= (*x)->getX() + SPRITE_WIDTH - 1) && (playerY + SPRITE_HEIGHT - 1 >= (*x)->getY() && playerY + SPRITE_HEIGHT - 1 <= (*x)->getY() + SPRITE_HEIGHT - 1))
                return true;
        }
        x++;
    }
    if(m_penelope != a)
    {
        if((playerX >= m_penelope->getX() && playerX <= m_penelope->getX() + SPRITE_WIDTH - 1) && (playerY >= m_penelope->getY() && playerY <= m_penelope->getY() + SPRITE_HEIGHT - 1))
            return true;
        
        if((playerX + SPRITE_WIDTH - 1 >= m_penelope->getX() && playerX + SPRITE_WIDTH - 1 <= m_penelope->getX() + SPRITE_WIDTH - 1) && (playerY >= m_penelope->getY() && playerY <= m_penelope->getY() + SPRITE_HEIGHT - 1))
            return true;
        
        if((playerX >= m_penelope->getX() && playerX <= m_penelope->getX() + SPRITE_WIDTH - 1) && (playerY + SPRITE_HEIGHT - 1 >= m_penelope->getY() && playerY + SPRITE_HEIGHT - 1 <= m_penelope->getY() + SPRITE_HEIGHT - 1))
            return true;
        
        if((playerX + SPRITE_WIDTH - 1 >= m_penelope->getX() && playerX + SPRITE_WIDTH - 1 <= m_penelope->getX() + SPRITE_WIDTH - 1) && (playerY + SPRITE_HEIGHT - 1 >= m_penelope->getY() && playerY + SPRITE_HEIGHT - 1 <= m_penelope->getY() + SPRITE_HEIGHT - 1))
            return true;
    }
    return false;
}

void StudentWorld::activateOnAppropriateActor(Actor* a)
{
    int xDiff = (m_penelope)->getX() - a->getX();
    int yDiff = (m_penelope)->getY() - a->getY();
    if((xDiff * xDiff) + (yDiff * yDiff) <= 100)
    {
        a->activateIfAppropriate(m_penelope);
        
    }
    list<Actor*>::iterator x = actors.begin();
    while(x != actors.end())
    {
        if((*x) == a && a->canExplode())
            return;
        xDiff = (*x)->getX() - a->getX();
        yDiff = (*x)->getY() - a->getY();
        if((xDiff * xDiff) + (yDiff * yDiff) <= 100)
        {
            a->activateIfAppropriate((*x));
        }
        x++;
    }
}

void StudentWorld::createLandmine()
{
    if(m_penelope->getMineCount() > 0)
    {
        actors.push_back(new Landmine(IID_LANDMINE, m_penelope->getX(), m_penelope->getY(), this));
        m_penelope->decMine();
    }
}

void StudentWorld::createVaccine(double x, double y)
{
    actors.push_back(new VaccineGoodie(IID_VACCINE_GOODIE, x, y, this));
}

void StudentWorld::createPit(double x, double y)
{
    actors.push_back(new Pit(IID_PIT, x, y, this));
}

void StudentWorld::createFlame(double x, double y, Direction dir)
{
    if(!isFlameBlockedAt(x, y))
        actors.push_back(new Flame(IID_FLAME, x, y, dir, this));
}

bool StudentWorld::isFlameBlockedAt(double x, double y)
{
    list<Actor*>::iterator a = actors.begin();
    while(a != actors.end())
    {
        int xDiff = (*a)->getX() - x;
        int yDiff = (*a)->getY() - y;
        if(((xDiff * xDiff) + (yDiff * yDiff) <= 100) && (*a)->blocksFlame())
            return true;
        a++;
    }
    return false;
}

void StudentWorld::useFlamethrower(Direction dir)
{
    switch(dir)
    {
        case GraphObject::up:
            for(int i = 1; i < 4; i++)
            {
                if(isFlameBlockedAt(m_penelope->getX(), m_penelope->getY() + (i * SPRITE_HEIGHT)))
                   break;
                createFlame(m_penelope->getX(), m_penelope->getY() + (i * SPRITE_HEIGHT), GraphObject::up);
            }
            break;
        case GraphObject::down:
            for(int i = 1; i < 4; i++)
            {
                if(isFlameBlockedAt(m_penelope->getX(), m_penelope->getY() - (i * SPRITE_HEIGHT)))
                    break;
                createFlame(m_penelope->getX(), m_penelope->getY() - (i * SPRITE_HEIGHT), GraphObject::down);
            }
            break;
        case GraphObject::left:
            for(int i = 1; i < 4; i++)
            {
                if(isFlameBlockedAt(m_penelope->getX() - (i * SPRITE_HEIGHT), m_penelope->getY()))
                    break;
                createFlame(m_penelope->getX() - (i * SPRITE_HEIGHT), m_penelope->getY(), GraphObject::left);
            }
            break;
        case GraphObject::right:
            for(int i = 1; i < 4; i++)
            {
                if(isFlameBlockedAt(m_penelope->getX() + (i * SPRITE_HEIGHT), m_penelope->getY()))
                    break;
                createFlame(m_penelope->getX() + (i * SPRITE_HEIGHT), m_penelope->getY(), GraphObject::right);
            }
            break;
    }
}

void StudentWorld::createVomit(double x, double y)
{
    actors.push_back(new Vomit(IID_VOMIT, x, y, this));
}

void StudentWorld::createZombie(double x, double y)
{
    int randomNumber = randInt(1, 10);
    if(randomNumber < 8)
        actors.push_back(new DumbZombie(IID_ZOMBIE, x, y, this));
    else
        actors.push_back(new SmartZombie(IID_ZOMBIE, x, y, this));
}

bool StudentWorld::locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distP, double& distZ, bool& isThreat)
{
    double xDiff = m_penelope->getX() - x , yDiff = m_penelope->getY() - y;
    double dp = (xDiff * xDiff) + (yDiff * yDiff);
    double dz = 100000;
    list<Actor*>::iterator it = actors.begin();
    Actor* closestZ = nullptr;
    while(it != actors.end())
    {

        if((*it)->canInfect())
        {
            xDiff = (*it)->getX() - x;
            yDiff = (*it)->getY() - y;
            double temp = (xDiff * xDiff) + (yDiff * yDiff);
            if(temp < dz)
            {
                closestZ = (*it);
                dz = temp;
            }
        }
        it++;
    }
    if(dp < dz || dz == 0)
    {
        otherX = m_penelope->getX();
        otherY = m_penelope->getY();
        distP = dp;
        isThreat = false;
        return true;
    }
    if(closestZ == nullptr)
        return false;
    else
    {
        otherX = closestZ->getX();
        otherY = closestZ->getY();
        distZ = dz;
        isThreat = true;
        return true;
    }
}

bool StudentWorld::locateNearestCitizenThreat(double x, double y, double& distZ)
{
    double xDiff, yDiff;
    double dz = 100000;
    list<Actor*>::iterator it = actors.begin();
    Actor* closestZ = nullptr;
    while(it != actors.end())
    {
        if((*it)->canInfect())
        {
            xDiff = (*it)->getX() - x;
            yDiff = (*it)->getY() - y;
            double temp = (xDiff * xDiff) + (yDiff * yDiff);
            if(temp < dz)
            {
                closestZ = (*it);
                dz = temp;
            }
        }
        it++;
    }
    if(closestZ != nullptr)
    {
        distZ = dz;
        return true;
    }
    return false;
}

bool StudentWorld::locateNearestVomitTrigger(double& targetX, double& targetY, Actor* a)
{
    Direction dir = a->getDirection();
    switch(dir)
    {
        case GraphObject::up:
        {
            int xDiff = a->getX() - m_penelope->getX();
            int yDiff = (a->getY() + SPRITE_HEIGHT) - m_penelope->getY();
            if((xDiff*xDiff) + (yDiff*yDiff) <= 100)
            {
                targetX = a->getX();
                targetY = a->getY() + SPRITE_HEIGHT;
                return true;
            }
            list<Actor*>::iterator it = actors.begin();
            while(it != actors.end())
            {
                if((*it)->canBeInfected())
                {
                    xDiff = a->getX() - (*it)->getX();
                    yDiff = (a->getY() + SPRITE_HEIGHT) - (*it)->getY();
                    if((xDiff*xDiff) + (yDiff*yDiff) <= 100)
                    {
                        targetX = a->getX();
                        targetY = a->getY() + SPRITE_HEIGHT;
                        return true;
                    }
                }
                it++;
            }
            break;
        }
        case GraphObject::down:
        {
            int xDiff = a->getX() - m_penelope->getX();
            int yDiff = (a->getY() - SPRITE_HEIGHT) - m_penelope->getY();
            if((xDiff*xDiff) + (yDiff*yDiff) <= 100)
            {
                targetX = a->getX();
                targetY = a->getY() - SPRITE_HEIGHT;
                return true;
            }
            list<Actor*>::iterator it = actors.begin();
            while(it != actors.end())
            {
                if((*it)->canBeInfected())
                {
                    xDiff = a->getX() - (*it)->getX();
                    yDiff = (a->getY() - SPRITE_HEIGHT) - (*it)->getY();
                    if((xDiff*xDiff) + (yDiff*yDiff) <= 100)
                    {
                        targetX = a->getX();
                        targetY = a->getY() - SPRITE_HEIGHT;
                        return true;
                    }
                }
                it++;
            }
            break;
        }
        case GraphObject::left:
        {
            int xDiff = (a->getX() - SPRITE_WIDTH) - m_penelope->getX();
            int yDiff = a->getY()- m_penelope->getY();
            if((xDiff*xDiff) + (yDiff*yDiff) <= 100)
            {
                targetX = a->getX() - SPRITE_WIDTH;
                targetY = a->getY();
                return true;
            }
            list<Actor*>::iterator it = actors.begin();
            while(it != actors.end())
            {
                if((*it)->canBeInfected())
                {
                    xDiff = (a->getX() - SPRITE_WIDTH) - (*it)->getX();
                    yDiff = a->getY() - (*it)->getY();
                    if((xDiff*xDiff) + (yDiff*yDiff) <= 100)
                    {
                        targetX = a->getX() - SPRITE_WIDTH;
                        targetY = a->getY();
                        return true;
                    }
                }
                it++;
            }
            break;
        }
        case GraphObject::right:
        {
            int xDiff = (a->getX() + SPRITE_WIDTH) - m_penelope->getX();
            int yDiff = a->getY()- m_penelope->getY();
            if((xDiff*xDiff) + (yDiff*yDiff) <= 100)
            {
                targetX = a->getX() + SPRITE_WIDTH;
                targetY = a->getY();
                return true;
            }
            list<Actor*>::iterator it = actors.begin();
            while(it != actors.end())
            {
                if((*it)->canBeInfected())
                {
                    xDiff = (a->getX() + SPRITE_WIDTH) - (*it)->getX();
                    yDiff = a->getY() - (*it)->getY();
                    if((xDiff*xDiff) + (yDiff*yDiff) <= 100)
                    {
                        targetX = a->getX() + SPRITE_WIDTH;
                        targetY = a->getY();
                        return true;
                    }
                }
                it++;
            }
            break;
        }
    }
    return false;
}

Actor* StudentWorld::locateNearestTarget(int dist, Actor* a)
{
    int min = 100000;
    Actor* result = nullptr;
    int xDiff = a->getX() - m_penelope->getX();
    int yDiff = a->getY()- m_penelope->getY();
    if((xDiff*xDiff) + (yDiff*yDiff) < min)
    {
        min = (xDiff*xDiff) + (yDiff*yDiff);
        result = m_penelope;
    }
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end())
    {
        if((*it)->canBeInfected())
        {
            xDiff = a->getX() - (*it)->getX();
            yDiff = a->getY() - (*it)->getY();
            if((xDiff*xDiff) + (yDiff*yDiff) < min)
            {
                min = (xDiff*xDiff) + (yDiff*yDiff);
                result = (*it);
            }
        }
        it++;
    }
    dist = min;
    return result;
}

void StudentWorld::levelFinished()
{
    tickStatus = GWSTATUS_FINISHED_LEVEL;
}

void StudentWorld::level()
{
    Level lev(assetPath());
    string levelFile = "level0" + getString(getLevel()) + ".txt";
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (getLevel() > 99 || result == Level::load_fail_file_not_found)
    {
        cerr << "Cannot find level01.txt data file" << endl;
        tickStatus = GWSTATUS_PLAYER_WON;
    }
    else if (result == Level::load_fail_bad_format)
    {
        cerr << "Your level was improperly formatted" << endl;
        tickStatus = GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        for(int i = 0; i < LEVEL_WIDTH; i++)
        {
            for(int j = 0; j < LEVEL_HEIGHT; j++)
            {
                Level::MazeEntry ge = lev.getContentsOf(i,j);
                switch (ge)
                {
                    case Level::empty:
                        break;
                    case Level::player:
                        m_penelope = new Penelope(IID_PLAYER, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this);
                        break;
                    case Level::wall:
                        actors.push_back(new Wall(IID_WALL, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::exit:
                        actors.push_back(new Exit(IID_EXIT, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::pit:
                        actors.push_back(new Pit(IID_PIT, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::gas_can_goodie:
                        actors.push_back(new GasCanGoodie(IID_GAS_CAN_GOODIE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::vaccine_goodie:
                        actors.push_back(new VaccineGoodie(IID_VACCINE_GOODIE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::landmine_goodie:
                        actors.push_back(new LandmineGoodie(IID_LANDMINE_GOODIE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::citizen:
                        actors.push_back(new Citizen(IID_CITIZEN, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        numCitizens++;
                        break;
                    case Level::dumb_zombie:
                        actors.push_back(new DumbZombie(IID_ZOMBIE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::smart_zombie:
                        actors.push_back(new SmartZombie(IID_ZOMBIE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                }
            }
        }
    }
}
