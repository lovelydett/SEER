//
// Created by Yuting Xie on 08/02/2023.
//

#ifndef GOGO_PROJ_DUMMYRISK1DETECTOR_H
#define GOGO_PROJ_DUMMYRISK1DETECTOR_H

#include "../../Comm/Pipe.h"
#include "../../Comm/PipeReader.h"
#include "../Detector.h"
#include "../risk_message/DummyRisk1.h"

#include <memory>

namespace gogort {

class DummyRisk1Detector : public Detector {
private:
  PipeReader<message::DummyRisk1> reader_;

public:
  DummyRisk1Detector() = delete;
  // Pipe reader should be explicitly initialized with a pipe
  DummyRisk1Detector(std::shared_ptr<Pipe> sensor_pipe);
  std::shared_ptr<Risk> anticipate_once() override;
};

} // namespace gogort

#endif // GOGO_PROJ_DUMMYRISK1DETECTOR_H
