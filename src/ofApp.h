#pragma once

#include "ofMain.h"
#include "ofAppEGLWindow.h"
#include "TerminalListener.h"
#include "ofxRPiCameraVideoGrabber.h"
#include "ImageFilterCollection.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        //void animateText();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    bool enabled;
    bool bFirst;
    string typeStr;
    
    ofTrueTypeFont  clarendon;
    
    float centreX;
    float centreY;
    
    bool animate;
    
    
    // - timer ones
    void                timerComplete(int num);
    float               endTime; // when do want to stop the timer
    bool                bTimerReached; // used as a trigger when we hit the timer
    
    bool                bCountdown;
    bool                snapCountdown;
    
    
    float               currentTime; // is counting on every draw

    int fader; //the actual 255 number
    bool animFader; // turn animation up or down
    
    int fontSize; //font size

    float scale;
    float countNumber;
    bool showCountdown;
    int flashFader;
    bool showFlash;
    
    
    ////VIDEO
    ofVideoGrabber 		vidGrabber;
    unsigned char * 	videoInverted;
    ofTexture			videoTexture;
    int 				camWidth;
    int 				camHeight;
    
    
    ofImage overlay1;
    ofImage overlay2;
    ofFbo fbo;
    bool takePhoto;
    
    ofImage img;
    
    string              dateTime;
    int 				snapCounter;

    
    
    // -  timer for showing stored image
    void                SnaptimerComplete(int num);
    float               SnapstartTime; // store when we start time timer
    float               SnapendTime; // when do want to stop the timer
    bool                SnapTimerReached; // used as a trigger when we hit the timer
 
    bool alert;
    
    float timer2;
    
    
    ///OMX camera
    TerminalListener consoleListener;
    ofxRPiCameraVideoGrabber videoGrabber;
    
    ImageFilterCollection filterCollection;
    
    OMXCameraSettings omxCameraSettings;
    
    bool doDrawInfo;
    
    };

