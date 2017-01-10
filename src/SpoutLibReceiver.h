#pragma once

#include "SpoutReceiver.h"
#include "SpoutLibUtilities.h"

namespace SpoutLib
{
	using ReceiverRef = std::shared_ptr<class Receiver>;

	class Receiver
	{
	public:
		static ReceiverRef create(const std::string& spoutName = "")
		{
			return ReceiverRef(new Receiver(spoutName));
		}

		~Receiver()
		{
			release();
		}

		bool update(bool bInvert = SPOUTLIB_INVERT)
		{
			return updateReceiver(bInvert);
		}

		void SelectSenderPanel()
		{
			if (receiver)
				receiver->SelectSenderPanel();
		}

		std::string getName() { return spout_name; }
		void setName(std::string name) 
		{ 
			if (name.compare(spout_name) != 0)
			{
				spout_name = name; 
				release();
			}
		}
		int getWidth() { return width; }
		int getHeight() { return height; }
		SPOUTLIB_TEX getTexture() { return texture; }

	private:
		Receiver(const std::string& spoutName)
			:spout_name(spoutName)
		{
		}

		bool updateReceiver(bool bInvert)
		{
			if (receiver == nullptr)
			{
				receiver = new SpoutReceiver;
				char mutableName[256];
				strcpy_s(mutableName, spout_name.size() + 1, spout_name.c_str());
				if (receiver->CreateReceiver(mutableName, width, height, true))
				{
					spout_name = std::string(mutableName);
					printf("[%s] '%s' is created %ux%u\n", module.c_str(), spout_name.c_str(), width, height);
					allocateTexture();
					return true;
				}
				else
				{
					release();
					return false;
				}
			}
			else
			{
				char mutableName[256];
				strcpy_s(mutableName, spout_name.size() + 1, spout_name.c_str());
				GLuint id = Util::getId(texture);
				GLenum target = Util::getTarget(texture);
				if (receiver->ReceiveTexture(mutableName, width, height, id, target, bInvert))
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
					release();
					return false;
				}
			}
		}

		void release()
		{
			if (receiver)
			{
				texture.clear();
				receiver->ReleaseReceiver();
				delete receiver;
				receiver = nullptr;
			}
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
		SpoutReceiver* receiver = nullptr;
		std::string spout_name;
		unsigned int width = 0;
		unsigned int height = 0;
		GLint glInternalFormat = GL_RGBA8;
#ifdef CINDER_CINDER
		ci::gl::Texture2dRef texture = nullptr;
#else
		ofTexture texture;
#endif
	};
}
