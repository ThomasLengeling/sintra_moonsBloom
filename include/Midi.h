void MoonsApp::setupMIDI()
{
	if (mMidiIn.getNumPorts() > 0){
		mMidiIn.listPorts();
		mMidiIn.openPort(0);
		console() << "Opening MIDI port 0" << std::endl;
	}
	else {
		console() << "No MIDI Ports found!!!!" << std::endl;
	}
}



void MoonsApp::updateMIDI(){
	while (mMidiIn.hasWaitingMessages()) {
		ci::midi::Message message;
		mMidiIn.getNextMessage(&message);

		processMidiMessage(&message);
	}

}

void MoonsApp::processMidiMessage(ci::midi::Message* message){
	//	console() << "midi port: " << message->port << " ch: " << message->channel << " status: " << message->status;
	//	console() << " byteOne: " << message->byteOne << " byteTwo: " << message->byteTwo << std::endl;

	switch (message->status) {
	case MIDI_CONTROL_CHANGE:

		switch (message->byteOne){
            case 1:
                indexEmitter = (int)ci::lmap<float>(message->byteTwo, 0.0, 127.0, 0.0, NUMSPHERE - 1.0);
                ci::app::console()<<"Emitter index: "<<indexEmitter<<std::endl;
                break;
            case 7:
                alphaBackground = ci::lmap<float>(message->byteTwo, 0.0, 127.0, 0.0, 1.0);
                mParticleEmitters->setAlphaVal(alphaBackground);
                break;
            case 8:
                masterBackground = ci::lmap<float>(message->byteTwo, 0.0, 127.0, 0.0, 1.0);
                mParticleEmitters->setBackgroundColor(masterBackground);
                break;

            case 16:
                attenuationBloom= ci::lmap<float>(message->byteTwo, 0.0, 127.0, 0.0, 2.2);
                break;
            case 17:
                 alphaBackground = ci::lmap<float>(message->byteTwo, 0.0, 127.0, 0.0, 1.0);
                break;
            case 18:
                grayBackground = ci::lmap<float>(message->byteTwo, 0.0, 127.0, 0.0, 1.0);
                mParticleEmitters->setBackgroundColor(indexEmitter, grayBackground);
                break;
                
            //BUTTONS
            case 21:
                mParticleEmitters->setBlackLight();
                attenuationBloom = 0.8;
                break;
            case 31:
                mParticleEmitters->setWhiteLight();
                attenuationBloom = 0.0;
                mParticleEmitters->setAlphaVal(0.2);
                break;
            case 41:
                mParticleEmitters->activateColorParticles();
                break;
            
            case 22:
                mParticleEmitters->setBlackLight(indexEmitter);
                break;
            case 32:
                mParticleEmitters->setWhiteLight(indexEmitter);
                break;
            case 42:
                attenuationBloom = 0.0;
                mParticleEmitters->setAlphaVal(0.2);
                break;
		}
		break;
	}
}