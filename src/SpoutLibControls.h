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

		void send(ofParameterGroup& parameters)
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
					for (const auto& ctrl : controls)
					{
						if (control_map.find(ctrl.name) == control_map.end())
						{
							ofLogWarning(module, "get contorl [%s] is unknown", ctrl.name.c_str());
							continue;
						}
						switch (ctrl.type)
						{
						case 100: 
							setValue(control_map[ctrl.name], ctrl.text); break;
						case 0: 
							setValue(control_map[ctrl.name], static_cast<int>(ctrl.value) == 1); break;
						case 10: 
							setValue(control_map[ctrl.name], ctrl.value); break;
						default:
							ofLogWarning(module, "get contorl [%s] type(%u) is unknown", ctrl.name.c_str(), ctrl.type);
							break;
						}
					}
				}					
			}			
		}

		void receive(ofParameterGroup& parameters)
		{
			if (spout_controls == nullptr)
			{
				spout_controls = new SpoutControls;
				//spout_controls->FindControls(spout_name);

				string file_path;
				if (spout_controls->FindControlFile(file_path))
				{
					
					//cout << "get " << file_path << endl;
				}
				string map_name = spout_name;
				if (spout_controls->FindControls(map_name))
				{
					cout << "get " << map_name << endl;
				}
				if (spout_controls->OpenControls(spout_name))
				{
					ofLogNotice(module, "[%s] is created", spout_name.c_str());
				}
				else
				{
					//cout << "check" << endl;
					release();
				}

			}

			if (spout_controls)
			{
				
				if (spout_controls->CheckControls(controls))
				{
					
					for (const auto& ctrl : controls)
					{
						if (control_map.find(ctrl.name) != control_map.end())
							continue;
						if (ctrl.type == 100) {
							ofParameter<std::string> p;
							parameters.add(p.set(ctrl.name, ctrl.text));
							control_map[ctrl.name] = p.newReference();
							cout << "add " << ctrl.name << endl;
						}
						else if (ctrl.type == 0) {
							ofParameter<bool> p;
							parameters.add(p.set(ctrl.name, static_cast<int>(ctrl.value) == 1));
							control_map[ctrl.name] = p.newReference();
							cout << "add " << ctrl.name << endl;
						}
						else if (ctrl.type == 10) {
							ofParameter<float> p;
							parameters.add(p.set(ctrl.name, ctrl.value));
							control_map[ctrl.name] = p.newReference();
							cout << "add " << ctrl.name << endl;
						}
						else {
							ofLogWarning(module, "get contorl [%s] type(%u) is unknown", ctrl.name.c_str(), ctrl.type);
						}
					}
				}
				if (spout_controls->SetControls(controls) == false)
					printf("fffffff\n");
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
					control_map[name] = param.newReference();
					ofLogNotice(module, "CreateControl text: %s, %s", name.c_str(), value.c_str());
				}
				else if (parameters[i].type() == typeid(ofParameter<bool>).name()) {
					ofParameter<bool>& param = parameters[i].cast<bool>();
					string name = param.getName();
					bool value = param.get();
					spout_controls->CreateControl(name, "bool", int(value));
					control_map[name] = param.newReference();
					ofLogNotice(module, "CreateControl bool: %s, %u", name.c_str(), int(value));
				}
				else if (parameters[i].type() == typeid(ofParameter<float>).name()) {
					ofParameter<float>& param = parameters[i].cast<float>();
					string name = param.getName();
					float value = param.get();
					float minimum = param.getMin();
					float maximum = param.getMax();
					spout_controls->CreateControl(name, "float", minimum, maximum, value);
					control_map[name] = param.newReference();
					ofLogNotice(module, "CreateControl float: %s, %f, %f, %f", name.c_str(), value, minimum, maximum);
				}
				else if (parameters[i].type() == typeid(ofParameterGroup).name()) {
					createControls((ofParameterGroup&)parameters[i]);
				}
			}
		}

		template<typename T>
		void setValue(std::shared_ptr<ofAbstractParameter> p, T v)
		{
			p->cast<T>().set(v);
		}

#if 0
		void updateParameters(const control& ctrl, ofParameterGroup& parameters)
		{
			for (size_t i = 0; i < parameters.size(); i++)
			{
				if (parameters[i].getEscapedName() == ctrl.name)
				{
					if (parameters[i].type() == typeid(ofParameter<string>).name()) {
						if (ctrl.type == 100)
						{
							parameters[i].cast<string>().set(ctrl.text);
							return;
						}
					}
					else if (parameters[i].type() == typeid(ofParameter<bool>).name()) {
						if (ctrl.type == 0)
						{
							parameters[i].cast<bool>().set(static_cast<int>(ctrl.value) == 1);
							return;
						}
					}
					else if (parameters[i].type() == typeid(ofParameter<float>).name()) {
						if (ctrl.type == 10)
						{
							parameters[i].cast<float>().set(ctrl.value);
							return;
						}
					}
					else if (parameters[i].type() == typeid(ofParameterGroup).name()) {
						updateParameters(ctrl, (ofParameterGroup&)parameters[i]);
					}
				}
			}
		}
#endif
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

		std::unordered_map<std::string, std::shared_ptr<ofAbstractParameter>> control_map;
	};
}
