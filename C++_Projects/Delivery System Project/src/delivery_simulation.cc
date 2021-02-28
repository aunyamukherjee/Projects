#include "delivery_simulation.h"
#include "entity_base.h"
#include "json_helper.h"
#include "drone.h"

namespace csci3081 {

DeliverySimulation::DeliverySimulation() {}

DeliverySimulation::~DeliverySimulation() {}

IEntity* DeliverySimulation::CreateEntity(const picojson::object& val) {
  //TODO for lab10: replace the ?????'s with the appropriate values,
  //  then uncomment the section of code
 
  if (JsonHelper::GetString(val, "type") == "drone") {
    std::vector<float> position = JsonHelper::GetStdFloatVector(val, "position");
    std::vector<float> direction = JsonHelper::GetStdFloatVector(val, "direction");
    return new Drone(position, direction, val);
  }
 
  return NULL;
}

void DeliverySimulation::AddFactory(IEntityFactory* factory) {}

void DeliverySimulation::AddEntity(IEntity* entity) { 
  //TODO for lab10: One line of code
  entities_.push_back(entity);
}

void DeliverySimulation::SetGraph(const IGraph* graph) {}

void DeliverySimulation::ScheduleDelivery(IEntity* package, IEntity* dest) {}

void DeliverySimulation::AddObserver(IEntityObserver* observer) {}

void DeliverySimulation::RemoveObserver(IEntityObserver* observer) {}

const std::vector<IEntity*>& DeliverySimulation::GetEntities() const { return entities_; }

void DeliverySimulation::Update(float dt) {}


// DO NOT MODIFY THE FOLLOWING UNLESS YOU REALLY KNOW WHAT YOU ARE DOING
void DeliverySimulation::RunScript(const picojson::array& script, IEntitySystem* system) const {
  JsonHelper::PrintArray(script);
  IDeliverySystem* deliverySystem = dynamic_cast<IDeliverySystem*>(system);
	if (deliverySystem) {

	    // To store the unadded entities_
	    std::vector<IEntity*> created_entities;

		for (unsigned int i=0; i < script.size(); i++) {
			const picojson::object& object = script[i].get<picojson::object>();
			const std::string cmd = object.find("command")->second.get<std::string>();
			const picojson::object& params = object.find("params")->second.get<picojson::object>();
			// May want to replace the next few if-statements with an enum
			if (cmd == "createEntity") {
				IEntity* entity = NULL;
				entity = deliverySystem->CreateEntity(params);
				if (entity) {
					created_entities.push_back(entity);
				} else {
					std::cout << "Null entity" << std::endl;
				}
			}
			else if (cmd == "addEntity") {
				int ent_index = static_cast<int>(params.find("index")->second.get<double>());
				if (ent_index >= 0 && ent_index < created_entities.size()) {
					deliverySystem->AddEntity(created_entities[ent_index]);
				}
			}
			else if (cmd == "scheduleDelivery" ) {
				int pkg_index = static_cast<int>(params.find("pkg_index")->second.get<double>());
				int dest_index = static_cast<int>(params.find("dest_index")->second.get<double>());
				if (pkg_index >= 0 && pkg_index < system->GetEntities().size()) {
					IEntity* pkg = deliverySystem->GetEntities()[pkg_index];
					if (dest_index >= 0 && pkg_index < system->GetEntities().size()) {
						IEntity* cst = system->GetEntities()[dest_index];
						if (pkg && cst) {
							deliverySystem->ScheduleDelivery(pkg, cst);
						}
					}
				}
				else {
					std::cout << "Failed to schedule delivery: invalid indexes" << std::endl;
				}
			}
		}
	}
}

}
