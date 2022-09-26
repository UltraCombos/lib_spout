#pragma once

#include "SpoutSender.h"
#include "SpoutLibUtilities.h"

namespace SpoutLib
{
	using SenderRef = std::shared_ptr<class Sender>;

	class Sender
	{
	public:
		static SenderRef create(const std::string& spoutName = "OF Spout")
		{
			return SenderRef(new Sender(spoutName));
		}

		~Sender()
		{
			release();
		}

		void update(SPOUTLIB_TEX tex, bool bInvert = SPOUTLIB_INVERT, DXGI_FORMAT dwFormat = DXGI_FORMAT_B8G8R8A8_UNORM)
		{
			updateSender(tex, bInvert,dwFormat);
		}
		
		bool isInitialized() { return (sender != nullptr); }
		std::string getName() { return spout_name; }

	private:
		Sender(const std::string& spoutName)
			:spout_name(spoutName)
		{
		}
		
		void updateSender(SPOUTLIB_TEX tex, bool bInvert, DWORD dwFormat)
		{
			if (Util::isAllocated(tex) == false)
			{
				std::printf("[%s] texture is not allocated\n", module.c_str());
				return;
			}

			if (Util::getWidth(tex) != width || Util::getHeight(tex) != height)
			{
				release();
				width = Util::getWidth(tex);
				height = Util::getHeight(tex);
			}

			if (sender == nullptr)
			{
				sender = new SpoutSender;
				if (sender->CreateSender(spout_name.c_str(), width, height, dwFormat))
				{
					std::printf("[%s] '%s' is created\n", module.c_str(), spout_name.c_str());
				}
				else
				{
					release();
				}
			}

			if (sender)
			{
				GLuint id = Util::getId(tex);
				GLenum target = Util::getTarget(tex);
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
