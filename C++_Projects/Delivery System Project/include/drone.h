/**
 * @file drone.h
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "entity_base.h"
#include <vector>
#include <string>

namespace csci3081 {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief This is the drone class that controls much of the functionality of the delivery system
 *
 * This class will define actions related to the drone and related classes.
 */

class Drone : public csci3081::EntityBase {
  // TODO: Add documentation to these functions
 public:
   /**
   * @brief Constructor: initializes the position and direction vectors, as well as the name
   * @param vector position, vector direct and picjson obj for additional information
   */
  Drone(std::vector<float> pos, std::vector<float> dir, const picojson::object& obj);
  /**
   * @brief Denoted Id of drone
   * @return Returns the Id of the drone.
   */
  int GetId() const;
  /**
   * @brief Denotes name of Drone
   * @return Returns name of Drone.
   */
  const std::string& GetName();
  /**
   * @brief Denotes the current position of the drone
   * @return Returns current Position vector.
   */
  const std::vector<float>& GetPosition() const;
  /**
   * @brief Denotes the current orientation of the Drone
   * @return Returns current Direction vector.
   */
  const std::vector<float>& GetDirection() const;
  /**
   * @brief Useful function to get the radius of a drone
   * @return Returns Drone radius.
   */
  float GetRadius() const;
  /**
   * @brief Denotes the version
   * @return Returns Version integer value
   */
  int GetVersion() const;
  /**
   * @brief Denotes whether or not a drone is dynamic. 
   * @return Returns bool 
   */
  bool IsDynamic() const;

  //TODO for lab10: What member variables are needed?
  // should they be private or public?
  private:
  std::string name;
  std::vector<float> position;
  std::vector<float> direction;
};

}
