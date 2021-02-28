#include "gtest/gtest.h"
#include <EntityProject/project_settings.h>
#include "../include/delivery_simulation.h"
#include <EntityProject/entity.h>
#include "json_helper.h"


#include <iostream>

namespace csci3081 {

using entity_project::IEntity;

class FactoryTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = dynamic_cast<IDeliverySystem*>(GetEntitySystem("default"));
  }
  virtual void TearDown() {}

  IDeliverySystem* system;
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(FactoryTest, DroneCreated) {
  picojson::object obj = JsonHelper::CreateJsonObject();
  JsonHelper::AddStringToJsonObject(obj, "type", "drone");
  std::vector<float> position_to_add;
  position_to_add.push_back(498.292);
  position_to_add.push_back(253.883);
  position_to_add.push_back(-228.623);
  JsonHelper::AddStdFloatVectorToJsonObject(obj, "position", position_to_add);
  std::vector<float> direction_to_add;
  direction_to_add.push_back(1);
  direction_to_add.push_back(0);
  direction_to_add.push_back(0);
  JsonHelper::AddStdFloatVectorToJsonObject(obj, "direction", direction_to_add);
  IEntity* entity = system->CreateEntity(obj);

  // Checks that the returned entity is not NULL
  ASSERT_NE(entity, nullptr) << "The entity created";
  system->AddEntity(entity);
  ASSERT_EQ(1, system->GetEntities().size());
  ASSERT_FLOAT_EQ(entity->GetPosition()[0], position_to_add[0]);
  ASSERT_FLOAT_EQ(entity->GetPosition()[1], position_to_add[1]);
  ASSERT_FLOAT_EQ(entity->GetPosition()[2], position_to_add[2]);
  ASSERT_FLOAT_EQ(entity->GetDirection()[0], direction_to_add[0]);
  ASSERT_FLOAT_EQ(entity->GetDirection()[1], direction_to_add[1]);
  ASSERT_FLOAT_EQ(entity->GetDirection()[2], direction_to_add[2]);

  // Checks that when GetDetails() is called, the entity returns 
  //  the picojson object that was used to initialize it
  ASSERT_EQ(picojson::value(system->GetEntities()[0]->GetDetails()).serialize(), picojson::value(obj).serialize());

}
}  // namespace csci3081
