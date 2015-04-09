//
//  Header.h
//  Luz_Abduct
//
//  Created by tom on 4/22/14.
//
//
#pragma once

#include "ParticleSystem.h"
#include "cinder/app/App.h"
#include "cinder/Rand.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/gl.h"

#define BANDCOUNT 8

class ParticleSystemReactive : public ParticleSystem{
public:

	ParticleSystemReactive(int numParticles, const ci::Vec2f & rectSize);
	~ParticleSystemReactive();
    
	void        createParticles(int numParticles);
	void        particleBeatForce();

	void        updateParticleSystems();

    void        background();
	void        drawConnections();
	void		drawParticles();
	void		drawHistory();
    
    void        preRender();
    void        posRender();
    
	void        setAttrFactor(const float & a){ attrFactor = a;}
	void        setForceIncrement(float a){ forceIncrement = a; }
    void        setBeatForce(const float & bf){ beatForce = bf;}
    void        setBeatValue(float  bf){ beatValue = bf;}
    void        setFftLevels(float * values){fftLevels = values;}
    void        setCenter(const ci::Vec3f & c){center = c;}
    void        setRadius(float r){radius = r;}
    void        setAttrPosition(ci::Vec3f & attr){attrPosition = attr;}
    void        setInitPos(const ci::Vec2f & pos){intiPos = pos;}
    void        setBackgroundGray(float g){grayBackground =g;}
    void        setAlphaVal(float a){alphaVal =a;}
    void        setWindowBounds(const ci::Area & a){windowBounds = a;}
    void        activateColorParticles(){activateColors = !activateColors;}
    
	float	  &	getBeatForce(){return beatForce;}
    float     & getBeatValue(){return beatValue;}
    ci::Vec3f & getAttrPosition(){return attrPosition;}
    
	void		activateLights();
	void        desactivateLights();
    
    void        rotateScene();
    void        setupLights();
    
    void        setBlackLights();
    void        setWhiteLights();
    
    void        toggleContrast();
    void        activateContraste(bool val);
    bool        getContrastBool(){return mContrast;}
    
    //TEXTURES
    void        bindFboTexture(){mFboParticle.bindTexture();}
    void        unbindFboTexture(){mFboParticle.unbindTexture();}
    
    void        drawFboTexture();
    void        drawFbo();
    
    //MIDETECTOR
    void        setFreq(float freq){mFreq = freq;}
    void        setAmp(float amp){mAmp = amp;}
    void        setOnset(bool on){onSet = on;}
    
private:
    
    bool        activateColors;
    ci::ColorA  colorLines;
    ci::ColorA  colorSphere;
    
    ci::Area    windowBounds;
    ci::Vec2f   intiPos;
    ci::Vec3f   center;
	float       grayBackground;
    float       alphaVal;
    
	float       distanceCamera;
    
    ci::Vec3f   attrPosition;
    
    float       radius;
    float       forceIncrement;
    float       attrFactor;

    float       particleRadius;
    float       randAngle;
    
    float       beatForce;
    float       beatValue;
    
    float       *fftLevels;
    
    float       padding;
    
    bool        mContrast;
    bool        mToogleContrast;
    bool        mLightTogle;
    
    
    float       mFreq;
    float       mAmp;
    bool        onSet;
    
    ci::ColorA  colorDifuse;
	ci::ColorA  colorAmbient;
	ci::ColorA  colorSpecular;
	ci::ColorA  colorEmission;
	ci::ColorA  modelAmbient;
    
    GLfloat *   light_position;
	GLfloat *   shininess_mat;
    
    float       rotation;
	float       rotationInc;
	float       rotationMax;


    ci::gl::Fbo mFboParticle;
};