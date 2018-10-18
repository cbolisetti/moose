//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef CENTRALDIFFERENCE_H
#define CENTRALDIFFERENCE_H

#include "ActuallyExplicitEuler.h"
#include "MeshChangedInterface.h"

// Forward declarations
class CentralDifference;

template <>
InputParameters validParams<CentralDifference>();

/**
 * Implements a truly explicit (no nonlinear solve) first-order, forward Euler
 * time integration scheme.
 */
class CentralDifference : public ActuallyExplicitEuler
{
public:
  CentralDifference(const InputParameters & parameters);

  virtual void computeTimeDerivatives() override;

protected:

  /// solution vector for \f$ {du^dotdot}\over{du} \f$
  Real & _du_dotdot_du;
};

#endif // CentralDifference_H
