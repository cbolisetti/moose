//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// MOOSE includes
#include "CentralDifference.h"
#include "NonlinearSystem.h"
#include "FEProblem.h"

registerMooseObject("MooseApp", CentralDifference);

template <>
InputParameters
validParams<CentralDifference>()
{
  InputParameters params = validParams<ActuallyExplicitEuler>();

  params.addClassDescription(
      "Implementation of Explicit/Forward Euler without invoking any of the nonlinear solver");

  return params;
}

CentralDifference::CentralDifference(const InputParameters & parameters)
  : ActuallyExplicitEuler(parameters),
    _du_dotdot_du(_sys.duDotDotDu())
{
  _fe_problem.setUDotOldRequested(true);
  _fe_problem.setUDotDotRequested(true);
  _fe_problem.setUDotDotOldRequested(true);
  _fe_problem.setSolutionState(4);
}

void
CentralDifference::computeTimeDerivatives()
{
  if (!_sys.solutionUDot())
    mooseError("CentralDifference: Time derivative of solution (`u_dot`) is not stored. Please "
               "set uDotRequested() to true in FEProblemBase before requesting `u_dot`.");

  if (!_sys.solutionUDotDot())
    mooseError("CentralDifference: Time derivative of solution (`u_dotdot`) is not stored. Please "
               "set uDotDotRequested() to true in FEProblemBase before requesting `u_dot`.");
  // computing second derivative
  // using the Central Difference method
  // u_dotdot_old = (first_term - second_term + third_term) / dt / dt
  //       first_term = u
  //      second_term = 2 * u_old
  //       third_term = u_older
  NumericVector<Number> & u_dotdot = *_sys.solutionUDotDot();
  u_dotdot = *_sys.solutionState(1); // solutionState(1) and solutionState(0) are equal at this point
  u_dotdot -= *_sys.solutionState(2);
  u_dotdot -= *_sys.solutionState(2);
  u_dotdot += *_sys.solutionState(3);
  u_dotdot *= 1.0 / (_dt * _dt);

  // computing first derivative
  // using the Central Difference method
  // u_dot_old = (first_term - second_term) / 2 / dt
  //       first_term = u
  //      second_term = u_older
  NumericVector<Number> & u_dot = *_sys.solutionUDot();
  u_dot = *_sys.solutionState(1); // old solution, which is the same as the current solution at this point
  // std::cout << "u\t" << _u_dot(0) << "\n";
  u_dot -= *_sys.solutionState(3); // 'older than older' solution
  u_dot *= 1.0 / (2.0 * _dt);
  // std::cout << "u_dot\t" << _u_dot(0) << "\n";

  // make sure _u_dotdot and _u_dot are in good state
  u_dotdot.close();
  u_dot.close();

  // used for Jacobian calculations
  _du_dot_du = 1.0 / (2 * _dt);
  _du_dotdot_du = 1.0 / (_dt * _dt);
}
