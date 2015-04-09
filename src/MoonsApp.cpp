#include "MoonsApp.h"
#include "timeToggle.h"
#include "OscDetector.h"
#include "Midi.h"

using namespace ci;
using namespace ci::app;
using namespace std;

//-------------------SETUP-----------------------------------------------------
void MoonsApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize(350, 600);
    settings->setFrameRate(200.0f);
 //   settings->disableFrameRate();
    settings->setWindowPos(0, 0);
    
}

void MoonsApp::setup()
{
    glEnable(GL_LINE_SMOOTH);
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_POINT_SMOOTH);
    glHint( GL_POINT_SMOOTH_HINT,  GL_NICEST );
	glEnable(GL_POINT_SIZE);
    glEnable(GL_LINE_WIDTH);
    
    //SECOND WINDOW
    mMainWindow = getWindow();
	mMainWindow->setTitle("GUI");
	mMainWindow->connectDraw(&MoonsApp::drawGUIWindow, this);
    mMainWindow->connectResize(&MoonsApp::resizeGUI, this);
    mMainWindow->connectMouseMove(&MoonsApp::mouseMoveGUI, this);

    ci::app::setFrameRate(200.0f);
    Window::Format format;
    format.size(APP_WIDTH, APP_HEIGHT);
    format.borderless();
    mRenderWindow = createWindow(format);
    mRenderWindow->connectDraw(&MoonsApp::drawMain, this);
    mRenderWindow->connectResize(&MoonsApp::resizeMain, this);
    mRenderWindow->connectMouseMove(&MoonsApp::mouseMoveMain, this);
    mRenderWindow->connectClose(&MoonsApp::shutdown, this);

	console() << "setting up OSC" << std::endl;
	oscSetup();

	console() << "setting up MIDI" << std::endl;
	setupMIDI();

    //CAMERA
    mCenter			= Vec3f::zero();
    mUp				= Vec3f::yAxis();
    mCam.setPerspective(45.0f, getWindowAspectRatio(), 0.1, 10000);
    mCam.setEyePoint(ci::Vec3f(0.f, 0.f, 300.f));
    mCam.setCenterOfInterestPoint(ci::Vec3f::zero());
    
    //PARTICLES
    mParticleEmitters = new ParticleEmitters(NUMSPHERE, NUMPARTICLE, ci::Vec2f(APP_WIDTH, APP_HEIGHT));
    
    //PARAMS
    mParams = params::InterfaceGl::create(mMainWindow, "Params", toPixels(ci::Vec2i(300, 500)));
    mParams->addParam("FPS", &avgFps);
    mParams->addSeparator();
    mParams->addParam("camOrientation", &camOrientation, "");
    mParams->addParam("CenterCam", &mCenter, "min=0.0 max=1000.0 step=10.0");
    mParams->addSeparator();
    mParams->addParam("attenuationBloom", &attenuationBloom, "min=0 max=2.5 step=0.1 keyIncr=z keyDecr=Z");
    mParams->addParam("alphaBackground", &alphaBackground, "min=0 max=1.0 step=0.01 keyIncr=x keyDecr=X");
    mParams->addParam("grayBackground", &grayBackground, "min=0 max=1.0 step=0.01 keyIncr=c keyDecr=C");
    
    mParams->addParam("particleRadius", &particleRadius, "min=50 max=150 step=1 step=1 keyIncr=r keyDecr=R");
    mParams->addParam("distanceCamera", &distanceCamera, "min=0 max=600 step=1 step=1 keyIncr=d keyDecr=D");
    
	//------------------------START VALUES------------------------------------------------------------------
	enableLights = true;
	activateHistory = true;
    
	contrasteThreshold = 8;

	activateContrast = false;
	contrasteColor = false;
    
	eventTimePrev = 0;
    
    attenuationBloom = 0.9f;
    alphaBackground  = 0.20;
    grayBackground   = 0.06;
    
    particleRadius   = 150;
    distanceCamera      = 420;
    
    activateBeatGrow = true;
    
    beatParticleThreshold = 0.001f;

	contrasteCount = 0;
    
    indexEmitter = 0;
    
    mParticleEmitters->activateColorParticles();
	//---------------------------------------------------------------------------------------------------------
	console() << "donde setup" << endl;
}

void MoonsApp::resizeMain()
{
    glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
    
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POINT_SMOOTH_HINT,  GL_NICEST );
    
    //mCam.setPerspective(45.0f, getWindowAspectRatio(), 0.1, 10000);
}

void MoonsApp::resizeGUI()
{
    
}


//------------EVENTS--------------------------------------------------------------
void MoonsApp::mouseDown( MouseEvent event )
{
    
}

void MoonsApp::mouseDrag( MouseEvent event)
{
    
}

void MoonsApp::mouseMoveMain( MouseEvent event)
{

}

void MoonsApp::mouseMoveGUI( MouseEvent event)
{
    
}

void MoonsApp::mouseUp( MouseEvent event)
{

}

void MoonsApp::keyDown(KeyEvent event)
{
    switch (event.getChar()) {
        case 'a':
            mParticleEmitters->clean();
            mParticleEmitters = new ParticleEmitters(NUMSPHERE, NUMPARTICLE, ci::Vec2f(APP_WIDTH, APP_HEIGHT));
        break;
        case 's':
            // mParticleSystemReactive->createParticlesRight(NUMPARTICLE);
            break;
        case '1':

            break;
        case '2':

            break;
        
        case '3':

            break;
        case '4':

            break;
            
        case '5':

            break;
        case '6':

            break;
    }
}

void MoonsApp::shutdown()
{
    mParticleEmitters->clean();
}

//------------UPDATE-----------------------------------------------------------------
void MoonsApp::update()
{
    avgFps = getAverageFps();
    
    timeActivator();
    
	oscUpdate();

	updateMIDI();
    
    mParticleEmitters->updateParticles();
    mParticleEmitters->draw();
    mParticleEmitters->updateBloom();
}
//------------DRAW------------------------------------------------------------------
void MoonsApp::draw()
{

}

void MoonsApp::drawMain()
{
    mParticleEmitters->drawBloom();
   // mParticleEmitters->drawFboTextures();
    
}

void MoonsApp::drawGUIWindow()
{
    gl::disableAlphaBlending();
    gl::disableDepthRead();
    gl::disableDepthWrite();
    
    gl::clear(Color::black());
 
    mParams->draw();
}


ci::Vec3f MoonsApp::sphericalToCartesians(float radius, float theta, float phi){
    float x = radius * sinf( theta ) * cosf( phi );
    float y = radius * sinf( theta ) * sinf( phi );
    float z = radius * cosf( theta );
    return ci::Vec3f(x, y, z);
}

CINDER_APP_NATIVE( MoonsApp, RendererGl  )
