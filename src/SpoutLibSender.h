#pragma once

#include "SpoutSender.h"

#include <memory>
#include "ofTexture.h"

namespace SpoutLib
{
	using SenderRef = std::shared_ptr<class Sender>;

	class Sender
	{
	public:
		static SenderRef create(const string& spoutName = "OF Spout")
		{
			return SenderRef(new Sender(spoutName));
		}

		~Sender()
		{
			release();
		}

		void update(ofTexture& tex, bool bInvert = false)
		{
			updateSender(tex, bInvert);
		}

	private:
		Sender(const string& spoutName)
			:spout_name(spoutName)
		{
		}

		void updateSender(ofTexture& tex, bool bInvert)
		{
			if (tex.isAllocated() == false)
			{
				ofLogWarning(module, "texture is not allocated");
				return;
			}

			if (tex.getWidth() != width || tex.getHeight() != height)
			{
				release();
				width = tex.getWidth();
				height = tex.getHeight();
			}

			if (sender == nullptr)
			{
				sender = new SpoutSender;
				if (sender->CreateSender(spout_name.c_str(), width, height))
				{
					ofLogNotice(module, "sender %s is created", spout_name.c_str());
				}
				else
				{
					release();
				}
			}

			if (sender)
			{
				GLuint id = tex.getTextureData().textureID;
				GLuint target = tex.getTextureData().textureTarget;
				if (sender->SendTexture(id, target, width, height, bInvert) == false)
				{
					release();
				}
			}
		}
		
		void release()
		{
			if (sender != nullptr)
			{
				sender->ReleaseSender();
				delete sender;
				sender = nullptr;
			}
		}

		SpoutSender* sender = nullptr;
		std::string spout_name;
		unsigned int width = 0;
		unsigned int height = 0;
		const string module = "SpoutLib";
	};
}
