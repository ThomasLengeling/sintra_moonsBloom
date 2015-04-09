//
//  ParticleSystem.h
//  ParticleSystem
//
//  Created by tom on 2/25/14.
//
//

#pragma once

#include "Particle.h"
#include <vector>

class ParticleSystem{
public:
    ParticleSystem();
    virtual ~ParticleSystem();
    
    void update();
    void draw();
    void drawHistory();
    void drawConnections();
    
    void addParticle(Particle *particle);
    void destroyParticle(Particle *particle);
    
    void clear(){particles.clear();}
    
    void particleMoveTo(unsigned int i, ci::Vec3f & dst) { particles[i]->moveTo(dst);}
    
    int size(){return particles.size();}
    
    inline std::vector<Particle *> getVectorPoints();
    
    std::vector<Particle *>::iterator  begin();
    std::vector<Particle *>::iterator  end();
    
private:
    
    std::vector<Particle *> particles;
    
};

std::vector<Particle *>  ParticleSystem::getVectorPoints()
{
    return particles;
}

