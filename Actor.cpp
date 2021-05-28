#include "Actor.h"
#include "StudentWorld.h"
#include <math.h>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor Class ////////////////////////////////////////////////////////////////////////////////////////

Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth,
    StudentWorld* swActor) : GraphObject(imageID, startX, startY, dir, size, depth) {

    GraphObject::setVisible(true);
    _state = true;
    _swActor = swActor;
}

//accessor and mutator for the state of the Actor
void Actor::setState(bool state) {
    _state = state;//return state 
}

bool Actor::getState() {
    return _state;
}

StudentWorld* Actor::getWorld() {
    return _swActor;
}
Actor::~Actor() {

}

//Person Class////////////////////////////////////////
Person::Person(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth,
    StudentWorld* swActor, int hitPoints) : Actor(imageID, startX, startY, dir, size, depth, swActor) {
    _hitPoints = hitPoints;
}

int Person::getHitPoints() {
    return _hitPoints;
}
void Person::setHitPoints(int hitPoints) {
    _hitPoints = hitPoints;
}
Person::~Person() {

}

// IceMan Class ///////////////////////////////////////////////////////////////////////////////////////

IceMan::IceMan(StudentWorld* swIceMan) : Person(IID_PLAYER, 30, 60, right, 1.0, 0, swIceMan, 10) {
    //initial state of the ice man. 
    _water = 5;
    _sonar = 1;
    _goldNuggets = 0;

}

void IceMan::doSomething() {
    if (!this->getState())
        return;
    int x = this->getX();
    int y = this->getY();
    StudentWorld* world = this->getWorld();


    int key;
    if (getWorld()->getKey(key) == true) { // Accessing GameWorld's getKey fn to see if a key was hit by user.
        switch (key) {
        case KEY_PRESS_LEFT:
            if (getDirection() != left)
                setDirection(left);
            else if (getX() > 0)
                moveTo(getX() - 1, getY());
            break;

        case KEY_PRESS_RIGHT:
            if (getDirection() != right)
                setDirection(right);
            else if (getX() < VIEW_WIDTH)
                moveTo(getX() + 1, getY());
            break;

        case KEY_PRESS_UP:
            if (getDirection() != up)
                setDirection(up);
            else if (getY() < VIEW_HEIGHT - 4)
                moveTo(getX(), getY() + 1);
            break;

        case KEY_PRESS_DOWN:
            if (getDirection() != down)
                setDirection(down);
            else if (getY() > 0)
                moveTo(getX(), getY() - 1);
            break;

        case KEY_PRESS_SPACE:
            // add a squirt in front of player.
            break;

        case KEY_PRESS_TAB:
            // add here
            break;

        case KEY_PRESS_ESCAPE:
            // add here
            break;
        }
    }

    //Remove ice if ice man's location overlaps with ice objects
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (world->isIcePresent(x + i, y + j)) {
                world->destroyIce(x + i, y + j);
                world->playSound(SOUND_DIG);
            }
        }
    }

}

//annoy
void IceMan::annoy(int annoyValue) {
    if (getHitPoints() != 0) {
        int hitPoints = getHitPoints();
        hitPoints -= annoyValue;
        setHitPoints(hitPoints);
    }
    if (getHitPoints() <= 0)
        setState(false);
}
//acessor and mutator for barrels
int IceMan::getBarrels() {
    return _barrelsLeft;
}
void IceMan::setBarrels(int barrelsLeft) {
    _barrelsLeft = barrelsLeft;
}
void IceMan::reduceBarrels() {
    _barrelsLeft--;
}


//acessor and mutator for water
int IceMan::getWater() {
    return _water;
}
void IceMan::setWater(int water) {
    _water = water;
}

//acessor and mutator for sonar
int IceMan::getSonar() {
    return _sonar;
}
void IceMan::setSonar(int sonar) {
    _sonar = sonar;
}

