#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace std;

namespace
{
	template <typename T> std::string tostr(const T& t, int p) { ostringstream os; os << std::setprecision(p) << std::fixed << t; return os.str(); }
	template <typename T> std::string tostr(const T& t, int p, char f) { ostringstream os; os << std::fixed << std::setfill(f) << std::setw(p) << t; return os.str(); }
	
	ci::gl::TextureRef getInfoTexture(float fps, double timeStep, double elapsedTime, double timeValue)
	{

		ci::TextLayout info_text;
		info_text.clear(ci::ColorA(0.2f, 0.2f, 0.2f, 0.5f));
		info_text.setFont(ci::Font("Arial", 17));
		info_text.setBorder(4, 2);
		//info_text.setColor(ci::Color(0.9f, 0.9f, 0.9f));
		info_text.setColor(ci::ColorA::hex(0xFFFAD5));
		info_text.addLine("app framerate: " + tostr(fps, 2));
		info_text.setColor(ci::ColorA::hex(0x105B63));
		info_text.setColor(ci::ColorA::hex(0xBD4932));
		info_text.addLine("time step: " + tostr(timeStep, 4));
		info_text.addLine("elapsed time: " + tostr(elapsedTime, 2));
		info_text.addLine("time value: " + tostr(timeValue, 3));
		return ci::gl::Texture::create(info_text.render(true));
	}
}