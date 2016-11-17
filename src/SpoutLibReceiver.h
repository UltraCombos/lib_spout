#pragma once

#include "SpoutReceiver.h"

#include <memory>
#include "ofTexture.h"

namespace SpoutLib
{
	using ReceiverRef = std::shared_ptr<class Receiver>;

	class Receiver
	{
	public:
		static ReceiverRef create(const string& spoutName = "OF Spout")
		{
			return ReceiverRef(new Receiver(spoutName));
		}

		~Receiver()
		{
			release();
		}

		void update(ofTexture& tex, bool bInvert = false)
		{
			updateReceiver(tex, bInvert);
		}

		void SelectSenderPanel()
		{
			if (receiver)
				receiver->SelectSenderPanel();
		}

	private:
		Receiver(const string& spoutName)
			:spout_name(spoutName)
		{
		}

		void updateReceiver(ofTexture& tex, bool bInvert)
		{
			if (receiver == nullptr)
			{
				receiver = new SpoutReceiver;
				char mutableName[256];
				strcpy_s(mutableName, spout_name.size() + 1, spout_name.c_str());
				if (receiver->CreateReceiver(mutableName, width, height, true))
				{
					spout_name = std::string(mutableName);
					ofLogNotice(module, "receiver %s is created %ux%u", spout_name.c_str(), width, height);
					tex.allocate(width, height, GL_RGBA);
				}
				else
				{
					release();
				}
			}

			if (receiver)
			{
				char mutableName[256];
				strcpy_s(mutableName, spout_name.size() + 1, spout_name.c_str());
				GLuint id = tex.getTextureData().textureID;
				GLuint target = tex.getTextureData().textureTarget;
				if (receiver->ReceiveTexture(mutableName, width, height, id, target, bInvert))
				{
					spout_name = std::string(mutableName);
					if (width != tex.getWidth() || height != tex.getHeight())
					{
						tex.allocate(width, height, GL_RGBA);
					}
				}
				else
				{
					release();
				}
			}
		}

		void release()
		{
			if (receiver != nullptr)
			{
				receiver->ReleaseReceiver();
				delete receiver;
				receiver = nullptr;
			}
		}

		SpoutReceiver* receiver = nullptr;
		std::string spout_name;
		unsigned int width = 0;
		unsigned int height = 0;
		const string module = "SpoutLib";
	};
}
