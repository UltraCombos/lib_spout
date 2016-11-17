#pragma once

#include "SpoutSender.h"
#include "SpoutControls.h"

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
#if 0
			spout_controls = new SpoutControls;
			spout_controls->CreateControl("User text", "text", "Spout sender control");
			spout_controls->CreateControl("Rotate", "bool", 1);
			spout_controls->CreateControl("Speed", "float", 0.0, 4.0, 0.5);
			spout_controls->OpenControls(spoutName);
#endif
		}

		void updateSender(ofTexture& tex, bool bInvert)
		{
			if (spout_controls)
			{
				if (spout_controls->CheckControls(controls))
				{
					for (int i = 0; i < controls.size(); i++) {
						if (controls.at(i).name == "User text")
							string m_UserText = controls.at(i).text;
						if (controls.at(i).name == "Speed")
							float m_RotationSpeed = controls.at(i).value;
						if (controls.at(i).name == "Rotate")
							bool m_bRotate = ((int)controls.at(i).value == 1);
					}
				}
			}

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
			if (sender)
			{
				sender->ReleaseSender();
				delete sender;
				sender = nullptr;
			}

			if (spout_controls)
			{
				spout_controls->CloseControls();
				delete spout_controls;
				spout_controls = nullptr;
			}
		}

		const std::string module = "SpoutLib";

		// sender
		SpoutSender* sender = nullptr;
		std::string spout_name;
		unsigned int width = 0;
		unsigned int height = 0;
		
		// controls
		SpoutControls* spout_controls = nullptr;
		std::vector<control> controls;

	};
}
