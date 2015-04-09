#pragma once

#include "ParticleSystemReactive.H"
#include "AudioAnaylzer.h"
#include "Bloom.h"

class ParticleEmitters : public Bloom{

public:
    
    ParticleEmitters(int numSphere, int numParticles, const ci::Vec2f & dims);
    
    void updateParticles();
    
    //DRAW FUNTIONS
    void drawParticles();
    void drawConnections();
    void drawShadedParticles();
    bool draw();
    
    //FBOs
    void drawFbo();
    void drawFboTextures();
    
    //BLOOM
    void updateBloom();
    void drawBloom();
    
    float getNumSpheres(){ return numEmitters;}
    
    void switchBackgrounds();
    void switchBackgrounds(int i);
    
    void bindFbosTextures();
    void unbindFbosTextures();
    
    void setBackgroundColor(float bkg);
    void setBackgroundColor(int index, float bkg);
    
    void setAlphaVal(float bkg);
    void setAlphaVal(int index, float bkg);
    
    void clean();
    
    void setBlackLight();
    void setWhiteLight();
    
    void setBlackLight(int index);
    void setWhiteLight(int index);
    
    void activateColorParticles();
    
    //DETECTOR
    void setAmp(int i, float amp);
    void setFreq(int i, float freq);
    void setOnset(int i, bool on);
    
    void setBeatValue(int i, float val);
    
protected:
    ParticleSystemReactive  **mParticlesEmitters;
    
    int numParticles;
    int numEmitters;
};