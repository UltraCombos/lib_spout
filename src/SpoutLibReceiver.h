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
		bool update(bool bInvert = SPOUTLIB_INVERT)
		{
			return updateReceiver(bInvert);
		}

		void SelectSenderPanel()
		{
				receiver.SelectSenderPanel();
		}

		std::string getName() { return spout_name; }
		void setName(std::string name) 
		{ 
			if (name.compare(spout_name) != 0)
			{
				spout_name = name; 
				releaseTexture();
			}
		}
		int getWidth() { return width; }
		int getHeight() { return height; }
		SPOUTLIB_TEX getTexture() { return texture; }

	private:
		Receiver(const std::string& spoutName,GLuint glFormat)
			:spout_name(spoutName)
			, glInternalFormat(glFormat)
		{
		}

		bool updateReceiver(bool bInvert)
		{
			if (bInitialized == false)
			{
				char mutableName[256];
				strcpy_s(mutableName, spout_name.size() + 1, spout_name.c_str());
				if (receiver.CreateReceiver(mutableName, width, height, false))
				{
					spout_name = std::string(mutableName);
					printf("[%s] '%s' is created %ux%u\n", module.c_str(), spout_name.c_str(), width, height);
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
				char mutableName[256];
				strcpy_s(mutableName, spout_name.size() + 1, spout_name.c_str());
				GLuint id = Util::getId(texture);
				GLenum target = Util::getTarget(texture);
				if (receiver.ReceiveTexture(mutableName, width, height, id, target, bInvert))
				{
					spout_name = std::string(mutableName);
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
		SpoutReceiver receiver;
		std::string spout_name;
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
