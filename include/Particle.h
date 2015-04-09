//
//  Particle.h
//  ParticleSystem
//
//  Created by tom on 2/25/14.
//
//

#pragma once


#include "cinder/gl/gl.h"
#include "cinder/vector.h"


class Particle{
public:
    Particle(const ci::Vec3f & position, float radius, float mass, float drag);
    
    void updateVerlet();
    void draw();
	void drawHistory();
    
    void moveTo(ci::Vec3f & target){
        prevPosition = position;
        ci::Vec3f positionNew = ( target - prevPosition ) * 0.95f;
        position += positionNew + forces / mass;
        forces = ci::Vec3f::zero();
    }
    void updateHistory();
    
    ci::Vec3f position;
    ci::Vec3f prevPosition;
    ci::Vec3f forces;
    float radius;
    float mass;
    float drag;
    
    float theta;
    float phi;
    
    inline float getX();
    inline float getY();
    inline float getZ();

	bool lights;
    
private:
    std::vector<ci::Vec3f> positionHistory;
    int tailLength;
    
};

float Particle::getZ()
{
    return position.z;
}

float Particle::getX()
{
    return position.x;
}

float Particle::getY()
{
    return position.y;
}