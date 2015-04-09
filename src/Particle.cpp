//
//  Particle.cpp
//  ParticleSystem
//
//  Created by tom on 2/25/14.
//
//

#include "Particle.h"


Particle::Particle( const ci::Vec3f & position, float radius, float mass, float drag){
    this->position = position;
    this->radius   = radius;
    this->mass     = mass;
    this->drag     = drag;
    prevPosition   = position;
    forces         = ci::Vec3f::zero();
    tailLength     = 12;
}


void Particle::updateVerlet()
{
    ci::Vec3f temp = position;
    
    //Velocity
    ci::Vec3f vel = (position - prevPosition) * drag;
    
    position += vel + forces / mass;
    
    prevPosition =  temp;
    
    forces = ci::Vec3f::zero();
}

void Particle::updateHistory()
{
    positionHistory.push_back(position);
    if(positionHistory.size() > tailLength){
        positionHistory.erase(positionHistory.begin());
    }
}

void Particle::draw()
{

    ci::gl::drawSphere(position, radius, 10);
    
}

void Particle::drawHistory()
{
    glBegin(GL_LINE_STRIP);
    int nPH = positionHistory.size();
    for(int i = 0; i < nPH;  i++){
        float alpha = (float) i / (float) nPH + 0.05;
        ci::gl::color(ci::ColorA(0.1f, 0.55f, 0.6f, alpha));
        ci::gl::vertex(positionHistory[i]);
    }
    glEnd();
}
