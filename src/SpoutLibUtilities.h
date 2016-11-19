#pragma once

#ifdef CINDER_CINDER
#include "cinder/gl/Texture.h"
#define SPOUTLIB_TEX ci::gl::Texture2dRef
#define SPOUTLIB_INVERT true
#else
#include <memory>
#include "ofTexture.h"
#define SPOUTLIB_TEX ofTexture&
#define SPOUTLIB_INVERT false
#endif

namespace SpoutLib { namespace Util
{
	
	static bool isAllocated(SPOUTLIB_TEX tex)
	{
#ifdef CINDER_CINDER
		return (tex != nullptr);
#else
		return tex.isAllocated();
#endif
	}

	static int getWidth(SPOUTLIB_TEX tex)
	{
#ifdef CINDER_CINDER
		return tex->getWidth();
#else
		return tex.getWidth();
#endif
	}

	static int getHeight(SPOUTLIB_TEX tex)
	{
#ifdef CINDER_CINDER
		return tex->getHeight();
#else
		return tex.getHeight();
#endif
	}

	static GLint getInternalFormat(SPOUTLIB_TEX tex)
	{
#ifdef CINDER_CINDER
		return tex->getInternalFormat();
#else
		return tex.getTextureData().glInternalFormat;
#endif
	}

	static GLuint getId(SPOUTLIB_TEX tex)
	{
#ifdef CINDER_CINDER
		return tex->getId();
#else
		return tex.getTextureData().textureID;
#endif
	}

	static GLenum getTarget(SPOUTLIB_TEX tex)
	{
#ifdef CINDER_CINDER
		return tex->getTarget();
#else
		return tex.getTextureData().textureTarget;
#endif
	}

}}