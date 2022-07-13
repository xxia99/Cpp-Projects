/*
Author: Xin Xia
Class: ECE 6122
Last Date Modified: 12/4/2021
Description: This is the cpp file defines the ECE_UAV's member functions and the external function threadFunction.
*/
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <eigen3/Eigen/StdVector>
#include "ECE_UAV.h"

using namespace std;
using namespace chrono;
using namespace Eigen;

void threadFunction(ECE_UAV* pUAV)
{
    float uavAcceleration;  // This is the acceleration along the flying direction
    float angleXY; // This angle refers to the horizontal direction, between X and Y
    float angelZ; // // This angle refers to the vertical direction, between Z and XY
    // Set the initial acceleration and angles for the UAV, depending on its position
    pUAV->setUAVPosition(&uavAcceleration, &angleXY, &angelZ);
    do
    {
        if(pUAV->onGround())  // The UAVs remain on the ground for 5 seconds after the beginning of the simulation
        {
            if(duration_cast<duration<float>>(high_resolution_clock::now()-pUAV->timer).count()>=5.0)
            {
                pUAV->takeOff();  // The UAVs take off after 5 seconds
            }
        }
        else if(pUAV->apMiddleTop()) // If the UAV has approached the middle top point (0,0,50)
        {
            if (duration_cast<duration<float>>(high_resolution_clock::now()-pUAV->timer).count()>=60)
            {  // If all the UAVs have come within 10 m of the point and the UAVs have flown along the surface for 60 seconds,
                pUAV->endDemo();  // the simulation will be shutdown
            }
            else
            {
                pUAV->flyonSphere();  // The UAV starts to fly on a virtual sphere
            }
        }
        else  // If the UAV has not approached the middle top point (0,0,50)
        {   // Keep tracing the UAVs
            pUAV->fly2MiddleTop(angelZ, angleXY, uavAcceleration);
            if(pUAV->phrase1End())  // Check if Phase 1 finishes
            {
                pUAV->approached();
            }
        }
        // The threadFunction updates the kinematic information every 10 msec.
        this_thread::sleep_for(chrono::milliseconds(10));
    }  // After demo start, keep doing the simulation
    while(pUAV->startDemo())
}

ECE_UAV::ECE_UAV(const float uavX, const float uavY)
{
    // The initial position of the UAV, where 0-x, 1-y, 2-z
    uavOrigin[0] = uavX;
    uavOrigin[1] = uavY;
    uavOrigin[2] = 0;
    uavLocation = uavOrigin;

    // Set the initial velocity and acceleration for the UAV
    for (int i=0;i<=2;i++)
    {
        velocity[i] = 0;  // The initial velocity of the UAV, where 0-x, 1-y, 2-z
        acceleration = 0; // The initial acceleration of the UAV, where 0-x, 1-y, 2-z
    }
    acceleration[2] = -10;
}

int ECE_UAV::startDemo()
{
    return this->demoStart;
}

void ECE_UAV::endDemo()
{
    this->demoStart = 0;
}

int ECE_UAV::onGround()
{
    return this->stay;
}

void ECE_UAV::takeOff()
{
    this->stay = 0;
}

int ECE_UAV::apMiddleTop()
{
    return this->check;
}

void ECE_UAV::approached()
{
    this->check = 1;
}

