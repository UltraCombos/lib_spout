#pragma once

#include "SpoutSender.h"
#include "SpoutLibUtilities.h"

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

		void update(SPOUT_TEX tex, bool bInvert = false)
		{
			updateSender(tex, bInvert);
		}

	private:
		Sender(const string& spoutName)
			:spout_name(spoutName)
		{
		}
		
		void updateSender(SPOUT_TEX tex, bool bInvert)
		{
			if (isAllocated(tex) == false)
			{
				ofLogWarning(module, "texture is not allocated");
				return;
			}

			if (getWidth(tex) != width || getHeight(tex) != height)
			{
				release();
				width = getWidth(tex);
				height = getHeight(tex);
			}

			if (sender == nullptr)
			{
				sender = new SpoutSender;
				if (sender->CreateSender(spout_name.c_str(), width, height))
				{
					ofLogNotice(module, "[%s] is created", spout_name.c_str());
				}
				else
				{
					release();
				}
			}

			if (sender)
			{
				GLuint id = getId(tex);
				GLenum target = getTarget(tex);
				if (sender->SendTexture(id, target, width, height, bInvert) == false)
				{
					release();
				}
			}
		}
		
		void release()
		{
			if (sender)
			{
				sender->ReleaseSender();
				delete sender;
				sender = nullptr;
			}
		}

		const std::string module = "SpoutLib::Sender";

		// sender
		SpoutSender* sender = nullptr;
		std::string spout_name;
		unsigned int width = 0;
		unsigned int height = 0;

	};
}
