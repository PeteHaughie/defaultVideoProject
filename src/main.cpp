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
#include "ofMain.h"
#include "ofApp.h"

int main()
{
	ofGLESWindowSettings settings;
	settings.setGLESVersion(2);
#ifdef __linux__
	settings.windowMode = OF_GAME_MODE;
#else
	settings.windowMode = OF_WINDOW;
#endif
	settings.setSize(640, 480);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
