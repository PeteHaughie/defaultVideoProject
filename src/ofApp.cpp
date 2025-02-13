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
#include "ofApp.h"

#include "iostream"

#define MIDI_MAGIC 63.50f // this is the direct middle of the midi range, 127
#define CONTROL_THRESHOLD .04f

bool hdAspectRatioSwitch = false;
float az = 1.0;
float sx = 0;
float dc = 0;

// dummy variables for midi control

float c1 = 0.0;
float c2 = 0.0;

int width = 0;
int height = 0;

const int controlSize = 17;

float control[controlSize];

bool midiActiveFloat[controlSize];

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofBackground(ofColor::black);
	ofHideCursor();
	ofDisableArbTex(); // normalise the coords
	width = 640;
	height = 480;

	inputSetup();

	allocateAndDeclareSundries();

	shader.load("shader");

	midiSetup();

	for (int i = 0; i < controlSize; i++)
	{
		control[i] = 0.0;		
		midiActiveFloat[i] = false;
	}
}

//--------------------------------------------------------------
void ofApp::inputSetup()
{

	input.setDesiredFrameRate(30);
	input.setDeviceID(0);
	input.initGrabber(width, height);
}

//------------------------------------------------------------
void ofApp::allocateAndDeclareSundries()
{
	fbo0.allocate(width, height, GL_RGBA);
	fbo0.begin();
	ofClear(0, 0, 0, 255);
	fbo0.end();

	aspectFixFb.allocate(width, height, GL_RGBA);
	aspectFixFb.begin();
	ofClear(0, 0, 0, 255);
	aspectFixFb.end();
}

//--------------------------------------------------------------
void ofApp::update()
{
	inputUpdate();
	midiBiz();
	midiUpdate();
}

void ofApp::midiUpdate()
{
	if (midiActiveFloat[0])
	{
		sx = control[0]; // -1 to 1
	}
	if (midiActiveFloat[9])
	{
		dc = control[8]; // -1 to 1
	}
}

//--------------------------------------------------------------
void ofApp::inputUpdate()
{
	if (devID != prevDevID)
	{
		input.close();
		input.setDeviceID(devID);
		input.initGrabber(width, height);
		prevDevID = devID;
	}

	if (midiID != prevMidiID)
	{
		midiIn.closePort();
		midiIn.openPort(midiID);
		ofLog() << "midiID: " << midiID;
		prevMidiID = midiID;
	}

	input.update();

	if (input.isFrameNew())
		// corner crop and stretch to preserve hd aspect ratio
		if (hdAspectRatioSwitch == 1)
		{
			aspectFixFb.begin();
			input.draw(-106, 0, ofGetWidth() + 106, ofGetHeight());
			aspectFixFb.end();
		}
		{
			fbo0.begin();
			ofClear(ofColor::white);
			shader.begin();

			shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
			shader.setUniform1f("mode", mode);
			shader.setUniform1f("xDisplace", sx);
			shader.setUniform1f("yDisplace", dc);

			if (hdAspectRatioSwitch == 0)
			{
				shader.setUniformTexture("tex0", input.getTexture(), 0);
			}
			if (hdAspectRatioSwitch == 1)
			{
				shader.setUniformTexture("tex0", aspectFixFb.getTexture(), 0);
			}
			ofDrawRectangle(0, 0, 640, 480);
			shader.end();
			fbo0.end();
		}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	fbo0.draw(0, 0, ofGetWidth(), ofGetHeight());

	// I use this block of code to print out like useful information for debugging various things and/or just to keep the
	// framerate displayed to make sure I'm not losing any frames while testing out new features.  uncomment the ofDrawBitmap etc etc
	// to print the stuff out on screen
	ofSetColor(255);
	string msg = "fps: " + ofToString((int)ofGetFrameRate(), 2);
	// ofDrawBitmapString(msg, 20, ofGetHeight() - 10);

	/*
	for(int i=0;i<controlSize;i++){
	cout<<control[i]<<endl;
	}
	for (int i = 0; i < controlSize; i++)
	{
		if (midiActiveFloat[i])
		{
			cout << "control-" << i << " is active and value is " << control[i] << endl;
		}
	}
	*/
	// active midi control and value share the same index in the array eg: `if (midiActiveFloat[0]) control0[0];` is the value of #cc 16
}

