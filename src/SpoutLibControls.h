#pragma once

#include "SpoutControls.h"

#include "ofTexture.h"
#include "ofParameter.h"

namespace SpoutLib
{
	using ControlsRef = std::shared_ptr<class Controls>;

	class Controls
	{
	public:
		static ControlsRef create(const string& spoutName = "OF Spout")
		{
			return ControlsRef(new Controls(spoutName));
		}

		~Controls()
		{
			release();
		}

		void update(ofParameterGroup& parameters)
		{
			if (spout_controls == nullptr)
			{
				spout_controls = new SpoutControls;
				createControls(parameters);
				if (spout_controls->OpenControls(spout_name))
				{
					ofLogNotice(module, "[%s] is created", spout_name.c_str());
				}
				else
				{
					release();
				}

			}
			
			if (spout_controls)
			{
				if (spout_controls->CheckControls(controls))
				{
					printf("%u controls\n", controls.size());
					for (const auto& ctrl : controls)
					{
						updateParameters(ctrl, parameters);
					}
				}
				else
				{
					//release();
				}
					
			}
			
		}

		void openSpoutController()
		{
			if (spout_controls)
				spout_controls->OpenSpoutController();
		}

	private:
		Controls(const string& spoutName)
			:spout_name(spoutName)
		{
		}
		
		void createControls(ofParameterGroup& parameters)
		{
			//string name = parameters[i].getEscapedName();
			//bool value = parameters[i].cast<bool>();
			// types: event, bool, text, float
			for (size_t i = 0; i < parameters.size(); i++)
			{
				if (parameters[i].type() == typeid(ofParameter<string>).name()) {
					ofParameter<string>& param = parameters[i].cast<string>();
					string name = param.getName();
					string value = param.get();
					spout_controls->CreateControl(name, "text", value);
					ofLogNotice(module, "CreateControl text: %s, %s", name.c_str(), value.c_str());
				}
				else if (parameters[i].type() == typeid(ofParameter<bool>).name()) {
					ofParameter<bool>& param = parameters[i].cast<bool>();
					string name = param.getName();
					bool value = param.get();
					spout_controls->CreateControl(name, "bool", int(value));
					ofLogNotice(module, "CreateControl bool: %s, %u", name.c_str(), int(value));
				}
				else if (parameters[i].type() == typeid(ofParameter<float>).name()) {
					ofParameter<float>& param = parameters[i].cast<float>();
					string name = param.getName();
					float value = param.get();
					float minimum = param.getMin();
					float maximum = param.getMax();
					spout_controls->CreateControl(name, "float", minimum, maximum, value);
					ofLogNotice(module, "CreateControl float: %s, %f, %f, %f", name.c_str(), value, minimum, maximum);
				}
				else if (parameters[i].type() == typeid(ofParameterGroup).name()) {
					createControls((ofParameterGroup&)parameters[i]);
				}
			}
		}

		void updateParameters(const control& ctrl, ofParameterGroup& parameters)
		{
			//cout << ctrl.type << endl;
			for (size_t i = 0; i < parameters.size(); i++)
			{
				if (parameters[i].getEscapedName() == ctrl.name)
				{
					if (parameters[i].type() == typeid(ofParameter<string>).name())
					{
						if (ctrl.type == 100)
						{
							printf("string\n");
							printf("set text %s\n", ctrl.text.c_str());
							parameters[i].cast<string>().set(ctrl.text);
							printf("done text %s\n", ctrl.text.c_str());
							continue;
						}
					}
#if 0
					if (parameters[i].type() == typeid(ofParameter<bool>).name())
					{
						if (ctrl.type == 1)
						{
							printf("bool\n");
							printf("set bool %u\n", static_cast<int>(ctrl.value));
							parameters[i].cast<bool>().set(static_cast<int>(ctrl.value) == 1);
							printf("done bool %u\n", static_cast<int>(ctrl.value));
							continue;
						}
					}

					if (parameters[i].type() == typeid(ofParameter<float>).name())
					{
						if (ctrl.type == 10)
						{
							printf("float\n");
							printf("set float %f\n", ctrl.value);
							parameters[i].cast<float>().set(ctrl.value);
							printf("done float %f\n", ctrl.value);
							continue;
						}
					}
#endif
				}

				createControls((ofParameterGroup&)parameters[i]);
			}
		}

		void release()
		{
			if (spout_controls)
			{
				spout_controls->CloseControls();
				delete spout_controls;
				spout_controls = nullptr;
			}
		}

		const std::string module = "SpoutLib::Contorls";

		// controls
		std::string spout_name;
		SpoutControls* spout_controls = nullptr;
		std::vector<control> controls;
	};
}
