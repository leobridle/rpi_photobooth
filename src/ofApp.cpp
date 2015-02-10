#include "ofApp.h"


////TODO


// ad enable button function

//convert to rPi



//--------------------------------------------------------------
void ofApp::setup(){
    
    //show overlay debug info or not
    doDrawInfo= false;
    
    //ofSetLogLevel(OF_LOG_VERBOSE);
    //ofSetLogLevel("ofThread", OF_LOG_ERROR);
    
    //allows keys to be entered via terminal remotely (ssh)
    //consoleListener.setup(this);

    currentTime=ofGetElapsedTimeMillis();
    dateTime = ofGetTimestampString();

    
    ofSetFrameRate(25);

    ofBackground(54, 54, 54, 255);
    
    //old OF default is 96 - but this results in fonts looking larger than in other programs.
    ofTrueTypeFont::setGlobalDpi(72);
    
    fontSize = 200;
    clarendon.loadFont("assets/clarendon.ttf", fontSize, true, true);
    clarendon.setLineHeight(72);
    clarendon.setLetterSpacing(1.037);

    countNumber = -2;
    typeStr = ofToString(countNumber);
    
    centreX = ofGetWindowWidth()*0.5;
    centreY = ofGetWindowHeight()*0.5;
    
    fader = 1;
    animFader=true;
    
    showCountdown=false;
    scale=0.2;
    flashFader=0;
    showFlash=false;
    
    ///VIDEO
    camWidth 		= ofGetWindowWidth();	// try to grab at this size.
    camHeight 		= ofGetWindowHeight();

    //vidGrabber.setDeviceID(0);
    //vidGrabber.setDesiredFrameRate(60);
    //vidGrabber.initGrabber(camWidth,camHeight);
    //videoTexture.allocate(camWidth,camHeight, GL_RGB);
    //ofSetVerticalSync(true);
    
    
    /// OMX VIDEO
    omxCameraSettings.width = camWidth; //default 1280
    omxCameraSettings.height = camHeight; //default 720
    omxCameraSettings.isUsingTexture = true; //default true
    omxCameraSettings.doRecording = false;   //default false
    
    	videoGrabber.setup(omxCameraSettings);
        filterCollection.setup();

    
    
    ///
    
    overlay1.setImageType(OF_IMAGE_COLOR_ALPHA);
    overlay1.loadImage("assets/overlay1.png");
    overlay2.setImageType(OF_IMAGE_COLOR_ALPHA);
    overlay2.loadImage("assets/overlay2.png");
    fbo.allocate(camWidth, camHeight, GL_RGBA); // with alpha, 8 bits red, 8 bits green, 8 bits blue, 8 bits alpha,
    fbo.begin();
    ofClear(255,255,255, 0);
    overlay1.draw(50,30);
    overlay2.draw(800,420);
    fbo.end();
    
    takePhoto=false;
    snapCounter=0;

//timer
    SnapTimerReached = false;
    SnapendTime = 4000; // in milliseconds - how long stored image is shown for (allow time for image saving delay too)


}

