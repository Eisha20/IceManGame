#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject { // "Base" Class
public:
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth,
        StudentWorld* swActor);

    virtual void doSomething() = 0; //pure virtual function
    void setState(bool state);      
    bool getState();
    virtual StudentWorld* getWorld();
    virtual ~Actor();

private:
    bool _state;//Actor is alive or dead
    StudentWorld *_swActor;
};


/*************************************************************************************************/
class IceMan : public Actor {
public:
    IceMan(StudentWorld* swIceMan);
    void doSomething();

    int getHealth();
    void decHealth(int health);
    int getSonar();
    void setSonar(int sonar);
    int getWater();
    void setWater(int water);
    int getGoldNuggets();
    void setGoldNuggets(int goldNuggets);
    int getBarrels();
    void setBarrels(int barrelsLeft);
    virtual ~IceMan();

private:

    int _health;
    int _water;
    int _sonar;
    int _goldNuggets;
    int _barrelsLeft;
};

/**************************************************************************************************/
class Ice : public Actor {
public:
    Ice(int startX, int startY, StudentWorld* swIce);
    void doSomething();

    virtual ~Ice();

private:

};

/******************************************PROTESTER HIERARCHY*****************************/

//class Protester : public Actor {
//public:
//    Protester();
//
//  virtual ~Protester();
//private:
//
//};

//class RegularProtester : public Protester {
//public:
//    RegularProtester();
//
//  virtual ~RegularProtester();
//private:
//
//};
//class HardcoreProtester : public Protester {
//public:
//    HardcoreProtester();
//
//    virtual ~HardcoreProtester();
//private:
//
//};
///*******************************************************************************************/
//
///******************************************GOODIES HIERARCHY****************************/
//
//class Goodie : public Actor {
//public:
//    Goodie();
//
//  virtual ~Goodie();
//private:
//
//};

//class OilBarrel : public Goodies {
//public:
//    OilBarrel();
//  virtual ~OilBarrel();
//private:
//};
//
//class Boulder: public Goodies {
//public:
//    Boulder();
//
//  virtual ~Boulder();
//private:
//
//};
//
//class Sonar : public Goodies {
//public:
//    Sonar();
//
//  virtual ~Sonar();
//private:
//
//};
//
//class GoldNugget : public Goodies {
//public: 
//    GoldNugget();
//    virtual ~GoldNugget();
//private:
//
//};

#endif // ACTOR_H_
