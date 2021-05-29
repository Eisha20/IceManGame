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

//checks if checkX, checkY is within a specified radius to x,y 
bool Actor::checkRadius(int x, int y, int checkX, int checkY, int radius) {
    double distance = sqrt((checkX - x) * (checkX - x) + (checkY - y) * (checkY - y));
    if (distance <= radius)
        return true;
    return false;

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

bool Person::isAnnoyable() {
    return true;
}

bool Person::canBlock() {
    return false;
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
            else if (getX() < VIEW_WIDTH - 4)
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
            if (getWater() > 0) {
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                switch (getWorld()->getIceMan()->getDirection()) {
                case up:
                    getWorld()->makeSquirt(x, y + 4);
                    break;
                case down:
                    getWorld()->makeSquirt(x, y - 4);
                    break;
                case left:
                    getWorld()->makeSquirt(x - 4, y);
                    break;
                case right:
                    getWorld()->makeSquirt(x + 4, y);
                    break;
                }
                break;

        case KEY_PRESS_TAB:
            // add here
            if (getGoldNuggets() > 0) {
                getWorld()->makeDroppedGoldNugget();
                getWorld()->getIceMan()->decGoldNuggets();
            }
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
void IceMan::decWater() {
    _water--;
}
void IceMan::incWater(int num) {
    _water += num;
}
//acessor and mutator for sonar
int IceMan::getSonar() {
    return _sonar;
}
void IceMan::decSonar() {
    _sonar--;
}
void IceMan::incSonar() {
    _sonar++;
}
//acessor and mutator for goldNuggets
int IceMan::getGoldNuggets() {
    return _goldNuggets;
}
void IceMan::incGoldNuggets() {
    _goldNuggets++;
}

void IceMan::decGoldNuggets() {
    _goldNuggets--;
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

bool Ice::canBlock() {
    return false;// it can block but might not be required to return true for implimentation
}
bool Ice::isAnnoyable() {
    return false;
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
    _ticksBetweenPerpendicularTurns = 200;
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

    if (checkRadius(getX(), getY(), iceManX, iceManY, 4)) {
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
        _ticksBetweenPerpendicularTurns--;
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
    int iceManX = getWorld()->getIceMan()->getX(); // create a private variable for these
    int iceManY = getWorld()->getIceMan()->getY();

    //if (checkRadius(getX(), getY(), iceManX, iceManY, 4)) { // if within a radius of 4. Spec says so but im unsure
      //  return false;
    //}
    //if (!canMoveTowardIceMan())
       // return false;

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
            while (!(getWorld()->isIcePresent(getX() - i, getY()))) { // ...check if ice blocks path btwn them. // -3 works but crashes
                ++i;
                if (getX() - i == iceManX) { //  If path is clear. // -3 works but crashes
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

bool Protester::canMoveTowardIceMan() {

    int iceManX = getWorld()->getIceMan()->getX();
    int iceManY = getWorld()->getIceMan()->getY();
    /*
    If a protester is to the right of iceman by 4 or less spaces(aka iceManX + 4 >= protesterX) and protesterY == iceManY(aka on same level)
        return false
        */
    return false; // testing


    if (iceManX + 4 >= getX())// && iceManY == getY())
        return false;
    return true;
}


bool Protester::canPerpendicularlyMove() {

    if (_ticksBetweenPerpendicularTurns > 0)
        return false;

    int randomNumber = (rand() % 2) + 1; // random num btwn 1 and 2.
    bool noIceLeftward = !(getWorld()->isIceAt(getX(), getY(), getX() - 1, getY() + 3));
    bool noIceRightward = !(getWorld()->isIceAt(getX(), getY(), getX() + 4, getY() + 3));
    bool noIceAbove = !(getWorld()->isIceAt(getX(), getY(), getX() + 3, getY() + 4));
    bool noIceBelow = !(getWorld()->isIceAt(getX(), getY(), getX() + 3, getY() - 1));

    if (getDirection() == left || getDirection() == right) {
        if (noIceAbove && noIceBelow && getY() < VIEW_HEIGHT - 4 && getY() > 0) { // If able to move left and right randomly select a direction
            // check boulders
            if (randomNumber == 1) { // move up
                setDirection(up);
                moveOneSquare();
            }
            else {  // randomNumber == 2 // move down
                setDirection(down);
                moveOneSquare();
            }
        }
        else if (noIceAbove && getY() < VIEW_HEIGHT - 4) { // move up
            setDirection(up);
            moveOneSquare();
        }
        else if (noIceBelow && getY() > 0) { // move down
            setDirection(down);
            moveOneSquare();
        }
        else { // ice above and below
            return false;
        }
        return true;
    }

    else if (getDirection() == up || getDirection() == down) {
        if (noIceLeftward && noIceRightward && getX() > 0 && getX() < VIEW_WIDTH - 4) {
            // check boulders
            if (randomNumber == 1) { // move left
                setDirection(left);
                moveOneSquare();
            }
            else {  // randomNumber == 2 // move right
                setDirection(right);
                moveOneSquare();
            }
        }
        else if (noIceLeftward && getX() > 0) { // move left
            setDirection(left);
            moveOneSquare();
        }
        else if (noIceRightward && getX() < VIEW_WIDTH - 4) { // move right
            setDirection(right);
            moveOneSquare();
        }
        else { // ice on both sides
            return false;
        }
        return true;
    }

    return true;
}

bool Protester::isBlocked() {

    switch (getDirection()) {
    case left:
        if (getX() <= 0) // blocked by edge of map
            return true;
        if (getWorld()->isIceAt(getX(), getY(), getX() - 1, getY() + 3)) // if ice is to left
            return true;
        //if (getWorld()->boulderPresent(getX() - 1, getY()))
           // return true;
        // Need some sort of check for goodies.
        break;
    case right:
        if (!(getX() < VIEW_WIDTH - 4)) // blocked by edge of map 
            return true;
        if (getWorld()->isIceAt(getX(), getY(), getX() + 4, getY() + 3)) // if ice is to right
            return true;
        // Need some sort of check for goodies.
        break;
    case up:
        if (!(getY() < VIEW_HEIGHT - 4)) // blocked by edge of map
            return true;
        if (getWorld()->isIceAt(getX(), getY(), getX() + 3, getY() + 4)) // if ice is above
            return true;
        break;
        // Need some sort of check for goodies.
    case down:
        if (!(getY() > 0)) // blocked by edge of map
            return true;
        if (getWorld()->isIceAt(getX(), getY(), getX() + 3, getY() - 1)) // if ice is below
            return true;
        break;
        // Need some sort of check for goodies.
    }
    return false;
}

void Protester::moveOneSquare() {
    if (_numSquaresToMoveInCurrentDirection <= 0) {
        _numSquaresToMoveInCurrentDirection = rand() % (53) + 8; // Reset to new random number.
        randomDirection(); // set direction to a NEW random direction.
    }

    switch (getDirection()) { // Move one square.
    case left:
        if (getX() > 0 && !(getWorld()->isIceAt(getX(), getY(), getX() - 1, getY() + 3))) {
            moveTo(getX() - 1, getY());

        }
        break;
    case right:
        if (getX() < VIEW_WIDTH - 4 && !(getWorld()->isIceAt(getX(), getY(), getX() + 4, getY() + 3)))
            moveTo(getX() + 1, getY());
        break;
    case up:
        if (getY() < VIEW_HEIGHT - 4 && !(getWorld()->isIceAt(getX(), getY(), getX() + 3, getY() + 4)))
            moveTo(getX(), getY() + 1);
        break;
    case down:
        if (getY() > 0 && !(getWorld()->isIceAt(getX(), getY(), getX() + 3, getY() - 1)))
            moveTo(getX(), getY() - 1);
        break;
    }
    //_numSquaresToMoveInCurrentDirection--;
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
            if (getX() < VIEW_WIDTH - 4)
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
            if (getX() < VIEW_WIDTH - 4)
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
            if (getX() < VIEW_WIDTH - 4)
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

void Protester::doSomething() { // P. 40 

    /*
     Currently need to:
     fix movement of protester when in close proximity to iceman
     breadth first search algorithm
     handle when protesters are close to boulders
    */

    //if (getWorld()->isBlockableActorNearby(this, 3)) {
         //setVisible(false);
    //}

    //if (getX() == getWorld()->getIceMan()->getX()) // FOR TESTING
        //setHitPoints(0);

    if (!this->getState()) // If dead, return immediately. // 1.
        return;

    else if (!isAbleToMove()) { // If in rest state, return immediately and decrement ticks left to wait. // 2
        return;
    }

    else if (getHitPoints() <= 0) { // If dead, have protester leave the field. // 3
        if (getX() == 60 && getY() == 60) { // If already at exit point... // A
            this->getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            setState(false); // Indicator for deletion.
            setVisible(false);
            return;
        }
        //setVisible(false); // FOR TESTING

        // Else, if not at exit point. annoy // B
            // => determine which direction to move based on a queue based maze searching algortihm
            // => move one step toward exit
            // => return
    }

    //This works
    else if (isWithinShoutingDistance() && isAbleToYell()) { // If able, shout at iceMan. // 4
        this->getWorld()->playSound(SOUND_PROTESTER_YELL); // A
        // annoy iceman and deduct 2 points from the iceman's hit points, possibly killing the iceman.
        getWorld()->getIceMan()->annoy(2);
        return;
    }

    else if (isIceManInClearSight()) { // 5ABCAB // && canMoveTowardIceMan(
        // If in clear sight, fn switches direction toward iceman and moves one square his way. 
        _numSquaresToMoveInCurrentDirection = 0;
        return;
    }


    else if (isBlocked()) { // if protester is about to run into an obstacle (ice, goodie,...edge of map?) // 6
        //--_numSquaresToMoveInCurrentDirection;
        while (isBlocked()) {
            randomDirection();
        }
        _numSquaresToMoveInCurrentDirection = rand() % (53) + 8;
        moveOneSquare(); // move one square in this direction 
        return;
    }

    else if (canPerpendicularlyMove()) { // 7 // perpendiular stuff
        _numSquaresToMoveInCurrentDirection = rand() % (53) + 8;
        return;
    }

    else { // 8
        moveOneSquare();
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
    StudentWorld* swGoodie) : Actor(imageID, startX, startY, dir, size,
        depth, swGoodie) {

}

void Goodie::annoy(int) {
    //none of the goodies can be annoyed
}

bool Goodie::canBlock() {
    return false;
}
bool Goodie::isAnnoyable() {
    return false;
}

Goodie::~Goodie() {

}

////////////////BOULDER CLASS///////////////////////////////////////////////////////////
Boulder::Boulder(int x, int y, StudentWorld* swBoulder) : Goodie(IID_BOULDER, x, y, down, 1.0,
    1, swBoulder) {
    _stable = true;
    _playSound = true;
}

void Boulder::doSomething() {
    static int wait = 0;

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
                wait = 0;
            }
        }
    }
    else if (!_stable) {
        if (_playSound) {
            getWorld()->playSound(SOUND_FALLING_ROCK);
            _playSound = false;
        }
        //check if iceMan is within a 3 sqaure radius
        if (checkRadius(getX(), getY(), getWorld()->getIceMan()->getX(), getWorld()->getIceMan()->getY(), 3)) {
            getWorld()->getIceMan()->annoy(100);
        }
        getWorld()->annoyPerson(getX(), getY(), 100, 3, true);
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

bool Boulder::canBlock() {
    return true;
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
            if (checkRadius(x, y, iceManX, iceManY, 4)) {
                setVisible(true);
                return;

            }
            else
                return;
        }
    }
    //if visible and within 3 square radius pick it up
    else if (isVisible() && checkRadius(x, y, iceManX, iceManY, 3)) {
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

    //if withing 3 radius of a protestor cause 2 annoyance. setstate dead
    if (getWorld()->annoyPerson(getX(), getY(), 2, 3, false))
        setState(false);
    else if (_travelDis == 0)
        setState(false);
    //if ice or boulder infront setStaet to dead
    else if (getWorld()->boulderPresent(getX(), getY()))
        setState(false);

    else if (getState()) {

        switch (getDirection()) {
        case up:
            if (getWorld()->isIcePresent(getX(), getY() + 1))
                setState(false);
            break;
        case down:
            if (getWorld()->isIcePresent(getX(), getY() - 1))
                setState(false);
            break;
        case right:
            if (getWorld()->isIcePresent(getX() + 1, getY()))
                setState(false);
            break;
        case left:
            if (getWorld()->isIcePresent(getX() - 1, getY()))
                setState(false);
            break;
        }
    }

    //need to reduce travle distance
    //else move forward in dir
    if (getState()) {
        switch (getDirection()) {
        case up:
            moveTo(getX(), getY() + 1);
            decTravelDis();
            return;
        case down:
            moveTo(getX(), getY() - 1);
            decTravelDis();
            return;

        case right:
            moveTo(getX() + 1, getY());
            decTravelDis();
            return;

        case left:
            moveTo(getX() - 1, getY());
            decTravelDis();
            return;
        }
    }
}

void Squirt::decTravelDis() {
    --_travelDis;
}

Squirt::~Squirt() {

}

/////Gold Nugget//////////////////////////////////////////////////////////////
GoldNugget::GoldNugget(int x, int y, StudentWorld* swNugget, bool isPickableProtester) : Goodie(IID_GOLD, x, y,
    right, 1.0, 2, swNugget) {
    if (isPickableProtester) {
        _pickableProtester = true;
        _permanent = false;
        setVisible(true);
    }
    else {
        _pickableProtester = false;
        _permanent = true;
        setVisible(false);
    }
}

bool GoldNugget::isPermanent() {
    return _permanent;
}


bool GoldNugget::isPickableProtester() {
    return _pickableProtester;
}

void GoldNugget::doSomething() {
    static int countTicks = 0;
    if (!getState())
        return;
    if (!isPickableProtester()) {
        if (!isVisible() && checkRadius(getX(), getY(), getWorld()->getIceMan()->getX(), getWorld()->getIceMan()->getY(), 4)) {
            setVisible(true);
            return;
        }
        else if (checkRadius(getX(), getY(), getWorld()->getIceMan()->getX(), getWorld()->getIceMan()->getY(), 3)) {
            setState(false);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseScore(10);
            getWorld()->getIceMan()->incGoldNuggets();
        }
    }
    else if (isPickableProtester()) {
        if (!isPermanent()) {
            countTicks++;
        }
        if (getWorld()->pickGold(getX(), getY(), 3)) {
            setState(false);
            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
            getWorld()->increaseScore(25);
        }
        else if (countTicks >= 100) {
            setState(false);
            countTicks = 0;
        }
    }
}

GoldNugget::~GoldNugget() {

}
///Sonar/////////////////////////////////////////////////////////////////
Sonar::Sonar(int x, int y, StudentWorld* swSonar) : Goodie(IID_SONAR, x, y, right, 1.0, 2, swSonar) {
    int num = 300 - (10 * getWorld()->getLevel());
    _ticksLeft = std::max(100, num);
}

int Sonar::getTicks() {
    return _ticksLeft;
}

void Sonar::decTicks() {
    _ticksLeft--;
}
void Sonar::doSomething() {
    if (!getState())
        return;
    if (checkRadius(getX(), getY(), getWorld()->getIceMan()->getX(), getWorld()->getIceMan()->getY(), 3)) {
        setState(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getIceMan()->incSonar();
        getWorld()->increaseScore(75);
    }
    else if (getTicks() <= 0) {
        setState(false);
    }
    else
        decTicks();
}

Sonar::~Sonar() {

}

///Water Pool///////////////////////////////////////////////////////////
WaterPool::WaterPool(int x, int y, StudentWorld* swSonar) : Goodie(IID_WATER_POOL, x, y, right, 1.0, 2, swSonar) {
    int num = 300 - (10 * getWorld()->getLevel());
    _ticksLeft = std::max(100, num);
}

int WaterPool::getTicks() {
    return _ticksLeft;
}

void WaterPool::decTicks() {
    _ticksLeft--;
}



void WaterPool::doSomething() {
    if (!getState())
        return;
    if (checkRadius(getX(), getY(), getWorld()->getIceMan()->getX(), getWorld()->getIceMan()->getY(), 3)) {
        setState(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getIceMan()->incWater(5);
        getWorld()->increaseScore(100);
    }
    else if (getTicks() <= 0) {
        setState(false);
    }
    else
        decTicks();
}


WaterPool::~WaterPool() {

}