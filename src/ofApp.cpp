#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){	 
/*
	// load in sounds:
	beat.load("sounds/jdee_beat.mp3");
	ow.load("sounds/ow.mp3");
	dog.load("sounds/dog.mp3");
	rooster.load("sounds/rooster.mp3");
*/	
	song.load("sounds/song.mp3");	
	px = 0.0;
/*	
	
	// we will bounce a circle using these variables:
	px = 300;
	py = 300;
	vx = 0;
	vy = 0;	
*/
	// the fft needs to be smoothed out, so we create an array of floats
	// for that purpose:
	fftSmoothed = new float[8192];
	for (int i = 0; i < 8192; i++){
		fftSmoothed[i] = 0;
	}
	
	nBandsToGet = 256;

////////////////////////////////////////////////////////////////
//		Adding Mesh from point cloud example	      //
////////////////////////////////////////////////////////////////

	ofSetVerticalSync(true);
	
	// load an image from disk
//	img.load("linzer.png");
	
	// we're going to load a ton of points into an ofMesh
	mesh.setMode(OF_PRIMITIVE_POINTS);
	
	int height=250;
	int width=250;

	// loop through the image in the x and y axes
	int skip = 4; // load a subset of the points
	for(int y = -250; y < width; y +=skip) {
		for(int x = -250; x < height; x+=skip) {
			ofColor cur = ofColor(255,0, 0);
			if(cur.a > 0) {
				// the alpha value encodes depth, let's remap it to a good depth range
				float z = ofMap(cur.a, 0, 255, -300, 300);
				cur.a = 255;
				mesh.addColor(cur);
				ofVec3f pos(x, z-450, y);
				mesh.addVertex(pos);
			}
		}
	}

	ofEnableDepthTest();
	glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
	glPointSize(3); // make the points bigger


}


//--------------------------------------------------------------
void ofApp::update(){
	
	ofBackground(12,12,12);

	// update the sound playing system:
	ofSoundUpdate();	

	song.setVolume(1);
/*	
	// (1) we increase px and py by adding vx and vy
	px += vx;
	py += vy;
*/	
	// (2) check for collision, and trigger sounds:
	// horizontal collisions:
	if (px == 0.0){
		song.play();
		px+=1;
		}

/*	if (px < 0){
		px = 0;
		vx *= -1;
		dog.play();
	} else if (px > ofGetWidth()){
		px = ofGetWidth();
		vx *= -1;
		ow.play();
	}
	// vertical collisions:
	if (py < 0 ){
		py = 0;
		vy *= -1;
		rooster.play();
	} else if (py > ofGetHeight()){
		py = ofGetHeight();
		vy *= -1;
		beat.play();
	}
	// (3) slow down velocity:
	vx 	*= 0.996f;
	vy 	*= 0.996f;
*/
/*	// (4) we use velocity for volume of the samples:
	float vel = sqrt(vx*vx + vy*vy);
	ow.setVolume(MIN(vel/5.0f, 1));
	beat.setVolume(MIN(vel/5.0f, 1));
	dog.setVolume(MIN(vel/5.0f, 1));
	rooster.setVolume(MIN(vel/5.0f, 1));
*/
	// (5) grab the fft, and put in into a "smoothed" array,
	//		by taking maximums, as peaks and then smoothing downward



	float * val = ofSoundGetSpectrum(nBandsToGet);		// request 256 values for fft
	for (int i = 0;i < nBandsToGet; i++){
		
		// let the smoothed calue sink to zero:
		fftSmoothed[i] *= 0.89f;
		
		// take the max, either the smoothed or the incoming:
		if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
		
	}


}

//--------------------------------------------------------------
void ofApp::draw(){

	
	// draw the fft resutls:
	ofSetColor(255,255,255,255);
	
	float width = (float)(5*256) / nBandsToGet;
	for (int i = 0;i < nBandsToGet; i++){
		// (we use negative height here, because we want to flip them
		// because the top corner is 0,0)
		ofDrawRectangle(100+i*width,ofGetHeight()-100,width,-(fftSmoothed[i] * 200));
	}
	
	// finally draw the playing circle:

	ofEnableAlphaBlending();
		ofSetColor(255,255,255,20);
		ofDrawCircle(px, py,50);
	ofDisableAlphaBlending();
	
//	ofSetHexColor(0xffffff);
//	ofDrawCircle(px, py,8);

//
////////////////////////////////////////////////////////////////
//		Adding Mesh code from point cloud example     //
////////////////////////////////////////////////////////////////

	ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);
	
	// even points can overlap with each other, let's avoid that
	cam.begin();
//	ofScale(2, -2, 2); // flip the y axis and zoom in a bit
//	ofRotateY(90);
//	ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2);
	mesh.draw();
	cam.end();

}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	// add into vx and vy a small amount of the change in mouse:
	vx += (x - prevx) / 20.0f;
	vy += (y - prevy) / 20.0f;
	// store the previous mouse position:
	prevx = x;
	prevy = y;
}
 
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	prevx = x;
	prevy = y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

