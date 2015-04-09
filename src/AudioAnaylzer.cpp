//
//  AudioAnaylzer.cpp
//  ParticleSystemBloom
//
//  Created by tom on 4/19/14.
//
//

#include "AudioAnaylzer.h"



AudioAnalyzer::AudioAnalyzer(ci::audio::ChannelIdentifier _channel, uint16 _bandCount)
{
    beatSensitivity    = 0.001f;
    avgLvlOld          = 0.f;
    beatValue          = 0.0f;
    
    bandCount = _bandCount;
    channel   = _channel;
    
    levels    = new float[bandCount];
    levelsPts = new float[bandCount];
    
    for(int i = 0; i < bandCount; i++){
        levels[i]    = 0;
        levelsPts[i] = 0;
    }
}

void AudioAnalyzer::drawFft( std::shared_ptr<float> & fftRef)
{
    
    if( !fftRef ) {
        return;
    }
    float *fftBuffer = fftRef.get();
    
#ifdef DRAWFFT
    float centerMargin= 15.0f;
    gl::color( Color::black() );
    gl::drawSolidCircle(Vec2f::zero(), 5.f);
    glLineWidth(3.f);
#endif
    
    float avgLvlLeft= 0.f;
    
    for( int i= 0; i < bandCount; i++ ) {
        ci::Vec2f p = ci::Vec2f(0.f, 200.f);
        p.rotate( 2.f * M_PI * (i / (float)bandCount) );
        float lvl = fftBuffer[i] / bandCount * p.length();
        lvl = fmin(lvl, p.length());
        
        levels[i] = fmax(levels[i], lvl);
        levelsPts[i] = fmax(levelsPts[i], levels[i]);
        
#ifdef DRAWFFT
        p.limit(1.f + centerMargin + levelsLeft[i]);
        gl::drawLine(p.limited(centerMargin), p);
        glPointSize(2.f);
        glBegin(GL_POINTS);
        gl::vertex(p + p.normalized()*levelsPtsLeft[i]);
        glEnd();
        glPointSize(1.f);
#endif
        avgLvlLeft += lvl;
    }
    avgLvlLeft /= (float)bandCount;
    glLineWidth(1.f);
    
#ifdef DRAWFFT
    gl::color( ColorA(0.f,0.f,0.f, 0.1f) );
    gl::drawSolidCircle(Vec2f::zero(), 5.f + avgLvlLeft);
#endif
}



void AudioAnalyzer::beatDetection(std::shared_ptr<float> & fftRef)
{
    beatValue = 0.0f;
    if(fftRef){
        float * fftBuffer = fftRef.get();
        float avgLv1 = 0.0f;
        for(int i = 0; i < bandCount; i++){
            avgLv1 += fftBuffer[i] / (float)bandCount;
        }
        avgLv1 /= (float)bandCount;
        if(avgLv1 > avgLvlOld + beatSensitivity){
            beatValue = avgLv1 - beatSensitivity;
        }
        avgLvlOld = avgLv1;
    }
    updateLevels();
}

void AudioAnalyzer::updateLevels()
{
    for(int i = 0; i < (bandCount); i++){
        levels[i]    = fmax(0.0f, levels[i] - 1.0f);
        levelsPts[i] = fmax(0.0f, levelsPts[i] - 0.95f);
    }
}
void AudioAnalyzer::drawFftScene(std::shared_ptr<float> & fftRef)
{
#ifdef DRAWFFT
    gl::pushMatrices();
    gl::translate(ci::Vec2i(200, 800));
    gl::rotate( getElapsedSeconds() * 10.f );
    gl::scale(ci::Vec3f(4.0, 4.0, 4.0));
#endif
    drawFft(fftRef);
#ifdef DRAWFFT
    gl::popMatrices();
#endif
    
}