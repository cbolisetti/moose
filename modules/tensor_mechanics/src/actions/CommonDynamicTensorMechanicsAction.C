//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "CommonDynamicTensorMechanicsAction.h"
#include "DynamicTensorMechanicsAction.h"
#include "ActionWarehouse.h"

registerMooseAction("TensorMechanicsApp", CommonDynamicTensorMechanicsAction, "meta_action");

defineLegacyParams(CommonDynamicTensorMechanicsAction);

InputParameters
CommonDynamicTensorMechanicsAction::validParams()
{
  InputParameters params = DynamicTensorMechanicsAction::validParams();
  return params;
}

CommonDynamicTensorMechanicsAction::CommonDynamicTensorMechanicsAction(const InputParameters & parameters)
  : Action(parameters)
{
}

void
CommonDynamicTensorMechanicsAction::act()
{
  // check if sub-blocks block are found which will use the common parameters
  auto action = _awh.getActions<DynamicTensorMechanicsAction>();
  if (action.size() == 0)
    mooseWarning("Common parameters are supplied, but not used in ", parameters().blockLocation());
}