// Set the locations of UAV on the ground
void ECE_UAV::setUAVPosition(float *uavAcceleration, float *angleXY, float *angelZ)
{
    // The UAV at the middle of the field
    if(!uavOrigin[0] && !uavOrigin[1])
    {
        *uavAcceleration = 10;  // The acceleration of the UAV at original point a = 20 - 10
        *angleXY = 1.57;
        *angelZ = 1.57;
    }
    else
    {
        if (uavOrigin[1] !=0)
        {
            *angleXY = atan(abs(uavOrigin[0]) / abs(uavOrigin[1]));
        }
        else
        {
            *angleXY = 1.57;
        }
        *angelZ = atan(abs(50 - uavOrigin[2])/sqrt(uavOrigin[0]*uavOrigin[0]+ uavOrigin[1]*uavOrigin[1]));
        *uavAcceleration = 20.0 * sin(acos(0.5))/(1.0 * cos(*angelZ));
    }
}
// Phrase 1: After the initial 5 seconds the UAVs then launch from the ground and go towards
// the point (0, 0, 50 m) above the ground with a maximum velocity of 2 m/s
void ECE_UAV::fly2MiddleTop(const float &angelZ, const float &angleXY, float &uavAcceleration)
{
    // Time since the UAVs launch for the original places they stayed at
    float flyTime = duration_cast<duration<float>>(high_resolution_clock::now()-this->timer).count() - 5;
    // The square of absolute value of the velocity vector, used to control the velocity of UAVs under 2 m/s
    float sqVelocity = 0;
    // The vector of acceleration in 3 dimensions
    // Calculate them separately
    acceleration[0] = uavAcceleration * cos(angelZ) * sin(angleXY);
    acceleration[1] = uavAcceleration * cos(angelZ) * cos(angleXY);
    acceleration[2] = uavAcceleration * sin(angelZ);
    for(int i = 0;i <= 2; i++)
    {
        // Control the direction of acceleration, ensuring the UAVs flying towards the middle top point
        if(uavOrigin[i] > 0)
        {
            acceleration[i] = -1 * acceleration[i]
        }
        velocity[i] += acceleration[i] * 0.01;
        sqVelocity += velocity[i] * velocity[i]  // The square of absolute value of the velocity vector
    }
    for(int i = 0;i <= 2; i++)
    {
        if(sqVelocity > 4)  // If faster than speed limit, keep the velocity
        {
            uavAcceleration = 0;
            uavLocation[i] += velocity[i] * 0.01;  // Compute UAV's location
            velocity[i] = velocity[i]/2  // Constrain the speed
        }
        else
        {
            uavLocation[i] = uavOrigin[i] + 0.5f * acceleration[i] * flyTime * flyTime;  // Compute UAV's location
        }
    }
}
// Phrase 1 end when the UAV reach the middle top point (0,0,50)
int ECE_UAV::phrase1End()
{
    if(abs(uavLocation[0])<0.0001 && abs(uavLocation[1])<0.0001 && abs(uavLocation[2]-50)<0.0001)
        return 1;
    else
        return 0;
}
// Phrase 2: As the UAVs approach the point, (0, 0, 50), they began to fly in random paths along the
// surface of a virtual sphere of radius 10 m while attempting to maintain a speed between 2 to 10 m/s.
void ECE_UAV::flyonSphere()
{
    float D;  // The distance from the UAV to the sphere center
    float Fs; // The virtual spring between the surface of the sphere and the radial distance of the UAV from the center of the sphere
    float normSum = 0; // This is a scale used to calculate the normalized uavLocation
    float sqVelocity = 0;
    Vector3f Force; // The vector of force motivates the UAV
    random_device rdEngine;
    uniform_real_distribution<float> uniDistribution(-1, 1);
    // Calculate distance from UAV to the sphere's center
    D = sqrt(uavLocation[0] * uavLocation[0] + uavLocation[1] * uavLocation[1] + (uavLocation[2] - 50) * (uavLocation[2] - 50))

    if (!abs(uavLocation[0]) && !abs(uavLocation[1]) && !abs(uavLocation[2]-50))
    {   // Randomly choose a path if the UAV is at the middle top point
        for(int i = 0;i < 2; i++)
        {
            uavLocation[i] = uniDistribution(rdEngine);
            normSum += uavLocation[i] * uavLocation[i];
        }
        uavLocation[2] = uniDistribution(rdEngine);
        normSum += (uavLocation[2]-50) * (uavLocation[2]-50);
        // Calculate the normalized uavLocation
        for(int i = 0;i <= 2; i++)
        {
            uavLocation[i] = uavLocation[i] / sqrt(normSum);
        }
    }
    else
    {
        for (int i = 0;i <= 2; i++)
        {
            uavLocation[i] = uavLocation[i] / D;
        }
    }
    // Use a variation of Hooke’s Law simulating a virtual spring between the surface
    // of the sphere and the radial distance of the UAV from the center of the sphere
    Fs = 10 * (10 - D) - sqrt(velocity[0]*velocity[0] +  velocity[1]*velocity[1] + velocity[2]*velocity[2]);
    for (int i = 0;i <= 2; i++)
    {
        Force[i] = Fs * uavLocation[i];
    }
    Force[2] += 10;

    // Constrain the Fs under 20 N
    if (Force[0]*Force[0]+Force[1]*Force[1]+Force[2]*Force[2] > 400)
    {
        for (int i = 0;i <= 2; i++)
        {
            Force[i] = Force[i] / sqrt(Force[0]*Force[0]+Force[1]*Force[1]+Force[2]*Force[2]) * 20;
        }
        Force[2] = Force[2] - 10;
    }
    // Compute the acceleration along the Force of the UAV
    acceleration  = Force;
    for (int i = 0;i <= 2; i++)
    {
        // Compute the velocity of the UAV
        velocity[i] += acceleration[i] * 0.01;
        // Compute the square velocity of the UAV to check if in 2~10 m/s
        sqVelocity += velocity[i] * velocity[i]
        uavLocation[i] += velocity[i] * 0.01;
    }
    if (sqVelocity>= 100 || sqVelocity <= 4)
    {
        for (int i = 0;i <= 2; i++)
        {
            // Constrain the velocity of the UAV
            velocity[i] = 2;
            uavLocation[i] += velocity[i] * 0.01;
        }
    }
}
// A member function start() causes the thread member variable to run
void ECE_UAV::start()
{
    thread uThread(threadFunction,this);
    uavThread = move(uThread);
}
// A member function joins thread member variable
void ECE_UAV::join()
{
    uavThread.join();
}
// A function helps to return the dynamic location of the UAVs
Vector3f* ECE_UAV::findUAV()
{
    return &uavLocation;
}

