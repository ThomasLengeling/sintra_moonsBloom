#include "ParticleEmitters.h"


ParticleEmitters::ParticleEmitters(int numSphere, int numParticles, const ci::Vec2f & dims) : Bloom()
{
    mParticlesEmitters = new ParticleSystemReactive*[numSphere];
    this->numParticles  = numParticles;
    this->numEmitters   = numSphere;
    float wx    = dims.x / (float)numSphere;
    float wy    = dims.y;
    
    for(int i = 0; i < numEmitters; i++){
      //  ci::Vec3f center = ci::Vec3f(  wx/2.0 + wx*(i), wy/2.0, 0);
        ci::Vec3f center = ci::Vec3f(wx/2.0, wy/2.0, 0);
        float    radius  = 210;
        ci::Vec2f area   = ci::Vec2f(wx, wy);
        mParticlesEmitters[i] = new ParticleSystemReactive(numParticles, area);//dims);
        
        mParticlesEmitters[i]->setCenter(center);
        mParticlesEmitters[i]->setRadius(radius);
        mParticlesEmitters[i]->setInitPos(ci::Vec2i( wx*(i), 0));
        mParticlesEmitters[i]->createParticles(numParticles);
        mParticlesEmitters[i]->setBackgroundGray(0.5);//ci::randFloat(1.0));
        mParticlesEmitters[i]->setWindowBounds(ci::Area(0, 0, dims.x, dims.y));
        ci::app::console()<<mParticlesEmitters[i]->getBeatForce()<<std::endl;
        ci::app::console()<<"x: "<<wx<<" y: "<<wy<<std::endl;
        ci::app::console()<<"xa: "<<dims.x<<" ay: "<<dims.y<<std::endl;
        ci::app::console()<<"xc: "<<center.x<<" cy: "<<center.y<<std::endl;
    }
    
    Bloom::setupBlur(ci::Vec2f(dims.x, dims.y));
}


void ParticleEmitters::clean()
{
    if(mParticlesEmitters != NULL){
        for (int i= 0; i<numEmitters; i++){
            if(mParticlesEmitters[i] != NULL)
                delete mParticlesEmitters[i];
        }

        delete[] mParticlesEmitters;
    }
}

////-----------------DRAW RELATED FUNTIONS------------------------------------------
bool ParticleEmitters::draw()
{
    for(int i = 0; i < numEmitters; i++){
        mParticlesEmitters[i]->preRender();
        mParticlesEmitters[i]->background();
        ci::gl::color(1.0, 1.0, 1.0);
        mParticlesEmitters[i]->drawParticles();
        ci::gl::color(0.0, 0.5, 1.6);
        mParticlesEmitters[i]->drawConnections();
        mParticlesEmitters[i]->drawHistory();
        mParticlesEmitters[i]->posRender();
        
       // mParticlesEmitters[i]->drawFbo();
    }
    return true;
}

void ParticleEmitters::drawParticles()
{
    ci::gl::color(1.0, 1.0, 1.0);
    for(int i = 0; i < numEmitters; i++){
        mParticlesEmitters[i]->background();
        mParticlesEmitters[i]->drawParticles();
    }
}

void ParticleEmitters::drawConnections()
{
    ci::gl::color(0.0, 0.5, 1.6);
    for(int i = 0; i < numEmitters; i++){
        mParticlesEmitters[i]->drawConnections();
    }
}

void ParticleEmitters::updateParticles()
{
    for(int i = 0; i < numEmitters; i++){
        mParticlesEmitters[i]->updateParticleSystems();
    }
    
}

void ParticleEmitters::drawFbo()
{
    for(int i =0; i < numEmitters; i++){
        mParticlesEmitters[i]->drawFbo();
    }
}

void ParticleEmitters::drawFboTextures()
{
    for(int i =0; i < numEmitters; i++){
        mParticlesEmitters[i]->drawFboTexture();
    }
}

//----------BLOOM--------------------------------------
void ParticleEmitters::updateBloom()
{
    Bloom::bindFboScene();
    drawFboTextures();
    Bloom::unbindFboScene();
    
    Bloom::updateBlur();
}

void ParticleEmitters::drawBloom()
{
    Bloom::drawBlur();
}

//-----USEFULL FUNCTIONS--------------------------------
void ParticleEmitters::bindFbosTextures()
{
    for(int i =0; i < numEmitters; i++){
        mParticlesEmitters[i]->bindFboTexture();
    }
}

void ParticleEmitters::unbindFbosTextures()
{
    for(int i =0; i < numEmitters; i++){
        mParticlesEmitters[i]->unbindFboTexture();
    }
}

void ParticleEmitters::setBackgroundColor(float bkg)
{
    for(int i =0; i < numEmitters; i++){
        mParticlesEmitters[i]->setBackgroundGray(bkg);
    }
}

void ParticleEmitters::setBackgroundColor(int index, float bkg)
{
    mParticlesEmitters[index]->setBackgroundGray(bkg);
}

void ParticleEmitters::setAlphaVal(float bkg)
{
    for(int i =0; i < numEmitters; i++){
        mParticlesEmitters[i]->setAlphaVal(bkg);
    }
}

void ParticleEmitters::setAlphaVal(int index, float bkg)
{
      mParticlesEmitters[index]->setAlphaVal(bkg);
}


void ParticleEmitters::activateColorParticles()
{
    for(int i = 0; i < numEmitters; i++){
        mParticlesEmitters[i]->activateColorParticles();
    }
}

//SWITCH EVENTS
void ParticleEmitters::setBlackLight()
{
    for(int i = 0; i < numEmitters; i++){
        mParticlesEmitters[i]->setBlackLights();
    }
}

void ParticleEmitters::setWhiteLight()
{
    for(int i = 0; i < numEmitters; i++){
        mParticlesEmitters[i]->setWhiteLights();
    }
}

void ParticleEmitters::setBlackLight(int index)
{
    mParticlesEmitters[index]->setBlackLights();
}

void ParticleEmitters::setWhiteLight(int index)
{
    mParticlesEmitters[index]->setWhiteLights();
}

void ParticleEmitters::switchBackgrounds()
{
    
}

void ParticleEmitters::switchBackgrounds(int index)
{
    ParticleSystemReactive  *particle = mParticlesEmitters[index];
    bool contrast = particle->getContrastBool();
    contrast = !contrast;
    particle->activateContraste(contrast);
}

//----DETECTOR
void ParticleEmitters::setAmp(int i, float amp)
{
    mParticlesEmitters[i]->setAmp(amp);
}

void ParticleEmitters::setFreq(int i, float freq)
{
    mParticlesEmitters[i]->setFreq(freq);
}

void ParticleEmitters::setOnset(int i, bool on)
{
    mParticlesEmitters[i]->setOnset(on);
}

void ParticleEmitters::setBeatValue(int i, float val)
{
    mParticlesEmitters[i]->setBeatValue(val);

}