#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "SpoutLibControls.h"
#include "SpoutLibReceiver.h"
#include "SpoutLibSender.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);

	void updateParameters();

private:
	enum {
		WIDTH = 1280,
		HEIGHT = 720,
		FBO_WIDTH = 1920,
		FBO_HEIGHT = 1080
	};
	
	// gl
	ofFbo fbo;
	
	bool is_debug_visible = true;
	const string gui_filename = "settings/gui_setting.xml";
	ofxGuiGroup gui;

	// settings
	ofParameter<float> time_step;
	ofParameter<float> elapsed_time;
	ofParameter<float> time_value;
	ofParameter<float> g_threshold;

	ofParameterGroup spout_group;
	ofParameter<string> spout_text;
	ofParameter<bool> spout_bool;
	ofParameter<float> spout_float;

	// spout
	ofTexture texture;
	SpoutLib::SenderRef spout_sender;
	SpoutLib::ReceiverRef spout_receiver;
	SpoutLib::ControlsRef spout_controls;
};



