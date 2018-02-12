#pragma once

#include "SpoutReceiver.h"
#include "SpoutLibUtilities.h"

namespace SpoutLib
{
	using ReceiverRef = std::shared_ptr<class Receiver>;

	class Receiver
	{
	public:
		static ReceiverRef create(const std::string& spoutName = "", GLuint glFormat = GL_RGBA8)
		{
			return ReceiverRef(new Receiver(spoutName, glFormat));
		}
		/*
		~Receiver()
		{
			releaseTexture();
		}
		*/
		bool isInitialized()
		{
			return bInitialized;
		}

		bool update(bool bInvert = SPOUTLIB_INVERT)
		{
			return updateReceiver(bInvert);
		}

		void SelectSenderPanel()
		{
				receiver.SelectSenderPanel();
		}

		std::string getName() { return mutableName; }
		void setName(std::string const& name) 
		{ 
			if (strncmp(mutableName, name.c_str(), sizeof(mutableName)) != 0)
			{
				mf_setName(name);
				releaseTexture();
			}
		}
		int getWidth() { return width; }
		int getHeight() { return height; }
		SPOUTLIB_TEX getTexture() { return texture; }

	private:
		Receiver(const std::string& spoutName,GLuint glFormat)
		: glInternalFormat(glFormat)
		{
			mf_setName(spoutName);
		}

		void mf_setName(std::string const& spoutName)
		{
			strcpy_s(mutableName, std::min<size_t>(spoutName.size() + 1, sizeof(mutableName)), spoutName.c_str());
		}

		bool updateReceiver(bool bInvert)
		{
			if (bInitialized == false)
			{
				if (receiver.CreateReceiver(mutableName, width, height, false))
				{
					printf("[%s] '%s' is created %ux%u\n", module.c_str(), mutableName, width, height);
					allocateTexture();
					bInitialized = true;
					return true;
				}
				else
				{
					//releaseTexture();
					return false;
				}
			}
			else
			{
				GLuint id = Util::getId(texture);
				GLenum target = Util::getTarget(texture);
				if (receiver.ReceiveTexture(mutableName, width, height, id, target, bInvert))
				{
					if (width != Util::getWidth(texture) || height != Util::getHeight(texture))
					{
						allocateTexture();
					}
					return true;
				}
				else
				{
					releaseTexture();
					receiver.ReleaseReceiver();
					bInitialized = false;
					return false;
				}
			}
		}

		void releaseTexture()
		{			
#ifdef CINDER_CINDER
#error "Need to clear texture in Cinder"
#else
			texture.clear(); 
#endif
		}

		void allocateTexture()
		{
#ifdef CINDER_CINDER
			texture = ci::gl::Texture2d::create(width, height, ci::gl::Texture2d::Format().internalFormat(glInternalFormat));
#else
			texture.allocate(width, height, glInternalFormat);
#endif
		}

		const std::string module = "SpoutLib::Receiver";

		// receiver
		bool bInitialized = false;
		char mutableName[256];
		SpoutReceiver receiver;
		unsigned int width = 0;
		unsigned int height = 0;
		GLint glInternalFormat;
#ifdef CINDER_CINDER
		ci::gl::Texture2dRef texture = nullptr;
#else
		ofTexture texture;
#endif
	};
}
