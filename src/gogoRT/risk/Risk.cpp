//
// Created by Yuting Xie on 22/02/2023.
//

#include "Risk.h"

namespace gogort {
Risk::Risk(const float zeta, const float kappa) : zeta_(zeta), kappa_(kappa) {
  id_ = get_next_uuid();
}

void Risk::set_zeta(const float zeta) { this->zeta_ = zeta; }

void Risk::set_kappa(const float kappa) { this->kappa_ = kappa; }

float Risk::get_zeta() const { return zeta_; }

float Risk::get_kappa() const { return kappa_; }

gogo_id_t Risk::get_id() const { return id_; }
} // namespace gogort