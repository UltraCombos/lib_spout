#pragma once

#ifdef CINDER_CINDER
#include "cinder/gl/Texture.h"
#define TEXTURE gl::Texture2dRef
#else
#include <memory>
#include "ofTexture.h"
#define SPOUT_TEX ofTexture&
#endif

namespace SpoutLib
{
	static bool isAllocated(SPOUT_TEX tex)
	{
#ifdef CINDER_CINDER
		return (tex != nullptr);
#else
		return tex.isAllocated();
#endif
	}

	static int getWidth(SPOUT_TEX tex)
	{
#ifdef CINDER_CINDER
		return tex->getWidth();
#else
		return tex.getWidth();
#endif
	}

	static int getHeight(SPOUT_TEX tex)
	{
#ifdef CINDER_CINDER
		return tex->getHeight();
#else
		return tex.getHeight();
#endif
	}

	static GLint getInternalFormat(SPOUT_TEX tex)
	{
#ifdef CINDER_CINDER
		return tex->getInternalFormat();
#else
		return tex.getTextureData().glInternalFormat;
#endif
	}

	static GLuint getId(SPOUT_TEX tex)
	{
#ifdef CINDER_CINDER
		return tex->getId();
#else
		return tex.getTextureData().textureID;
#endif
	}

	static GLenum getTarget(SPOUT_TEX tex)
	{
#ifdef CINDER_CINDER
		return tex->getTarget();
#else
		return tex.getTextureData().textureTarget;
#endif
	}

	static void allocate(SPOUT_TEX tex, int width, int height, GLint glInternalFormat)
	{
#ifdef CINDER_CINDER
		return tex->getTarget();
#else
		tex.allocate(width, height, glInternalFormat);
#endif
	}
}