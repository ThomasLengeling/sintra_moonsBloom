//
//  AudioLoader.h
//  Luz_Abduct
//
//  Created by tom on 4/22/14.
//
//

#pragma once

#include "cinder/audio/Io.h"
#include "cinder/audio/Output.h"
#include "cinder/audio/FftProcessor.h"


class AudioLoader{
public:
    AudioLoader(int _bandCount){
        //AUDIO
        try{
            mTrack = ci::audio::Output::addTrack(ci::audio::load(ci::app::loadResource(AUDIO)));
            mTrack->setTime(0);
            mTrack->enablePcmBuffering(true);
            mTrack->stop();
        }catch( ... ){
            ci::app::console()<<"error loding sound"<<std::endl;
        }
        
        bandCount = _bandCount;
    }
    
    ~AudioLoader(){
        mTrack->~Track();
    }
    
    std::shared_ptr<float>  getFFT(ci::audio::ChannelIdentifier channel){
        mPcmBuffer = mTrack->getPcmBuffer();
        if(mPcmBuffer){
            std::shared_ptr<float>  vec = ci::audio::calculateFft(mPcmBuffer->getChannelData(channel), bandCount);
            return vec;
        }
        return NULL;
    }
    
    int      getBandCont(){return bandCount;}
    float       getTime(){ return mTrack->getTime();}
    void        play(){mTrack->play();}
    void        stop(){mTrack->stop();}
    void        volumen(float v){mTrack->setVolume(v);}
    
private:
    ci::audio::TrackRef          mTrack;
    ci::audio::PcmBuffer32fRef   mPcmBuffer;
    uint16                       bandCount;
};