//--------------------------------------------------------------
void ofApp::exit()
{

	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage &msg)
{

	// add the latest message to the message queue
	midiMessages.push_back(msg);

	// remove any old messages if we have too many
	while (midiMessages.size() > 2)
	{
		midiMessages.erase(midiMessages.begin());
	}
}

//----------------------------------------------------------
void ofApp::midiSetup()
{
	// print input ports to console
	midiIn.listInPorts();

	// open port by number (you may need to change this)
	midiIn.openPort(0);
	// midiIn.openPort("IAC Pure Data In");	// by name
	// midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port

	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);

	// add ofApp as a listener
	midiIn.addListener(this);

	// print received messages to the console
	midiIn.setVerbose(true);
}

//----------------------------------------------------------
void ofApp::midiBiz()
{

	for (unsigned int i = 0; i < midiMessages.size(); ++i)
	{

		ofxMidiMessage &message = midiMessages[i];

		if (message.status < MIDI_SYSEX)
		{
			// text << "chan: " << message.channel;
			if (message.status == MIDI_CONTROL_CHANGE)
			{
				// #cc 16 – 23
				if (message.control > 15 && message.control < 24)
				{
					if (abs((message.value - MIDI_MAGIC) / MIDI_MAGIC - control[message.control - 16]) < CONTROL_THRESHOLD)
					{
						midiActiveFloat[message.control - 16] = true;
					}
					if (midiActiveFloat[message.control - 16]) // if the control is active and "true"
					{
						control[message.control - 16] = (message.value - MIDI_MAGIC) / MIDI_MAGIC;
					}
				}
				// #cc 120 – 127
				if (message.control > 119 && message.control < 128)
				{
					if (abs((message.value - MIDI_MAGIC) / MIDI_MAGIC - control[message.control - 120 + 8]) < CONTROL_THRESHOLD)
					{
						midiActiveFloat[message.control - 120 + 8] = true;
					}
					if (midiActiveFloat[message.control - 120 + 8]) // if the control is active and "true"
					{
						control[message.control - 120 + 8] = (message.value - MIDI_MAGIC) / MIDI_MAGIC;
					}
				}
				// #cc 58
				if (message.control == 58)
				{
					for (int i = 0; i < controlSize; i++)
					{
						control[i] = 0.0;
						midiActiveFloat[i] = false;
					}
				}
				// #cc 61
				if (message.control == 61)
				{
					mode += 1;
					if (mode > 2)
					{
						mode = 0;
					}
					midiMessages.erase(midiMessages.begin() + i);
				}
				// #cc 62
				if (message.control == 62)
				{
						hdAspectRatioSwitch = !hdAspectRatioSwitch;
						midiMessages.erase(midiMessages.begin() + i);
				}
			}
		}
	}
}

//----------------------------------------------------------
void ofApp::keyPressed(int key)
{
	// Here is how I map controls from the keyboard
	if (key == ' ' || key == '0')
	{
		vector<ofVideoDevice> devices = input.listDevices();
		if (devID < devices.size())
		{
			devID++;
		}
		else
		{
			devID = 0;
		}
	}
	if (key == '9')
	{
		vector<string> devices;
		devices = midiIn.getInPortList();
		if (devices.empty())
		{
			cout << "no midi devices found" << endl;
			return;
		}
		else
		{
			for (unsigned long int i = 0; i < devices.size(); i++)
			{
				cout << devices[i] << endl;
			}
			if (midiID < devices.size())
			{
				midiID++;
			}
			else
			{
				midiID = 0;
			}
		}
	}

	// fb0 x displace
	if (key == 's')
	{
		sx += .01;
	}
	if (key == 'x')
	{
		sx -= .01;
	}

	// fb0 y displace
	if (key == 'd')
	{
		dc += .01;
	}
	if (key == 'c')
	{
		dc -= .01;
	}
	if (key == 'm')
	{
		mode += 1.0;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}
