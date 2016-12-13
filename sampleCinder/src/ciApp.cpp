#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "Utilities.h"

#include "SpoutLibSender.h"
#include "SpoutLibReceiver.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ciApp : public App {
public:
	void setup() override;
	void keyDown(KeyEvent event) override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;
	void cleanup() override;
	
private:
	enum {
		FBO_WIDTH = 1920,
		FBO_HEIGHT = 1080
	};
	
	const fs::path data_path = getAppPath().parent_path().parent_path().parent_path().append("/data/");

	double time_step;
	double elapsed_time;
	double time_value;

	gl::FboRef fbo;

	bool is_debug_visible = true;

	gl::TextureRef spout_tex;
	SpoutLib::SenderRef spout_sender;
	SpoutLib::ReceiverRef spout_receiver;
};

void ciApp::setup()
{
	// check data folder
	if (!ci::fs::exists(data_path))
		ci::fs::create_directory(data_path);

	// setup fbo
	gl::Fbo::Format fbo_format;
	fbo_format.setSamples(4);
	fbo_format.setColorTextureFormat(gl::Texture2d::Format().internalFormat(GL_RGBA8));
	fbo = gl::Fbo::create(FBO_WIDTH, FBO_HEIGHT, fbo_format.colorTexture());

	spout_sender = SpoutLib::Sender::create("Cinder Spout");
	spout_receiver = SpoutLib::Receiver::create("");
}

void ciApp::keyDown(KeyEvent event)
{
	switch (event.getCode())
	{
	case KeyEvent::KEY_ESCAPE:
		quit();
		break;
	case KeyEvent::KEY_F11:
		setFullScreen(!isFullScreen());
		break;
	case KeyEvent::KEY_F1:
		is_debug_visible = !is_debug_visible;
		break;
	case KeyEvent::KEY_c:
		spout_receiver->SelectSenderPanel();
		break;
	}
}

void ciApp::mouseDown(MouseEvent event)
{
}

void ciApp::update()
{
	// update params
	double current_time = ci::app::getElapsedSeconds();
	time_step = glm::clamp(current_time - elapsed_time, 1.0 / 120.0, 1.0 / 30.0);
	elapsed_time = current_time;
	int interval = 100000;
	double value = int(current_time * 1000) % interval / double(interval);
	time_value = sin(value * glm::two_pi<double>()) * 0.5 + 0.5; // continuous sin value

	// update fbo
	{
		const gl::ScopedFramebuffer scp_fbo(fbo);
		const gl::ScopedViewport scp_vp(ivec2(0), fbo->getSize());
		gl::clear();

		const gl::ScopedDepth scp_depth(true);
		const gl::ScopedMatrices scp_m;
		gl::setMatricesWindow(fbo->getSize());

		vec2 mouse_pos = ((vec2)getMousePos() / (vec2)getWindowSize()) * (vec2)fbo->getSize();

		gl::drawSolidCircle(mouse_pos, 30.0f);

		gl::drawColorCube(vec3(0), vec3(1, 2, 3));
	}

	spout_sender->update(fbo->getColorTexture());
	if (spout_receiver->update())
	{
		spout_tex = spout_receiver->getTexture();
	}
}

void ciApp::draw()
{
	auto viewport = getWindowBounds();
	gl::clear();
	
	{
		auto left_viewport = viewport;
		left_viewport.setX2(left_viewport.x2 / 2);
		auto rect = Rectf(fbo->getBounds()).getCenteredFit(left_viewport, false);
		gl::draw(fbo->getColorTexture(), rect);		
	}

	if (spout_tex)
	{
		auto right_viewport = viewport;
		right_viewport.setX1(right_viewport.x2 / 2);
		auto rect = Rectf(spout_tex->getBounds()).getCenteredFit(right_viewport, false);
		gl::draw(spout_tex, rect);
	}
		

	if (is_debug_visible)
	{
		const gl::ScopedColor scpColor(Color::white());
		auto tex = getInfoTexture(getAverageFps(), time_step, elapsed_time, time_value);
		ivec2 pos(20, getWindowHeight() - tex->getHeight() - 20);
		gl::draw(tex, pos);
	}
}

void ciApp::cleanup()
{
	console() << "cleanup" << endl;
}

CINDER_APP(ciApp, RendererGl(RendererGl::Options().version(4, 5)), [](App::Settings* settings){
	settings->setWindowSize(1280, 720);
	settings->setConsoleWindowEnabled(true);
	settings->setTitle("Cinder Application");
	settings->setResizable(true);
})