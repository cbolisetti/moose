//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef INERTIALFORCE_H
#define INERTIALFORCE_H

#include "TimeKernel.h"
#include "Material.h"

// Forward Declarations
class InertialForce;
class TimeIntegrator;

template <>
InputParameters validParams<InertialForce>();

class InertialForce : public TimeKernel
{
public:
  InertialForce(const InputParameters & parameters);

  virtual void computeResidual() override;

protected:
  virtual Real computeQpResidual() override;

  virtual Real computeQpJacobian() override;

private:
  unsigned int _var_num;
  const MaterialProperty<Real> & _density;
  const VariableValue * _u_old;
  const VariableValue * _vel_old;
  const VariableValue * _accel_old;
  const Real _beta;
  const Real _gamma;
  const MaterialProperty<Real> & _eta;
  const Real _alpha;

  // Velocity and acceleration calculated by time integrator
  const VariableValue * _u_dot_residual;
  const VariableValue * _u_dotdot_residual;
  const VariableValue * _u_dot_old;
  const VariableValue * _du_dot_du;
  const VariableValue * _du_dotdot_du;

  /// A pointer to TimeIntegrator.
  TimeIntegrator * _time_integrator;
};

#endif // INERTIALFORCE_H