//acessor and mutator for goldNuggets
int IceMan::getGoldNuggets() {
    return _goldNuggets;
}
void IceMan::setGoldNuggets(int goldNuggets) {
    _goldNuggets = goldNuggets;
}

IceMan::~IceMan() {
    this->setVisible(false);
}

// Ice Class //////////////////////////////////////////////////////////////////////////////////////////

Ice::Ice(int startX, int startY, StudentWorld* swIce) : Actor(IID_ICE, startX, startY, right, 0.25, 3, swIce) {
}

void Ice::doSomething() {
    return;
}
void Ice::annoy(int) {
    //ice cannot be annnoyed
}

Ice::~Ice() {
    this->setVisible(false);
}

// Protester Class ////////////////////////////////////////////////////////////////////////////////////

Protester::Protester(int image, int hitPoints, StudentWorld* swProtester, bool state)
    : Person(image, 60, 60, left, 1.0, 0, swProtester, hitPoints) {
    GraphObject::setVisible(true);
    _image = image;
    _isLeavingField = false;
    _level = getWorld()->getLevel();
    _ticksToWaitBetweenMoves = std::max(0, 3 - _level / 4);
    _ticksToWaitBetweenYells = 15;
    _numSquaresToMoveInCurrentDirection = rand() % (53) + 8; // rand()%(max-min + 1) + min;
}


// Accessor and Mutator for isLeavingField
bool Protester::getIsLeavingField() {
    return _isLeavingField;
}

void Protester::setIsLeavingField(bool isLeavingField) {
    _isLeavingField = isLeavingField;
}

bool Protester::isWithinShoutingDistance() {

    // int iceManDirection = getWorld()->getIceMan()->getDirection();

    int iceManX = getWorld()->getIceMan()->getX();
    int iceManY = getWorld()->getIceMan()->getY();

    // int xDist = abs(iceManX - getX());
    // int yDist = abs(iceManY - getY());

    double distance = sqrt((iceManX - getX()) * (iceManX - getX()) + (iceManY - getY()) * (iceManY - getY()));

    // if (xDist <= 4 && yDist <= 4) { // If (x,y) coords are within 4 or less of IceMan.  
    if (distance <= 4) {
        switch (getDirection()) {
        case left:
            if (iceManX < getX()) // If Protester is facing left toward IceMan. 
                return true;
            break;
        case right:
            if (iceManX >= getX())
                return true;
            break;
        case up:
            if (iceManY >= getY())
                return true;
            break;
        case down:
            if (iceManY <= getY())
                return true;
            break;
        }
    }
    return false;
}

bool Protester::isAbleToMove() {
    if (_ticksToWaitBetweenMoves <= 0) {
        _ticksToWaitBetweenMoves = std::max(0, 3 - _level / 4);
        _ticksToWaitBetweenYells--;
        return true;
    }
    else {
        _ticksToWaitBetweenMoves--;
        return false;
    }
}

bool Protester::isAbleToYell() {
    if (_ticksToWaitBetweenYells <= 0) {
        _ticksToWaitBetweenYells = 15;
        return true;
    }
    else {
        return false;
    }

}

