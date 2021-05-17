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

//////Goodie Class//////////////////////////////////////////////////////////////////////////

//Goodie::Goodie(): Actor() {
//
//}