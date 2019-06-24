/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "NodalLumpedMassKernel.h"
#include "SubProblem.h"
#include "libmesh/utility.h"
#include "MooseVariable.h"
#include "Assembly.h"
#include "NonlinearSystem.h"
#include "AuxiliarySystem.h"
#include "MooseMesh.h"
#include "DelimitedFileReader.h"

registerMooseObject("TensorMechanicsApp", NodalLumpedMassKernel);

template <>
InputParameters
validParams<NodalLumpedMassKernel>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Calculates the residual for the interial force/moment and the "
                             "contribution of mass dependent Rayleigh damping and HHT time "
                             "integration scheme.");

  params.addParam<Real>("mass", "Mass associated with the node");
  params.addParam<FileName>("nodal_mass_file",
                            "The file containing the nodal positions and the corresponding nodal masses.");
  return params;
}

NodalLumpedMassKernel::NodalLumpedMassKernel(const InputParameters & parameters)
  : Kernel(parameters),
   _mass(isParamValid("mass") ? getParam<Real>("mass") : 0.0)
{
  if (!isParamValid("nodal_mass_file") && !isParamValid("mass"))
    mooseError(
        "NodalMassMatrix: Please provide either mass or nodal_mass_file as input.");
  else if (isParamValid("nodal_mass_file") && isParamValid("mass"))
    mooseError("NodalMassMatrix: Please provide either mass or nodal_mass_file as input, "
               "not both.");

  if (isParamValid("nodal_mass_file"))
  {
    MooseUtils::DelimitedFileReader nodal_mass_file(getParam<FileName>("nodal_mass_file"));
    nodal_mass_file.setHeaderFlag(MooseUtils::DelimitedFileReader::HeaderFlag::OFF);
    nodal_mass_file.read();
    std::vector<std::vector<Real>> data = nodal_mass_file.getData();
    if (data.size() != 4)
      mooseError("NodalMassMatrix: The number of columns in ",
                 getParam<FileName>("nodal_mass_file"),
                 " should be 4.");

    unsigned int node_found = 0;
    ConstNodeRange * local_node_set = _mesh.getLocalNodeRange();
    for (ConstNodeRange::const_iterator it = local_node_set->begin(); it != local_node_set->end(); ++it)
    {
      const Node * local_node = *it;
      _node_id_to_mass[local_node->id()] = 0.0;

      for (unsigned int i = 0; i < data[0].size(); ++i)
      {
        if (MooseUtils::absoluteFuzzyEqual(data[0][i], (*local_node)(0), 1e-6) &&
           MooseUtils::absoluteFuzzyEqual(data[1][i], (*local_node)(1), 1e-6) &&
           MooseUtils::absoluteFuzzyEqual(data[2][i], (*local_node)(2), 1e-6))
        {
          _node_id_to_mass[local_node->id()] = data[3][i];
          node_found += 1;
          break;
        }
      }
    }

    if (node_found != data[0].size())
      mooseError("NodalLumpedMassMatrix: Out of ",
                 data[0].size(),
                 " nodal positions in ",
                 getParam<FileName>("nodal_mass_file"),
                 " only ",
                 node_found,
                 " nodes were found in the block.");
  }
}

void
NodalLumpedMassKernel::computeResidual()
{
  prepareVectorTag(_assembly, _var.number());
  precalculateResidual();

  mooseAssert(re.size() == 2, "Beam element only has two nodes.");

  // dummy values for residual. Residual not required for eigenvalue calculation
  _local_re(0) = 10.0;
  _local_re(1) = -10.0;

  accumulateTaggedLocalResidual();

  if (_has_save_in)
  {
    Threads::spin_mutex::scoped_lock lock(Threads::spin_mtx);
    for (const auto & var : _save_in)
      var->sys().solution().add_vector(_local_re, var->dofIndices());
  }
}

void
NodalLumpedMassKernel::computeJacobian()
{
  prepareMatrixTag(_assembly, _var.number(), _var.number());

  _local_ke.zero();

  std::vector<Real> mass(_test.size());
  if (isParamValid("mass"))
  {
    for (unsigned int i = 0; i < _test.size(); ++i)
      mass[i] = _mass;
  }
  else
  {
    for (unsigned int i = 0; i < _test.size(); ++i)
    {
      const Node * node = _current_elem->node_ptr(i);
      if (_node_id_to_mass.find(node->id()) != _node_id_to_mass.end())
        mass[i] = _node_id_to_mass[node->id()];
      else
        mooseError("NodalLumpedMassKernel: Unable to find current node in the node_id_to_mass map.");
    }
  }

  for (unsigned int i = 0; i < _test.size(); ++i)
    _local_ke(i, i) = mass[i];

  accumulateTaggedLocalMatrix();

  if (_has_diag_save_in)
  {
    unsigned int rows = _local_ke.m();
    DenseVector<Number> diag(rows);
    for (unsigned int i = 0; i < rows; ++i)
      diag(i) = _local_ke(i, i);

    Threads::spin_mutex::scoped_lock lock(Threads::spin_mtx);
    for (const auto & var : _diag_save_in)
      var->sys().solution().add_vector(diag, var->dofIndices());
  }
}