bool Protester::isIceManInClearSight() {
    int iceManX = getWorld()->getIceMan()->getX();
    int iceManY = getWorld()->getIceMan()->getY();

    int i = 0;
    if (getX() == iceManX) { // If in same column...
        if (getY() > iceManY) { //...and protester is above iceman...
            while (!(getWorld()->isIcePresent(getX(), getY() - i))) { // ...check if ice blocks path btwn them.
                ++i;
                if (getY() - i == iceManY) { //  If path is clear.
                    setDirection(down);
                    moveTo(getX(), getY() - 1);
                    return true;
                }
            }
            return false;
        }
        else if (getY() < iceManY) { // If protester is below iceman....
            while (!(getWorld()->isIcePresent(getX(), getY() + i))) { // ... check if ice blocks path btwn them.
                ++i;
                if (getY() + i == iceManY) { //  If path is clear.
                    setDirection(up);
                    moveTo(getX(), getY() + 1);
                    return true;
                }
            }
            return false;
        }
    }

    if (getY() == iceManY) { // If in same row...
        if (getX() > iceManX) { //...and protester is right of iceman...
            while (!(getWorld()->isIcePresent(getX() - i, getY()))) { // ...check if ice blocks path btwn them.
                ++i;
                if (getX() - i == iceManX) { //  If path is clear.
                    setDirection(left);
                    moveTo(getX() - 1, getY());
                    return true;
                }
            }
            return false;
        }
        else if (getX() < iceManX) { // If protester is left of iceman...
            while (!(getWorld()->isIcePresent(getX() + i, getY()))) { // ...check if ice blocks path btwn them.
                ++i;
                if (getX() + i == iceManX) { //  If path is clear.
                    setDirection(right);
                    moveTo(getX() + 1, getY());
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

bool Protester::isBlocked() {

    switch (getDirection()) {
    case left:
        if (getX() <= 0) // blocked by edge of map
            return true;
        if (getWorld()->isIcePresent(getX() - 1, getY())) // if ice is to left
            return true;
        // Need some sort of check for goodies.
        break;
    case right:
        if (!(getX() < VIEW_WIDTH)) // blocked by edge of map
            return true;
        if (getWorld()->isIcePresent(getX() + 1, getY())) // if ice is to right
            return true;
        // Need some sort of check for goodies.
        break;
    case up:
        if (!(getY() < VIEW_HEIGHT - 4)) // blocked by edge of map
            return true;
        if (getWorld()->isIcePresent(getX(), getY() + 1)) // if ice is above
            return true;
        break;
        // Need some sort of check for goodies.
    case down:
        if (!(getY() > 0)) // blocked by edge of map
            return true;
        if (getWorld()->isIcePresent(getX(), getY() - 1)) // if ice is below
            return true;
        break;
        // Need some sort of check for goodies.
    }
    return false;
}

void Protester::moveOneSquare() { /*
if (_numSquaresToMoveInCurrentDirection <= 0) {
        _numSquaresToMoveInCurrentDirection = rand() % (53) + 8; // Reset to new random number.
        randomDirection(); // set direction to a NEW random direction.
    } */

    switch (getDirection()) { // Move one square.
    case left:
        if (getX() > 0 && !(getWorld()->isIcePresent(getX() - 1, getY())))
            moveTo(getX() - 1, getY());
        break;
    case right:
        if (getX() < VIEW_WIDTH && !(getWorld()->isIcePresent(getX() + 1, getY())))
            moveTo(getX() + 1, getY());
        break;
    case up:
        if (getY() < VIEW_HEIGHT - 4 && !(getWorld()->isIcePresent(getX(), getY() + 1)))
            moveTo(getX(), getY() + 1);
        break;
    case down:
        if (getY() > 0 && !(getWorld()->isIcePresent(getX(), getY() - 1)))
            moveTo(getX(), getY() - 1);
        break;
    }
    _numSquaresToMoveInCurrentDirection--;
    return;
}

void Protester::randomDirection() { // set direction to a NEW random direction.
    int directionNum = (rand() % 3) + 1; // random num btwn 1 and 3.

    if (getDirection() == up) {
        switch (directionNum) {
        case 1:
            if (getX() > 0)
                setDirection(left);
            break;
        case 2:
            if (getX() < VIEW_WIDTH)
                setDirection(right);
            break;
        case 3:
            if (getY() > 0)
                setDirection(down);
            break;
        }
    }
    else if (getDirection() == down) {
        switch (directionNum) {
        case 1:
            if (getX() > 0)
                setDirection(left);
            break;
        case 2:
            if (getX() < VIEW_WIDTH)
                setDirection(right);
            break;
        case 3:
            if (getY() < VIEW_HEIGHT - 4)
                setDirection(up);
            break;
        }
    }
    else if (getDirection() == left) {
        switch (directionNum) {
        case 1:
            if (getY() > 0)
                setDirection(down);
            break;
        case 2:
            if (getX() < VIEW_WIDTH)
                setDirection(right);
            break;
        case 3:
            if (getY() < VIEW_HEIGHT - 4)
                setDirection(up);
            break;
        }
    }
    else if (getDirection() == right) { // right
        switch (directionNum) {
        case 1:
            if (getY() > 0)
                setDirection(down);
            break;
        case 2:
            if (getX() > 0)
                setDirection(left);
            break;
        case 3:
            if (getY() < VIEW_HEIGHT - 4)
                setDirection(up);
            break;
        }
    }
    return;
}

void Protester::doSomething() {
    if (!this->getState()) // If dead, return immediately.
        return; // 1

    else if (!isAbleToMove()) { // If in rest state, return immediately and decrement ticks left to wait.
        return; // 2
    }

    else if (getHitPoints() == 0) { // If dead, have protester leave the field.
        if (getX() == 60 && getY() == 60) { // If already at exit point...
            this->getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            setState(false); // Indicator for deletion.
            setVisible(false);
            return; // 3
        }

        // Else, if not at exit point. annoy
            // => determine which direction to move based on a queue based maze searching algortihm
            // => move one step toward exit
            // => return
    }

    //This works
    else if (isWithinShoutingDistance() && isAbleToYell()) { // If able, shout at iceMan. // 4
        this->getWorld()->playSound(SOUND_PROTESTER_YELL); // A
        return;
    }

    else if (isIceManInClearSight()) { // If in clear sight, fn also switches direction toward iceman and moves one square his way. // 5ABCAB
        _numSquaresToMoveInCurrentDirection = 0;
        return;
    }


    else if (isBlocked()) { // if protester is about to run into an obstacle (ice, goodie,...edge of map?)
        //randomDirection(); // pick a new direction
        while (isBlocked()) {
            randomDirection();
        }
        moveOneSquare(); // move one square in this direction 
        return;
    }

    else { // Move one square in current direction
        // What if facing a wall or ice or goodie.
        moveOneSquare();
        return;
    }
    return;
}

Protester::~Protester() {

}
// Regular Protester Class ////////////////////////////////////////////////////////////////////////////

RegularProtester::RegularProtester(StudentWorld* swRegProtester)
    : Protester(IID_PROTESTER, 5, swRegProtester, true) {
}

void RegularProtester::annoy(int) {

}
RegularProtester::~RegularProtester() {

}
//////Goodie Class//////////////////////////////////////////////////////////////////////////

Goodie::Goodie(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth,
    StudentWorld* swGoodie) : Actor(IID_BOULDER, startX, startY, down, 1.0,
        1, swGoodie) {

}

//checks if checkX, checkY is within a 3 square radius to x,y 
bool Goodie::checkRadius3(int x, int y, int checkX, int checkY) {

    double distance = sqrt((checkX - x) * (checkX - x) + (checkY - y) * (checkY - y));
    if (distance <= 3)
        return true;
    return false;

}



//checks if checkX, checkY is within a 4 square radius to x,y
bool Goodie::checkRadius4(int x, int y, int checkX, int checkY) {
    double distance = sqrt((checkX - x) * (checkX - x) + (checkY - y) * (checkY - y));
    if (distance <= 4)
        return true;
    return false;
}
void Goodie::annoy(int) {
    //none of the goodies can be annoyed
}
Goodie::~Goodie() {

}

////////////////BOULDER CLASS///////////////////////////////////////////////////////////
Boulder::Boulder(int x, int y, StudentWorld* swBoulder) : Goodie(IID_BOULDER, x, y, down, 1.0,
    1, swBoulder) {
    _stable = true;

}

void Boulder::doSomething() {
    static int wait = 0;
    static int once = 0;
    if (!getState())
        return;

    if (_stable) {
        if (isIcePresentBelow(this->getX(), this->getY()))
            return;
        else {
            if (wait <= 30) {
                ++wait;
                return;
            }
            else {
                _stable = false;
            }
        }
    }
    else if (!_stable) {
        if (once == 0) {
            getWorld()->playSound(SOUND_FALLING_ROCK);
            once++;
        }
        //check if iceMan is within a 3 sqaure radius
        if (checkRadius3(getX(), getY(), getWorld()->getIceMan()->getX(), getWorld()->getIceMan()->getY())) {
            getWorld()->getIceMan()->annoy(100);
        }
        //check if any protertor is within a 3 square radius 
       // if(checkRadius3())
        fall();

    }
}

void Boulder::fall() {
    int x = getX();
    int y = getY();
    if (y == 0) {
        setState(false);
    }
    else if (isIcePresentBelow(x, y)) {
        setState(false);
    }

    else if (getWorld()->boulderPresent(x, y)) {
        setState(false);
    }
    else {
        moveTo(x, y - 1);
    }

    return;
}



//checks if ice is present in any of the four columns in the row below the x,y position
bool Boulder::isIcePresentBelow(int x, int y) {
    bool present = false;
    for (int i = 0; i < 4; i++) {
        if (getWorld()->isIcePresent(x + i, y - 1))
            present = true;
    }
    return present;
}


Boulder::~Boulder() {

}

///////////OIL BARREL////////////////////////////////////////
OilBarrel::OilBarrel(int x, int y, StudentWorld* swGoodie) : Goodie(IID_BARREL, x, y, right, 1.0, 2, swGoodie)
{
    setVisible(false); // oil is not visible initially
}

void OilBarrel::doSomething() {
    //if dead the function returns 
    if (!getState())
        return;

    int iceManX = getWorld()->getIceMan()->getX();
    int iceManY = getWorld()->getIceMan()->getY();
    int x = getX();
    int y = getY();

    //if not visible yet check to see if iceMan is within 4 square radius. If yes make visible and return
    if (!isVisible()) {

        if (!isVisible()) {
            if (checkRadius4(x, y, iceManX, iceManY)) {
                setVisible(true);
                return;

            }
            else
                return;
        }
    }
    //if visible and within 3 square radius pick it up
    else if (isVisible() && checkRadius3(x, y, iceManX, iceManY)) {
        setState(false);
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->getIceMan()->reduceBarrels();
    }


}


OilBarrel::~OilBarrel() {
    //setVisible(false); 
}

//////SQUIRT//////////////////////////////////////////////////////////////////////
Squirt::Squirt(int x, int y, Direction dir, StudentWorld* swWorld) : Goodie(IID_WATER_SPURT
    , x, y, dir, 1.0, 1, swWorld) {
    _travelDis = 4;
}

void Squirt::doSomething() {

    //if withing 3 radius of a protestor cause 3 annoyance. setstate dead
    if (_travelDis == 0)
        setState(false);
    //if ice or boulder infront setStaet to dead
    else if (getWorld()->boulderPresent(getX(), getY()))
        setState(false);

    else if (getState()) {

        switch (getDirection()) {
        case up:
            if (getWorld()->isIcePresent(getX(), getY() + 1))
                setState(false);
        case down:
            if (getWorld()->isIcePresent(getX(), getY() - 1))
                setState(false);
        case right:
            if (getWorld()->isIcePresent(getX() + 1, getY()))
                setState(false);
        case left:
            if (getWorld()->isIcePresent(getX() - 1, getY()))
                setState(false);
        }
    }

    //else move forward in dir
    if (getState()) {
        switch (getDirection()) {
        case up:
            moveTo(getX(), getY() + 1);
            return;
        case down:
            moveTo(getX(), getY() - 1);
            return;
        case right:
            moveTo(getX() + 1, getY());
            return;
        case left:
            moveTo(getX() - 1, getY());
            return;
        }
    }
}


Squirt::~Squirt() {

}
