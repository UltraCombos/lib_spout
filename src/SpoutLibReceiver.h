#pragma once

#include "SpoutReceiver.h"
#include "SpoutLibUtilities.h"

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

		void update(SPOUT_TEX tex, bool bInvert = false)
		{
			updateReceiver(tex, bInvert);
		}

		void SelectSenderPanel()
		{
			if (receiver)
				receiver->SelectSenderPanel();
		}

		bool isInitialized() { return (receiver != nullptr); }
		std::string getName() { return spout_name; }
		void setName(std::string name) { spout_name = name; release(); }
		int getWidth() { return width; }
		int getHeight() { return height; }

	private:
		Receiver(const string& spoutName)
			:spout_name(spoutName)
		{
		}

		void updateReceiver(SPOUT_TEX tex, bool bInvert)
		{
			if (receiver == nullptr)
			{
				receiver = new SpoutReceiver;
				char mutableName[256];
				strcpy_s(mutableName, spout_name.size() + 1, spout_name.c_str());
				if (receiver->CreateReceiver(mutableName, width, height, true))
				{
					spout_name = std::string(mutableName);
					ofLogNotice(module, "[%s] is created %ux%u", spout_name.c_str(), width, height);
					int frt = isAllocated(tex) ? getInternalFormat(tex) : glInternalFormat;
					allocate(tex, width, height, frt);
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
				GLuint id = getId(tex);
				GLenum target = getTarget(tex);
				if (receiver->ReceiveTexture(mutableName, width, height, id, target, bInvert))
				{
					spout_name = std::string(mutableName);
					if (width != SpoutLib::getWidth(tex) || height != SpoutLib::getHeight(tex))
					{
						int frt = getInternalFormat(tex);
						allocate(tex, width, height, frt);
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
			if (receiver)
			{
				receiver->ReleaseReceiver();
				delete receiver;
				receiver = nullptr;
			}
		}

		const string module = "SpoutLib::Receiver";

		// receiver
		SpoutReceiver* receiver = nullptr;
		std::string spout_name;
		unsigned int width = 0;
		unsigned int height = 0;
		GLint glInternalFormat = GL_RGBA8;
	};
}
