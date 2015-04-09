//
//  ParticleSystemReactive.cpp
//  Luz_Abduct
//
//  Created by tom on 4/22/14.
//
//

#include "ParticleSystemReactive.h"


ParticleSystemReactive::ParticleSystemReactive(int num, const ci::Vec2f & dims) : ParticleSystem()
{
    ParticleSystem::clear();
    particleRadius  = 132;
    attrFactor      = 0.34;
    forceIncrement  = 0.000016;
    beatForce       = 250;
    attrPosition    = ci::Vec3f(0, 0, 0);
    
    rotationMax     = 0;
    rotationInc     = 1;
    
    alphaVal        = 0.4;
    grayBackground  = 0.0;
    
    activateColors  =  false;
    colorLines      =  ci::ColorA(0.0, 0.0, 0.0, 1.0);
	colorSphere     =  ci::ColorA(0.0, 0.0, 0.0, 1.0);
    
    fftLevels       =  NULL;
    
    ci::gl::Fbo::Format format;
	///format.enableMipmapping(true);
	//format.setWrap(GL_CLAMP_TO_BORDER_ARB, GL_CLAMP_TO_BORDER_ARB);
    
	format.setCoverageSamples(16);
	format.setSamples(8);
    
    mFboParticle        = ci::gl::Fbo(dims.x, dims.y, format);
    
    light_position    = new GLfloat[4];
    shininess_mat     = new GLfloat[1];
    light_position[0] =  0;
    light_position[1] =  0;
    light_position[2] =  0;
    light_position[3] = -1;
    shininess_mat[0]  = 50;
    
    attrPosition      = ci::Vec3f(0.0, 0.0, 0.0);
    
    padding =  15;

    setupLights();
    
    mContrast = false;
    
    mAmp   = 0.0;
    mFreq  = 0.0;
    onSet  = false;
}


ParticleSystemReactive::~ParticleSystemReactive()
{
    if(light_position != NULL)
        delete light_position;
    
    if(shininess_mat != NULL)
        delete shininess_mat;
    
    if(fftLevels != NULL)
        delete fftLevels;
    
    ParticleSystem::clear();
    
}

void ParticleSystemReactive::background()
{
    
}

void ParticleSystemReactive::activateContraste(bool val)
{
    mToogleContrast = true;
    mContrast       = val;
}

void ParticleSystemReactive::toggleContrast()
{
    if(mToogleContrast){
        if(mContrast){
            grayBackground = 1.0;
            setWhiteLights();
        }else{
            grayBackground = 0.0;
            setBlackLights();
        }
        mToogleContrast = false;
    }
}

void ParticleSystemReactive::preRender()
{
    toggleContrast();
    
    mFboParticle.bindFramebuffer();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    ci::gl::pushModelView();
  
    ci::gl::setViewport(windowBounds);//mFboParticle.getBounds());
    ci::gl::enableAlphaBlending();
    
    ci::gl::color(0, 0, 0, 1.0);
    ci::gl::drawSolidRect(ci::Area(0, 0, mFboParticle.getWidth(), mFboParticle.getHeight() ));

    
    ci::gl::color(grayBackground, grayBackground, grayBackground, alphaVal);
    ci::gl::drawSolidRect(ci::Area(padding, padding, mFboParticle.getWidth() - padding, mFboParticle.getHeight() - padding ));
    
    ci::gl::pushMatrices();
    ci::gl::translate(center);
        
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorDifuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, colorAmbient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess_mat);
    glMaterialfv(GL_FRONT, GL_EMISSION, colorEmission);
    
    rotateScene();
}

void ParticleSystemReactive::drawFbo()
{
    ci::gl::pushMatrices();
    ci::gl::translate(intiPos);
    ci::gl::draw(mFboParticle.getTexture(), mFboParticle.getBounds());
    ci::gl::popMatrices();
}

void ParticleSystemReactive::drawFboTexture()
{
    ci::gl::pushMatrices();
    ci::gl::translate(intiPos);
    mFboParticle.getTexture().enableAndBind();
    ci::gl::setViewport(windowBounds);
    //ci::gl::color(ci::Color::black());
     ci::gl::color(1.0, 1.0, 1.0, 1.0);
    ci::gl::drawSolidRect(mFboParticle.getBounds());
    mFboParticle.unbindTexture();
    ci::gl::popMatrices();
}

