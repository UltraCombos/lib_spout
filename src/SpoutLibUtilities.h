#pragma once

#ifdef CINDER_CINDER
#include "cinder/gl/Texture.h"
#define TEXTURE gl::Texture2dRef
#else
#include <memory>
#include "ofTexture.h"
#define TEXTURE ofTexture&
#endif

namespace SpoutLib
{
	bool isAllocated(TEXTURE tex)
	{
#ifdef CINDER_CINDER
		return (tex != nullptr);
#else

#endif
	}

	int getWidth(TEXTURE tex)
	{
#ifdef CINDER_CINDER
		return tex->getWidth();
#else

#endif
	}

	int getHeight(TEXTURE tex)
	{
#ifdef CINDER_CINDER
		return tex->getHeight();
#else

#endif
	}

	GLint getInternalFormat(TEXTURE tex)
	{
#ifdef CINDER_CINDER
		return tex->getInternalFormat();
#else

#endif
	}

	GLuint getId(TEXTURE tex)
	{
#ifdef CINDER_CINDER
		return tex->getId();
#else

#endif
	}

	GLenum getTarget(TEXTURE tex)
	{
#ifdef CINDER_CINDER
		return tex->getTarget();
#else

#endif
	}
}