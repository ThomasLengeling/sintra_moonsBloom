
void MoonsApp::oscSetup()
{
	listener.setup(PORT);
}

void MoonsApp::oscUpdate()
{
    int rand = ci::randInt(3);
    bool onsetAmp = false;
    
	while (listener.hasWaitingMessages()) {
		ci::osc::Message message;
		listener.getNextMessage(&message);

		//	console() << "New message received" << std::endl;
		//	console() << "Address: " << message.getAddress() << std::endl;
		//	console() << "Num Arg: " << message.getNumArgs() << std::endl;

		for (int i = 0; i < message.getNumArgs(); i++) {
			//		console() << "-- Argument " << i << std::endl;
			//		console() << "---- type: " << message.getArgTypeName(i) << std::endl;

			if (message.getAddress() == "/pitch"){
                
                if(message.getArgType(0) == ci::osc::TYPE_INT32){ //background emmanuek
                    if(message.getArgAsInt32(0) == 3)
                        ci::app::console()<<"3 tags"<<std::endl;
                }
				if (message.getArgType(i) == ci::osc::TYPE_FLOAT) {
					try {
						//    console() << "------ value: " << message.getArgAsFloat(i) << std::endl;
						freq = ci::lmap<float>(message.getArgAsFloat(i), 20, 20000, 0, 1.0);
						freqLog = log(message.getArgAsFloat(i) + 1.0);
                        float randIndex = ci::randInt(3);
                        mParticleEmitters->setFreq(randIndex, freq);
					}
					catch (...) {
						console() << "Exception reading argument as float" << std::endl;
					}
				}
			}
			if (message.getAddress() == "/onset"){
				onSet = !onSet;
                onsetAmp = true;
                
                if(message.getArgAsInt32(0) == 3){
                //    mParticleEmitters->switchBackgrounds(rand);
                }
			}

			if (message.getAddress() == "/amp"){
				if (message.getArgType(i) == ci::osc::TYPE_FLOAT) {
					try {
						//     console() << "------ value: " << message.getArgAsFloat(i) << std::endl;
						amp = ci::lmap<float>(message.getArgAsFloat(i), 0.0, 1.0, 0.0, 1.0);
                        mParticleEmitters->setAmp(rand, amp);
					}
					catch (...) {
						console() << "Exception reading argument as float" << std::endl;
					}
				}
			}
            
            if (message.getAddress() == "/powerbands"){
				if (message.getArgType(i) == ci::osc::TYPE_BLOB) {
					try {
						//     console() << "------ value: " << message.getArgAsFloat(i) << std::endl;
						amp = ci::lmap<float>(message.getArgAsFloat(i), 0.0, 1.0, 0.0, 1.0);
					}
					catch (...) {
						console() << "Exception reading argument as float" << std::endl;
					}
				}
			}
            
		}
	}
    if(onsetAmp){
         mParticleEmitters->switchBackgrounds(rand);
         mParticleEmitters->setAmp(rand, amp);
    }else{
       // mParticleEmitters->setAmp(rand, 0.0);
    }
    
    mParticleEmitters->setBeatValue(rand, amp + freq);
}