void ParticleSystemReactive::posRender()
{
    ci::gl::disableAlphaBlending();
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    ci::gl::popMatrices();
    ci::gl::pushModelView();
    mFboParticle.unbindFramebuffer();
}

void ParticleSystemReactive::updateParticleSystems()
{
    particleBeatForce();
    ParticleSystem::update();
}

void ParticleSystemReactive::drawConnections()
{
    if(activateColors)
        ci::gl::color(colorLines);
    ci::gl::lineWidth(1.0 + mAmp*20);
    ParticleSystem::drawConnections();
}

void ParticleSystemReactive::drawParticles()
{
    if(activateColors)
        ci::gl::color(colorSphere);
    ParticleSystem::draw();
}


void ParticleSystemReactive::drawHistory()
{
    ParticleSystem::drawHistory();
}

void ParticleSystemReactive::setupLights()
{
    colorDifuse       = ci::ColorA(0.9, 0.9, 0.9, 1.0);
    colorAmbient      = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
    colorSpecular     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
    colorEmission     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
}

void ParticleSystemReactive::setWhiteLights()
{
    colorDifuse       = ci::ColorA(0.0, 0.0, 0.0, 1.0);
    colorAmbient      = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
    colorSpecular     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
    colorEmission     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
    
    colorLines        = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
    colorSphere       = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
    
    grayBackground  = 1.0;
}

void ParticleSystemReactive::setBlackLights()
{
    colorDifuse       = ci::ColorA(1.0, 1.0, 1.0, 1.0);
    colorAmbient      = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
    colorSpecular     = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
    colorEmission     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
    
    colorLines        = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
    colorSphere       = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
    
    grayBackground  = 0.0;
}


void ParticleSystemReactive::createParticles(int numParticles)
{
    ParticleSystem::clear();
    int particleNUMThird = (numParticles * 2)/3.0;
    for(int i = 0; i < numParticles; i++){
        float x = ci::randFloat(-radius, radius);
        float y = ci::randFloat(-radius, radius);
        float z = ci::randFloat(-radius, radius);
        float radius = ci::randFloat(2.4f, 7.5f);
        float mass   = radius * 8.0;
        if(i > particleNUMThird){
            radius = 2.3f;
            mass   = 25.f;
        }
        float drag   = 0.94f;
        Particle * particle = new Particle(ci::Vec3f(x, y, z), radius, mass, drag);
        ParticleSystem::addParticle(particle);
    }
    ci::app::console()<<ParticleSystem::size()<<" NEW PARTICLE SIZE"<<std::endl;
}


void ParticleSystemReactive::rotateScene()
{
    float bVal = beatValue;
    if( bVal > 0.085){
        if( rotationMax < rotationInc){
            rotation    =  bVal / 30.0;
            rotationMax =  bVal * 10.0;
            rotationInc = 0;
        }
    }
    
    float r = ci::app::getElapsedSeconds() * (1.0f + rotationInc);
    ci::gl::rotate(ci::Vec3f::one() * r); //*r
    
    if( rotationMax > rotationInc)
        rotationInc+=rotation;
}

void ParticleSystemReactive::particleBeatForce()
{
    for(auto it = ParticleSystem::begin(); it != ParticleSystem::end(); ++it){
        ci::Vec3f attrForce = attrPosition - (*it)->position;
        attrForce *= attrFactor;
        if(attrPosition.distance((*it)->position) < radius){
            attrForce = (*it)->position - attrPosition;
            attrForce *= 0.1f + beatValue; //0.2f
        }
        
        (*it)->forces += attrForce;
        ci::Vec3f beatForceVec = (*it)->position - attrPosition;
        beatForceVec.normalize();
        beatForceVec.rotate(ci::randVec3f(), randAngle);
		beatForceVec *= (beatValue + 0.01) * ci::randFloat(beatForce*0.01, beatForce * 3);
		(*it)->forces += beatForceVec;// + //ci::randVec3f()*(fftLevels);
        
        for(auto it2 = ParticleSystem::begin(); it2 != ParticleSystem::end(); ++it2){
            (*it)->forces += ((*it)->position - (*it2)->position ) * forceIncrement * 0.5;
        }
    }
    
    randAngle = ci::lmap<float>(beatValue, 0.0, 0.8, 0, 360);
}