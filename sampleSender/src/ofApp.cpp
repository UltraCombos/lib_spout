#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(WIDTH, HEIGHT);
	ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
	ofDisableArbTex();
	ofSetFrameRate(60);
	//ofSetVerticalSync(true);

	{
		ofLog(OF_LOG_NOTICE, "application start with resolution: %u x %u", ofGetWidth(), ofGetHeight());
	}
	
	{
		spout_sender = SpoutLib::Sender::create("SpoutLib Sender");
		
	}

	// allocate fbo
    {
        ofFbo::Settings s;
        s.width = FBO_WIDTH;
		s.height = FBO_HEIGHT;
        s.useDepth = true;
		s.colorFormats = { GL_RGBA8 };
        
        fbo.allocate(s);
    }
    
	// setup gui
    {
		gui.setup();
		gui.setName("gui");

		ofParameterGroup g_settings;
		g_settings.setName("settings");
		g_settings.add(time_step.set("time_step", 1.0f / 120.0f, 0.0f, 1.0f / 30.0f));
		time_step.setSerializable(false);
		g_settings.add(elapsed_time.set("elapsed_time", ofGetElapsedTimef()));
		elapsed_time.setSerializable(false);
		g_settings.add(time_value.set("time_value", 0, 0, 1));
		time_value.setSerializable(false);
		g_settings.add(g_threshold.set("threshold", 0.5f, 0, 1));
		gui.add(g_settings);

		spout_group.setName("spout controls");
		spout_group.add(spout_text.set("s_text", "from openFramewroks"));
		spout_group.add(spout_bool.set("s_bool", true));
		spout_group.add(spout_float.set("s_float", 1.0f, 0.0f, TWO_PI));
		gui.add(spout_group);

		gui.loadFromFile(gui_filename);
    }
	
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetWindowTitle("oF Spout Sender: " + ofToString(ofGetFrameRate(), 1));

	

	// update params and ubo
	updateParameters();
	
	// update main fbo
	{
		fbo.begin();
		ofClear(0);
		auto viewport = ofGetCurrentViewport();
		float x = ((float)ofGetMouseX() / ofGetWidth()) * viewport.width;
		float y = ((float)ofGetMouseY() / ofGetHeight()) * viewport.height;
		ofDrawCircle(x, y, 30.0f);

		fbo.end();
	}
	
	spout_sender->update(fbo.getTexture());
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(0);
	auto viewport = ofGetCurrentViewport();
	
	fbo.draw(viewport);

	// draw debug things
	if (is_debug_visible)
	{
		gui.draw();
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofLog(OF_LOG_NOTICE, "application exit");
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){	
	
	switch (key)
	{
	case OF_KEY_F1:
		is_debug_visible = !is_debug_visible;
		break;
	case 's':
		gui.saveToFile(gui_filename);
		break;
	case 'l':
		gui.loadFromFile(gui_filename);
		break;
	}
}

void ofApp::updateParameters()
{
	float current_time = ofGetElapsedTimef();
	time_step = ofClamp(current_time - elapsed_time, time_step.getMin(), time_step.getMax());
	elapsed_time = current_time;
	int interval = 100000;
	float value = ofGetElapsedTimeMillis() % interval / float(interval);
	time_value = sin(value * TWO_PI) * 0.5f + 0.5f; // continuous sin value

}