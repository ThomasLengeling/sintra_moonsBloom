//
//  ParticleSystem.cpp
//  ParticleSystem
//
//  Created by tom on 2/25/14.
//
//

#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
    
}

ParticleSystem::~ParticleSystem()
{
    for(auto it = particles.begin(); it != particles.end(); ++it){
        delete *it;
    }
    particles.clear();
}


void ParticleSystem::update()
{
    for(auto it = particles.begin(); it != particles.end(); ++it){
       (*it)->updateVerlet();
    }
    
}

void ParticleSystem::draw()
{
    
    for(auto it = particles.begin(); it != particles.end(); ++it){
        (*it)->draw();
    }

}
void ParticleSystem::drawHistory()
{
	for (auto it = particles.begin(); it != particles.end(); ++it){
		(*it)->updateHistory();
		//(*it)->drawHistory();
	}

}
void ParticleSystem::drawConnections()
{
    glBegin(GL_LINES);
    for(auto it = particles.begin(); it != particles.end(); ++it){
        Particle *pa = (*it);
        for(auto it2 = particles.begin(); it2 != particles.end(); ++it2){
            Particle *pb = (*it2);
            float d = fabs(pa->position.x - pb->position.x) + fabs(pa->position.y - pb->position.y) + fabs(pa->position.z - pb->position.z);
            if( d > 22 && d < 85){
                glVertex3f(pb->position);
                glVertex3f(pa->position);
                
            }
        }
    }
    glEnd();
}

void ParticleSystem::addParticle( Particle *particle)
{
    particles.push_back(particle);
}

void ParticleSystem::destroyParticle( Particle *particle)
{
    std::vector< Particle *>::iterator it = std::find(particles.begin(), particles.end(), particle);
    delete *it;
    particles.erase( it );
}

std::vector<Particle *>::iterator ParticleSystem::begin()
{
    return particles.begin();
}
std::vector<Particle *>::iterator ParticleSystem::end()
{
    return particles.end();
}
