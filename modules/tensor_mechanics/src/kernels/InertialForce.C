//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "InertialForce.h"
#include "SubProblem.h"
#include "TimeIntegrator.h"

registerMooseObject("TensorMechanicsApp", InertialForce);

template <>
InputParameters
validParams<InertialForce>()
{
  InputParameters params = validParams<TimeKernel>();
  params.addClassDescription("Calculates the residual for the interial force "
                             "($M \\cdot acceleration$) and the contribution of mass"
                             " dependent Rayleigh damping and HHT time "
                             " integration scheme ($\\eta \\cdot M \\cdot"
                             " ((1+\\alpha)velq2-\\alpha \\cdot vel-old) $)");
  params.set<bool>("use_displaced_mesh") = true;
  params.addCoupledVar("velocity", "velocity variable");
  params.addCoupledVar("acceleration", "acceleration variable");
  params.addParam<Real>("beta", "beta parameter for Newmark Time integration");
  params.addParam<Real>("gamma", "gamma parameter for Newmark Time integration");
  params.addParam<MaterialPropertyName>("eta",
                                        0.0,
                                        "Name of material property or a constant real "
                                        "number defining the eta parameter for the "
                                        "Rayleigh damping.");
  params.addParam<Real>("alpha",
                        0,
                        "alpha parameter for mass dependent numerical damping induced "
                        "by HHT time integration scheme");
  params.addParam<MaterialPropertyName>(
      "density", "density", "Name of Material Property that provides the density");
  return params;
}

InertialForce::InertialForce(const InputParameters & parameters)
  : TimeKernel(parameters),
    _var_num(_var.number()),
    _density(getMaterialProperty<Real>("density")),
    _beta(isParamValid("beta") ? getParam<Real>("beta") : 0.1),
    _gamma(isParamValid("gamma") ? getParam<Real>("gamma") : 0.1),
    _eta(getMaterialProperty<Real>("eta")),
    _alpha(getParam<Real>("alpha")),
    _time_integrator(_sys.getTimeIntegrator())
{
  if (isParamValid("beta") && isParamValid("gamma") && isParamValid("velocity") &&
      isParamValid("acceleration"))
  {
    _vel_old = &coupledValueOld("velocity");
    _accel_old = &coupledValueOld("acceleration");
    _u_old = &valueOld();
  }
  else if (!isParamValid("beta") && !isParamValid("gamma") && !isParamValid("velocity") &&
           !isParamValid("acceleration"))
  {
    _u_dot_residual = &(_var.uDotResidual());
    _u_dotdot_residual = &(_var.uDotDotResidual());
    _u_dot_old = &(_var.uDotOld());
    _du_dot_du = &(_var.duDotDu());
    _du_dotdot_du = &(_var.duDotDotDu());
  }
  else
    mooseError("InertialForce: Either all or none of `beta`, `gamma`, `velocity`and `acceleration` "
               "should be provided as input.");

  if (_alpha != 0 && _time_integrator->isExplicit())
    mooseError("InertialForce: HHT time integration parameter can only be used with Newmark-Beta time integrator.");
}

Real
InertialForce::computeQpResidual()
{
  if (_dt == 0)
    return 0;
  else if (isParamValid("beta"))
  {
    Real accel = 1. / _beta *
                 (((_u[_qp] - (*_u_old)[_qp]) / (_dt * _dt)) - (*_vel_old)[_qp] / _dt -
                  (*_accel_old)[_qp] * (0.5 - _beta));
    Real vel = (*_vel_old)[_qp] + (_dt * (1 - _gamma)) * (*_accel_old)[_qp] + _gamma * _dt * accel;
    return _test[_i][_qp] * _density[_qp] *
           (accel + vel * _eta[_qp] * (1 + _alpha) - _alpha * _eta[_qp] * (*_vel_old)[_qp]);
  }

  else if (_time_integrator->isLumped())
    // Lumped mass option
    // Only lumping the matrix here
    // will multiply by corresponding residual multiplier after lumping the matrix
    return _test[_i][_qp] * _density[_qp];
  //
  // else if (_alpha == 0)
  //   // no HHT, implicit or explicit
  //   return _test[_i][_qp] * _density[_qp] * ((*_u_dotdot_residual)[_qp] + (*_u_dot_residual)[_qp] * _eta[_qp]);

  else
  {
    // Consistent mass option
    // Same for explicit, implicit, and implicit with HHT
    return _test[_i][_qp] * _density[_qp] *
           ((*_u_dotdot_residual)[_qp] + (*_u_dot_residual)[_qp] * _eta[_qp] * (1.0 + _alpha) -
            _alpha * _eta[_qp] * (*_u_dot_old)[_qp]);
  }
}

void
InertialForce::computeResidual()
{
  prepareVectorTag(_assembly, _var.number());

  precalculateResidual();
  for (_i = 0; _i < _test.size(); _i++)
    for (_qp = 0; _qp < _qrule->n_points(); _qp++)
      _local_re(_i) += _JxW[_qp] * _coord[_qp] * computeQpResidual();
      // in the above line, computeQpResidual only contains the Qp mass when lumped mass option is used

  // Residual calculation for lumped-mass matrices for explicit integration
  if (_time_integrator->isLumped() && _time_integrator->isExplicit())
  {
    std::vector<Node *> node;
    node.resize(_test.size());
    for (unsigned int i = 0; i < node.size(); ++i)
      node[i] = _current_elem->get_node(i);

    // Fetch the solution for the nodes in the at time t
    NonlinearSystemBase & nonlinear_sys = _fe_problem.getNonlinearSystemBase();
    const NumericVector<Number> & u_dotdot_residual = _time_integrator->computeUDotDotResidual();
    const NumericVector<Number> & u_dot_residual = _time_integrator->computeUDotResidual();
    Real u_dot_residual_node, u_dotdot_residual_node;
    for (unsigned int j = 0; j < node.size(); j++)
    {
      u_dot_residual_node = u_dot_residual(node[j]->dof_number(nonlinear_sys.number(), _var_num, 0));
      u_dotdot_residual_node = u_dotdot_residual(node[j]->dof_number(nonlinear_sys.number(), _var_num, 0));
      _local_re(j) *= u_dotdot_residual_node + _eta[_qp] * u_dot_residual_node; // Calculating the residuals at each node
    }
  }

  accumulateTaggedLocalResidual();

  if (_has_save_in)
  {
    Threads::spin_mutex::scoped_lock lock(Threads::spin_mtx);
    for (const auto & var : _save_in)
      var->sys().solution().add_vector(_local_re, var->dofIndices());
  }
}

Real
InertialForce::computeQpJacobian()
{
  if (_dt == 0)
    return 0;
  else if (isParamValid("beta"))
    return _test[_i][_qp] * _density[_qp] / (_beta * _dt * _dt) * _phi[_j][_qp] +
           _eta[_qp] * (1 + _alpha) * _test[_i][_qp] * _density[_qp] * _gamma / _beta / _dt *
               _phi[_j][_qp];
  else
    {
      return _test[_i][_qp] * _density[_qp] * (*_du_dotdot_du)[_qp] * _phi[_j][_qp] +
           _eta[_qp] * (1 + _alpha) * _test[_i][_qp] * _density[_qp] * (*_du_dot_du)[_qp] *
               _phi[_j][_qp];
    }
}
