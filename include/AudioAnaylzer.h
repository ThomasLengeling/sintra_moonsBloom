#pragma once

#include "Resources.h"
#include "cinder/audio/Io.h"
#include "AudioLoader.h"

//#define DRAWFFT


class AudioAnalyzer{
public:
    
    AudioAnalyzer(ci::audio::ChannelIdentifier _channel, uint16 _bandCount);
    ~AudioAnalyzer(){ }
    
    void     updateLevels();
    void     beatDetection( std::shared_ptr<float> & fftRef);
    void     drawFftScene( std::shared_ptr<float> & fftRef);
    
    inline float &  getBeatValue();
    inline float * getLevels();

private:
    void     drawFft( std::shared_ptr<float> & fftRef);
    
    ci::audio::ChannelIdentifier   channel;
    
    //AUDI
    float                   beatSensitivity;
    
    float                  * levels;
    float                  * levelsPts;
    
    float                   avgLvlOld;
    float                   beatValue;
    uint16                  bandCount;
};


float * AudioAnalyzer::getLevels()
{
    return levels;
}


float & AudioAnalyzer::getBeatValue()
{
    return beatValue;
}