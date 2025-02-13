/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

class ofApp : public ofBaseApp, public ofxMidiListener
{

public:
	~ofApp() noexcept override = default;
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);

	void newMidiMessage(ofxMidiMessage &eventArgs);
	ofxMidiIn midiIn;
	std::vector<ofxMidiMessage> midiMessages;
	std::size_t maxMessages = 8; //< max number of messages to keep track of

	void midiSetup();
	void midiBiz();
	void midiUpdate();

	ofShader shader;

	ofFbo fbo0;
	ofFbo fbo1;
	ofFbo aspectFixFb;
	long unsigned int devID = 0, prevDevID = 0;
	long unsigned int midiID = 0, prevMidiID = 0;

	int modeValue = 0, prevModeValue = 0;
	float mode = 0.0;

	ofVideoGrabber input;

	void allocateAndDeclareSundries();
	void inputSetup();
	void inputUpdate();
};
