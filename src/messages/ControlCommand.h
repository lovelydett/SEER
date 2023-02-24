//
// Created by Yuting Xie on 24/02/2023.
//

#ifndef GOGO_PROJ_CONTROLCOMMAND_H
#define GOGO_PROJ_CONTROLCOMMAND_H

#include "../gogoRT/Message.h"

namespace message {

class ControlCommand : public gogort::Message {
private:
  float velocity_;
  float steering_angle_;

public:
  float get_velocity() const { return velocity_; }
  void set_velocity(float velocity) { velocity_ = velocity; }
  float get_steering_angle() const { return steering_angle_; }
  void set_steering_angle(float steering_angle) {
    steering_angle_ = steering_angle;
  }
  std::string to_string() override {
    return "ControlCommand: velocity: " + std::to_string(velocity_) +
           " steering_angle: " + std::to_string(steering_angle_);
  };
};

} // namespace message

#endif // GOGO_PROJ_CONTROLCOMMAND_H
