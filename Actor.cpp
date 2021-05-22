#include "Actor.h"
#include "StudentWorld.h"

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
    _state = state;
}

bool Actor::getState() {
    return _state;
}

StudentWorld* Actor::getWorld() {
    return _swActor;
}
Actor::~Actor() {

}

// IceMan Class ///////////////////////////////////////////////////////////////////////////////////////

IceMan::IceMan(StudentWorld* swIceMan) : Actor(IID_PLAYER, 30, 60, right, 1.0, 0, swIceMan) {
    //initial state of the ice man. 
    _water = 5;
    _sonar = 1;
    _goldNuggets = 0;
    _health = 100;
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
//acessor and mutator for barrels
int IceMan::getBarrels() {
    return _barrelsLeft;
}
void IceMan::setBarrels(int barrelsLeft) {
    _barrelsLeft = barrelsLeft;
}

//acessor and mutator for health
int IceMan::getHealth() {
    return _health;
}
void IceMan::decHealth(int health) {
    /* if (health != 0) {
         _health =
     }*/

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

Ice::~Ice() {
    this->setVisible(false);
}

// Protester Class ////////////////////////////////////////////////////////////////////////////////////

Protester::Protester(int image, int hitPoints, StudentWorld* swProtester)
    : Actor(image, 60, 60, left, 1.0, 0, swProtester) {
    GraphObject::setVisible(true);
    _swProtester = swProtester;
    _hitPoints = hitPoints;
    _image = image;
    _isLeavingField = false;
    _level = getWorld()->getLevel();
    _ticksToWaitBetweenMoves = std::max(0, 3 - _level / 4);
    _ticksToWaitBetweenYells = 15;
    _numSquaresToMoveInCurrentDirection = rand() % (53) + 8; // rand()%(max-min + 1) + min;
}

// Accessor and Mutator for hitPoints
int Protester::getHitPoints() {
    return _hitPoints;
}
void Protester::setHitPoints(int hitPoints) {
    _hitPoints = hitPoints;
}

// Accessor and Mutator for isLeavingField
bool Protester::getIsLeavingField() {
    return _isLeavingField;
}

void Protester::setIsLeavingField(bool isLeavingField) {
    _isLeavingField = isLeavingField;
}

bool Protester::isWithinShoutingDistance() {
    // This may not be 100% correct yet as I'm not entirely sure what the expected functionality of this is supposed to be. Once 
    // the protester can track the iceMan I should be able to better match up the exact specifications of how 
    // this function should work while playing the finished version of the game he sent. :)

    // int iceManDirection = getWorld()->getIceMan()->getDirection();

    int iceManX = getWorld()->getIceMan()->getX();
    int iceManY = getWorld()->getIceMan()->getY();

    int xDist = abs(iceManX - getX());
    int yDist = abs(iceManY - getY());

    if (xDist <= 4 && yDist <= 4) { // If (x,y) coords are within 4 or less of IceMan.  
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

void Protester::doSomething() { // Very unfinished still.
    if (!this->getState()) // If dead, return immediately.
        return;

    else if (!isAbleToMove()) { // If in rest state, return immediately and decrement ticks left to wait.
        return;
    }

    else if (_hitPoints == 0) { // Have Protester leave the field.
    }

    else if (isWithinShoutingDistance() && isAbleToYell()) { // Shout at iceMan.
        this->getWorld()->playSound(SOUND_PROTESTER_YELL);
        return;
    }
    return;
}

Protester::~Protester() {

}
// Regular Protester Class ////////////////////////////////////////////////////////////////////////////

RegularProtester::RegularProtester(StudentWorld* swRegProtester)
    : Protester(IID_PROTESTER, 5, swRegProtester) {
    //_hitPoints = 5;
}

RegularProtester::~RegularProtester() {

}
//////Goodie Class//////////////////////////////////////////////////////////////////////////

//Goodie::Goodie(): Actor() {
//
//}