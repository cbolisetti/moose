/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef NodalLumpedMassKernel_H
#define NodalLumpedMassKernel_H

#include "Kernel.h"
#include "Material.h"
#include "RankTwoTensor.h"

// Forward Declarations
class NodalLumpedMassKernel;

template <>
InputParameters validParams<NodalLumpedMassKernel>();

class NodalLumpedMassKernel : public Kernel
{
public:
  NodalLumpedMassKernel(const InputParameters & parameters);

  virtual void computeResidual() override;

  virtual void computeJacobian() override;

protected:
  virtual Real computeQpResidual() override { return 0.0; };

private:
  const Real _mass;

  /// Map between boundary nodes and nodal mass
  std::map<dof_id_type, Real> _node_id_to_mass;

};

#endif // NodalLumpedMassKernel_H
