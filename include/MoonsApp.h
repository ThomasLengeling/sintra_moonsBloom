#pragma once


#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"

#include "cinder/MayaCamUI.h"

#include "cinder/params/Params.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"

#include "Resources.h"

#include "ParticleSystemReactive.h"

#include "OscListener.h"

#include "MidiIn.h"
#include "MidiMessage.h"
#include "MidiConstants.h"

#include "ParticleEmitters.h"


#define NUMPARTICLE     200
#define NUMSPHERE       3

#define APP_WIDTH       1920
#define APP_HEIGHT      1080

#define PORT            32000

/*
spline moving attraction position
camera change zoom and out.

simulate the initial setup
change colors.

shader spheres joining and exploding.


when creating a sphere with custom index, normals and vertexs, by
definition its a convex hull of a sphere, then changing the vertex will transform everytime the sphere
but eveytime its a sphere by definition of the index of the vertices.
*/

class MoonsApp : public ci::app::AppNative {
public:

	void setup();
	void prepareSettings(ci::app::AppBasic::Settings *settings);
	void mouseDown(ci::app::MouseEvent event);
    
	void mouseMoveMain(ci::app::MouseEvent event);
    void mouseMoveGUI(ci::app::MouseEvent event);
    
	void mouseDrag(ci::app::MouseEvent event);
	void mouseUp(ci::app::MouseEvent event);
	void keyDown(ci::app::KeyEvent event);

	void update();
	void draw();
	void resizeMain();
    void resizeGUI();
    void shutdown();

	//SECOND WINDOW
	WindowRef	 mMainWindow;
	void         drawMain();
	void         shutDown();

	void         drawGUIWindow();
	WindowRef    mRenderWindow;

	//PARTICLES AND AUDIO
	ParticleEmitters * mParticleEmitters;
    
    bool    readyParticles;
	bool    activateHistory;
	bool    activateContrast;
	int     contrasteCount;
	bool    contrasteColor;

	float   lineSensity;
	float   contrasteThreshold;
    
    int     indexEmitter;

	//CAM
	ci::CameraPersp     mCam;
	ci::Quatf           camOrientation;
	bool                activateBeatGrow;

	ci::Vec3f           sphericalToCartesians(float radius, float theta, float phi);

	//PARAMS
	ci::params::InterfaceGlRef mParams;

	float                   avgFps;
	float                   randAngle;

	//GLSL
	ci::gl::GlslProg        mDifuseShader;

	//OSC
	void				oscSetup();
	void				oscUpdate();
	ci::osc::Listener   listener;
	float				freq;
	float				freqLog;
	float				amp;
	bool				onSet;

	//LIGHTS
	bool  enableLights;
	float attenuationBloom;
	float alphaBackground;
	float grayBackground;
    float masterBackground;

	//TIMERS
	void timeToggle();
	void timeActivator();

	bool activateEvent;
	int  T0GGLE_MODE;
	int  eventTimePrev;

	//CAMERA
	float distanceCamera;

	float zoom;
	float particleRadius;
	ci::Vec3f mCenter;
	ci::Vec3f mUp;

	float beatParticleThreshold;

	//MIDI
	void				setupMIDI();
	void				updateMIDI();
	void				processMidiMessage(ci::midi::Message* message);
	ci::midi::Input		mMidiIn;
};