//--------------------------------------------------------------
void ofApp::update(){
    
    scale +=0.07; //scale rate
    if(animFader==true){fader +=30; }
    if(animFader==false){fader -=30; }
    if(fader>=255){animFader=false;} //stop fading up at full opacity (255)
    if(fader<=0){
        countNumber--; //when it's invisible we advance to next number.
        scale = 0.2; //and reset size
        fader = 0; //and make invisible
        animFader=true; //then start animation again

    }
  
    if (countNumber ==0 && currentTime>=3000){showCountdown=false;} //check when to play flash animation
    
    if (countNumber==0 && fader==0){ ///single fire event at end of countdown
        cout << "photo call now!" << endl;

        takePhoto=true;
    }

    
    ///VIDEO
    vidGrabber.update();
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    currentTime = ofGetElapsedTimeMillis();
    
    
    ///VIDEO scale and translate are to flip image
    ofSetHexColor(0xffffff);
    ofScale( -1, 1, 1 );
    ofTranslate( -ofGetWidth(), 0, 0 );
    //draws at camera resolution
    videoGrabber.draw();
    //OLD//vidGrabber.draw(0,0,camWidth,camHeight);
    ofScale( -1, 1, 1 );
    ofTranslate( -ofGetWidth(), 0, 0 );


    ////// COUNTDOWN ANIMATION /////
    if(enabled==true && showCountdown==true){
    
    ofPushMatrix();
    ofTranslate(centreX, centreY);
    ofScale(scale, scale, 1);
    
    float fontWidth = clarendon.stringWidth(typeStr);
    float fontHeight = clarendon.stringHeight(typeStr);
    ofEnableAlphaBlending();
    ofSetColor(255 , 255, 255,fader);
    clarendon.drawString(ofToString(countNumber), -fontWidth/2, -fontHeight/2+100);//last number is vertical scale offset
    ofPopMatrix();
    }

    
    ofEnableAlphaBlending(); //draw the fbo overlay stuff
    ofSetColor(255 , 255, 255,255);
    fbo.draw(0,0,camWidth,camHeight);
    
    // conditions to show or hide stored image based on snapendtime and timer2 (could be improved probably)
    
    //timer2
    float timer2 = ofGetElapsedTimeMillis() - SnapstartTime;
    
    if(timer2 >= SnapendTime) { //if "timer started at zero on flash" is more than "5 seconds set as time limit at start"....
        //hides saved image
        
        ofSetHexColor(0xFFFFFF);
        img.draw(-2000,-2000,48,27);
        enabled=true;
        
    } if (timer2< SnapendTime && snapCounter>=1){
        
        //shows saved image
        
        ofSetHexColor(0xFFFFFF);
        img.draw(0,0,camWidth,camHeight);
        enabled=false;
        
    }
    if(timer2>=2000 && timer2<=2100 && snapCounter>=1){ //hack to just call photo save once. Might double fire sometimes.
        
        cout << "saving image now" << endl;
        
        string fileName = "img_"+ofToString(dateTime)+".png";
        img.saveImage(fileName);
        
    }


    
    

    if (takePhoto==true){  ////FIRE!
        
        // grab a rectangle
        img.grabScreen(0,0,camWidth,camHeight);

        SnapstartTime = ofGetElapsedTimeMillis();  // Initiate timer at 0. Will start counting up to limit from now.

        takePhoto= false;
        snapCounter++; //snapcounter is neccesary to make sure that things don't fire before buttons pressed.
        
        showFlash==true;
        flashFader==255;
        
    }
    
    
    if (countNumber<=0){ //end of countdown animation continuous event
        flashFader-=10;
        ofEnableAlphaBlending();
        ofSetColor(255,255,255,flashFader),
        ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    }

    
    ////INFO
    if (doDrawInfo ==true){
    ofSetColor(100 , 255, 100);
    string fpsStr = "frame rate: "+ofToString(ofGetFrameRate(), 2);
    string timerstring = "true or false: "+ofToString(animFader);
    string xTimeString = "fader value: "+ofToString(fader);
    string count2 = "count:  "+ofToString(countNumber);
    string infoSnapstart ="snapstart: "+ofToString(SnapstartTime);
    string infotimer2 ="timer2: "+ofToString(timer2);
    string enableX ="enableButton status: "+ofToString(alert);
    


    
    ofDrawBitmapString(fpsStr +"\n\n"
                       + timerstring
                       + "\n\n" + xTimeString
                       + "\n\n" + count2
                       + "\n\n" + infoSnapstart
                       + "\n\n" + infotimer2
                       + "\n\n" + enableX
                       
                       
                       ,100,500);
    
}
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    if(key == ' ' && enabled ==true){
        scale = 0.5;
        fader = 1;
        animFader=true;
        countNumber=3;
        showCountdown=true;
        flashFader=255;

    }
    if (key == 'e')
    {
        videoGrabber.applyImageFilter(filterCollection.getNextFilter());
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    


}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


