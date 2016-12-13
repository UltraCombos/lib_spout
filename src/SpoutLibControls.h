#pragma once

#include "SpoutControls.h"
#include "json.hpp"

#include "ofParameter.h"

namespace SpoutLib
{
	using ControlsRef = std::shared_ptr<class Controls>;

	class Controls
	{
	public:
		static ControlsRef create(const string& spoutName = "")
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
					printf("[%s] '%s' is created\n", module.c_str(), spout_name.c_str());
				}
				else
				{
					printf("[%s] '%s' create fail\n", module.c_str(), spout_name.c_str());
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
							printf("[%s] get contorl '%s' is unknown\n", module.c_str(), ctrl.name.c_str());
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
							printf("[%s] get contorl '%s' type(%u) is unknown\n", module.c_str(), ctrl.name.c_str(), ctrl.type);
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
				string file_path;
				if (spout_controls->FindControlFile(file_path))
				{
					parseControls(file_path);
				}
				string map_name;
				if (spout_controls->FindControls(map_name))
				{
					cout << "get " << map_name << endl;
					parameters.setName(map_name);
					for (auto& ctrl : controls)
					{
						switch (ctrl.type)
						{
						case 100:
							parameters.add(ofParameter<string>().set(ctrl.name, ctrl.text)); break;
						case 0:
							parameters.add(ofParameter<bool>().set(ctrl.name, static_cast<bool>(ctrl.value))); break;
						case 10:
							parameters.add(ofParameter<float>().set(ctrl.name, ctrl.value)); break;
						}
					}
				}
				if (spout_controls->CreateControls(map_name, controls))
				{
					cout << "create controls" << endl;
				}			

			}

			if (spout_controls)
			{
				for (auto& ctrl : controls)
				{
					if (ctrl.type == 10)
						ctrl.value = ABS(sin(ofGetElapsedTimef()));
				}
				if (spout_controls->SetControls(controls))
				{
					cout << "set ok" << endl;
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
			control_map.clear();
			for (size_t i = 0; i < parameters.size(); i++)
			{
				if (parameters[i].type().compare(typeid(ofParameter<string>).name()) == 0) {
					ofParameter<string>& param = parameters[i].cast<string>();
					string name = param.getName();
					string value = param.get();
					spout_controls->CreateControl(name, "text", value);
					control_map[name] = param.newReference();
					printf("[%s] CreateControl text: %s, %s\n", module.c_str(), name.c_str(), value.c_str());
				}
				else if (parameters[i].type().compare(typeid(ofParameter<bool>).name()) == 0) {
					ofParameter<bool>& param = parameters[i].cast<bool>();
					string name = param.getName();
					bool value = param.get();
					spout_controls->CreateControl(name, "bool", int(value));
					control_map[name] = param.newReference();
					printf("[%s] CreateControl bool: %s, %u\n", module.c_str(), name.c_str(), int(value));
				}
				else if (parameters[i].type().compare(typeid(ofParameter<float>).name()) == 0) {
					ofParameter<float>& param = parameters[i].cast<float>();
					string name = param.getName();
					float value = param.get();
					float minimum = param.getMin();
					float maximum = param.getMax();
					spout_controls->CreateControl(name, "float", minimum, maximum, value);
					control_map[name] = param.newReference();
					printf("[%s] CreateControl float: %s, %f, %f, %f\n", module.c_str(), name.c_str(), value, minimum, maximum);
				}
				else if (parameters[i].type().compare(typeid(ofParameterGroup).name()) == 0) {
					createControls((ofParameterGroup&)parameters[i]);
				}
			}
		}

		void parseControls(const string& file_path)
		{
			cout << "get " << file_path << endl;
			std::string doc = ofBufferFromFile(file_path).getText();
			size_t from_idx = doc.find("INPUTS") - 1;
			size_t to_idx = doc.find_last_of(',');
			doc = doc.substr(from_idx, to_idx - from_idx);
			to_idx = doc.find_last_of(',');
			doc = "{" + doc.substr(0, to_idx) + "]}";
			ofBufferToFile("spout_control.json", ofBuffer(doc));
			auto& document = nlohmann::json::parse(doc.c_str());

			if (document.find("INPUTS") != document.end())
			{
				//cout << "ok" << endl;
				auto& inputs = document["INPUTS"];
				for (size_t i = 0; i < inputs.size(); i++)
				{
					auto& input = inputs[i];
					control ctrl;
					ctrl.name = input.value("NAME", "");
					string type = input.value("TYPE", "");
					if (type == "text")
					{
						ctrl.type = 100;
						ctrl.text = input.value("TEXT", "");
					}
					else if (type == "bool")
					{
						ctrl.type = 0;
						ctrl.value = input.value("DEFAULT", 1.0f);
					}
					else if (type == "float")
					{
						ctrl.type = 10;
						ctrl.value = input.value("DEFAULT", 1.0f);
					}
					controls.push_back(ctrl);
				}
			}
			else
			{
				cout << "INPUTS is not found" << endl;
			}
		}

		template<typename T>
		void setValue(std::shared_ptr<ofAbstractParameter> p, T v)
		{
			p->cast<T>().set(v);
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

		std::unordered_map<std::string, std::shared_ptr<ofAbstractParameter>> control_map;
	};
}
