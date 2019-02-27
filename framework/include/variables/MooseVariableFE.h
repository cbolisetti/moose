//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "metaphysicl/numberarray.h"
#include "metaphysicl/dualnumber.h"

#include "MooseTypes.h"
#include "MooseVariableFEBase.h"
#include "SubProblem.h"
#include "SystemBase.h"
#include "MooseMesh.h"
#include "MooseVariableData.h"

#include "libmesh/numeric_vector.h"
#include "libmesh/dof_map.h"
#include "libmesh/elem.h"
#include "libmesh/quadrature.h"
#include "libmesh/dense_vector.h"
#include "libmesh/dense_vector.h"

class TimeIntegrator;

/**
 * Class for stuff related to variables
 *
 * Each variable can compute nodal or elemental (at QPs) values.
 */
template <typename OutputType>
class MooseVariableFE : public MooseVariableFEBase
{
public:
  typedef OutputType OutputShape;
  typedef OutputType OutputValue;
  typedef typename TensorTools::IncrementRank<OutputShape>::type OutputGradient;
  typedef typename TensorTools::IncrementRank<OutputGradient>::type OutputSecond;
  typedef typename TensorTools::DecrementRank<OutputShape>::type OutputDivergence;

  typedef MooseArray<OutputShape> FieldVariableValue;
  typedef MooseArray<OutputGradient> FieldVariableGradient;
  typedef MooseArray<OutputSecond> FieldVariableSecond;
  typedef MooseArray<OutputShape> FieldVariableCurl;
  typedef MooseArray<OutputDivergence> FieldVariableDivergence;

  typedef MooseArray<std::vector<OutputShape>> FieldVariablePhiValue;
  typedef MooseArray<std::vector<OutputGradient>> FieldVariablePhiGradient;
  typedef MooseArray<std::vector<OutputSecond>> FieldVariablePhiSecond;
  typedef MooseArray<std::vector<OutputShape>> FieldVariablePhiCurl;
  typedef MooseArray<std::vector<OutputDivergence>> FieldVariablePhiDivergence;

  typedef MooseArray<std::vector<OutputShape>> FieldVariableTestValue;
  typedef MooseArray<std::vector<OutputGradient>> FieldVariableTestGradient;
  typedef MooseArray<std::vector<OutputSecond>> FieldVariableTestSecond;
  typedef MooseArray<std::vector<OutputShape>> FieldVariableTestCurl;
  typedef MooseArray<std::vector<OutputDivergence>> FieldVariableTestDivergence;

  MooseVariableFE(unsigned int var_num,
                  const FEType & fe_type,
                  SystemBase & sys,
                  const Assembly & assembly,
                  Moose::VarKindType var_kind,
                  THREAD_ID tid);

  void clearDofIndices() override;

  void prepare() override;
  void prepareNeighbor() override;
  void prepareLowerD() override;

  void prepareAux() override;

  void reinitNode() override;
  void reinitAux() override;
  void reinitAuxNeighbor() override;

  void reinitNodes(const std::vector<dof_id_type> & nodes) override;
  void reinitNodesNeighbor(const std::vector<dof_id_type> & nodes) override;

  /**
   * Whether or not this variable is actually using the shape function value.
   *
   * Currently hardcoded to true because we always compute the value.
   */
  bool usesPhi() const { return true; }
  /**
   * Whether or not this variable is actually using the shape function gradient.
   *
   * Currently hardcoded to true because we always compute the value.
   */
  bool usesGradPhi() const { return true; }
  /**
   * Whether or not this variable is actually using the shape function value.
   *
   * Currently hardcoded to true because we always compute the value.
   */
  bool usesPhiNeighbor() const { return true; }
  /**
   * Whether or not this variable is actually using the shape function gradient.
   *
   * Currently hardcoded to true because we always compute the value.
   */
  bool usesGradPhiNeighbor() const { return true; }
  /**
   * Whether or not this variable is computing any second derivatives.
   */
  bool usesSecondPhi() const;

  /**
   * Whether or not this variable is actually using the shape function second derivative on a
   * neighbor.
   */
  bool usesSecondPhiNeighbor() const;

  /**
   * Whether or not this variable is computing any second derivatives.
   */
  bool computingSecond() const { return usesSecondPhi(); }

  /**
   * Whether or not this variable is computing the curl
   */
  bool computingCurl() const;

  const std::set<SubdomainID> & activeSubdomains() const override;
  bool activeOnSubdomain(SubdomainID subdomain) const override;

  bool isNodal() const override { return _element_data->isNodal(); }
  bool isVector() const override;
  const Node * const & node() const { return _element_data->node(); }
  const dof_id_type & nodalDofIndex() const override { return _element_data->nodalDofIndex(); }
  bool isNodalDefined() const;

  const Node * const & nodeNeighbor() const { return _neighbor_data->node(); }
  const dof_id_type & nodalDofIndexNeighbor() const override
  {
    return _neighbor_data->nodalDofIndex();
  }
  bool isNodalNeighborDefined() const;

  const Elem * const & currentElem() const override { return _element_data->currentElem(); }

  /**
   * Current side this variable is being evaluated on
   */
  const unsigned int & currentSide() const { return _element_data->currentSide(); }

  /**
   * Current neighboring element
   */
  const Elem * const & neighbor() const { return _neighbor_data->currentElem(); }

  virtual void prepareIC() override;

  const FieldVariablePhiValue & phi() const { return _element_data->phi(); }
  const FieldVariablePhiGradient & gradPhi() const { return _element_data->gradPhi(); }
  const FieldVariablePhiSecond & secondPhi() const;
  const FieldVariablePhiCurl & curlPhi() const;

  const FieldVariablePhiValue & phiFace() const { return _element_data->phiFace(); }
  const FieldVariablePhiGradient & gradPhiFace() const { return _element_data->gradPhiFace(); }
  const FieldVariablePhiSecond & secondPhiFace() const;
  const FieldVariablePhiCurl & curlPhiFace() const;

  const FieldVariablePhiValue & phiNeighbor() const { return _neighbor_data->phi(); }
  const FieldVariablePhiGradient & gradPhiNeighbor() const { return _neighbor_data->gradPhi(); }
  const FieldVariablePhiSecond & secondPhiNeighbor() const;
  const FieldVariablePhiCurl & curlPhiNeighbor() const;

  const FieldVariablePhiValue & phiFaceNeighbor() const { return _neighbor_data->phiFace(); }
  const FieldVariablePhiGradient & gradPhiFaceNeighbor() const
  {
    return _neighbor_data->gradPhiFace();
  }
  const FieldVariablePhiSecond & secondPhiFaceNeighbor() const;
  const FieldVariablePhiCurl & curlPhiFaceNeighbor() const;

  const FieldVariablePhiValue & phiLower() const { return _lower_data->phi(); }
  const FieldVariablePhiGradient & gradPhiLower() const { return _lower_data->gradPhi(); }

  template <ComputeStage compute_stage>
  const typename VariableTestGradientType<OutputType, compute_stage>::type & adGradPhi()
  {
    return _element_data->template adGradPhi<compute_stage>();
  }

  template <ComputeStage compute_stage>
  const typename VariableTestGradientType<OutputType, compute_stage>::type & adGradPhiFace()
  {
    return _element_data->template adGradPhiFace<compute_stage>();
  }

  // damping
  const FieldVariableValue & increment() const { return _element_data->increment(); }

  const FieldVariableValue & vectorTagValue(TagID tag)
  {
    return _element_data->vectorTagValue(tag);
  }
  const FieldVariableValue & matrixTagValue(TagID tag)
  {
    return _element_data->matrixTagValue(tag);
  }

  /// element solutions
  const FieldVariableValue & sln() const { return _element_data->sln(Moose::Current); }
  const FieldVariableValue & slnOld() const { return _element_data->sln(Moose::Old); }
  const FieldVariableValue & slnOlder() const { return _element_data->sln(Moose::Older); }
  const FieldVariableValue & slnPreviousNL() const { return _element_data->sln(Moose::PreviousNL); }

  /// element gradients
  const FieldVariableGradient & gradSln() const { return _element_data->gradSln(Moose::Current); }
  const FieldVariableGradient & gradSlnOld() const { return _element_data->gradSln(Moose::Old); }
  const FieldVariableGradient & gradSlnOlder() const
  {
    return _element_data->gradSln(Moose::Older);
  }
  const FieldVariableGradient & gradSlnPreviousNL() const
  {
    return _element_data->gradSln(Moose::PreviousNL);
  }

  /// element gradient dots
  const FieldVariableGradient & gradSlnDot() const { return _element_data->gradSlnDot(); }
  const FieldVariableGradient & gradSlnDotDot() const { return _element_data->gradSlnDotDot(); }

  /// element seconds
  const FieldVariableSecond & secondSln() const { return _element_data->secondSln(Moose::Current); }
  const FieldVariableSecond & secondSlnOld() const { return _element_data->secondSln(Moose::Old); }
  const FieldVariableSecond & secondSlnOlder() const
  {
    return _element_data->secondSln(Moose::Older);
  }
  const FieldVariableSecond & secondSlnPreviousNL() const
  {
    return _element_data->secondSln(Moose::PreviousNL);
  }

  /// element curls
  const FieldVariableCurl & curlSln() const { return _element_data->curlSln(Moose::Current); }
  const FieldVariableCurl & curlSlnOld() const { return _element_data->curlSln(Moose::Old); }
  const FieldVariableCurl & curlSlnOlder() const { return _element_data->curlSln(Moose::Older); }

  /// AD
  template <ComputeStage compute_stage>
  const typename VariableValueType<OutputType, compute_stage>::type & adSln() const
  {
    return _element_data->template adSln<compute_stage>();
  }
  template <ComputeStage compute_stage>
  const typename VariableGradientType<OutputType, compute_stage>::type & adGradSln() const
  {
    return _element_data->template adGradSln<compute_stage>();
  }
  template <ComputeStage compute_stage>
  const typename VariableSecondType<OutputType, compute_stage>::type & adSecondSln() const
  {
    return _element_data->template adSecondSln<compute_stage>();
  }
  template <ComputeStage compute_stage>
  const typename VariableValueType<OutputType, compute_stage>::type & adUDot() const
  {
    return _element_data->template adUDot<compute_stage>();
  }

  /// neighbor AD
  template <ComputeStage compute_stage>
  const typename VariableValueType<OutputType, compute_stage>::type & adSlnNeighbor() const
  {
    return _neighbor_data->template adSln<compute_stage>();
  }
  template <ComputeStage compute_stage>
  const typename VariableGradientType<OutputType, compute_stage>::type & adGradSlnNeighbor() const
  {
    return _neighbor_data->template adGradSln<compute_stage>();
  }
  template <ComputeStage compute_stage>
  const typename VariableSecondType<OutputType, compute_stage>::type & adSecondSlnNeighbor() const
  {
    return _neighbor_data->template adSecondSln<compute_stage>();
  }
  template <ComputeStage compute_stage>
  const typename VariableValueType<OutputType, compute_stage>::type & adUDotNeighbor() const
  {
    return _neighbor_data->template adUDot<compute_stage>();
  }

  /// element dots
  const FieldVariableValue & uDot() const { return _element_data->uDot(); }
  const FieldVariableValue & uDotDot() const { return _element_data->uDotDot(); }
  const FieldVariableValue & uDotOld() const { return _element_data->uDotOld(); }
  const FieldVariableValue & uDotDotOld() const { return _element_data->uDotDotOld(); }
  const VariableValue & duDotDu() const { return _element_data->duDotDu(); }
  const VariableValue & duDotDotDu() const { return _element_data->duDotDotDu(); }

  /// neighbor solutions
  const FieldVariableValue & slnNeighbor() const { return _neighbor_data->sln(Moose::Current); }
  const FieldVariableValue & slnOldNeighbor() const { return _neighbor_data->sln(Moose::Old); }
  const FieldVariableValue & slnOlderNeighbor() const { return _neighbor_data->sln(Moose::Older); }
  const FieldVariableValue & slnPreviousNLNeighbor() const
  {
    return _neighbor_data->sln(Moose::PreviousNL);
  }
 
  const FieldVariableValue & uDotResidual()
  {
    if (_sys.solutionUDot())
    {
      _need_u_dot_residual = true;
      return _u_dot_residual;
    }
    else
      mooseError("MooseVariableFE: Time derivative of solution (`u_dot`) is not stored. Please set "
                 "uDotRequested() to true in FEProblemBase before requesting `u_dot`.");
  }


  const FieldVariableValue & uDotDotResidual()
  {
    if (_sys.solutionUDotDot())
    {
      _need_u_dotdot_residual = true;
      return _u_dotdot_residual;
    }
    else
      mooseError("MooseVariableFE: Second time derivative of solution (`u_dotdot`) is not stored. "
                 "Please set uDotDotRequested() to true in FEProblemBase before requesting "
                 "`u_dotdot`.");
  }

  /// neighbor solution gradients
  const FieldVariableGradient & gradSlnNeighbor() const
  {
    return _neighbor_data->gradSln(Moose::Current);
  }
  const FieldVariableGradient & gradSlnOldNeighbor() const
  {
    return _neighbor_data->gradSln(Moose::Old);
  }
  const FieldVariableGradient & gradSlnOlderNeighbor() const
  {
    return _neighbor_data->gradSln(Moose::Older);
  }
  const FieldVariableGradient & gradSlnPreviousNLNeighbor() const
  {
    return _neighbor_data->gradSln(Moose::PreviousNL);
  }

  /// neighbor grad dots
  const FieldVariableGradient & gradSlnNeighborDot() const { return _neighbor_data->gradSlnDot(); }
  const FieldVariableGradient & gradSlnNeighborDotDot() const
  {
    return _neighbor_data->gradSlnDotDot();
  }

  /// neighbor solution seconds
  const FieldVariableSecond & secondSlnNeighbor() const
  {
    return _neighbor_data->secondSln(Moose::Current);
  }
  const FieldVariableSecond & secondSlnOldNeighbor() const
  {
    return _neighbor_data->secondSln(Moose::Old);
  }
  const FieldVariableSecond & secondSlnOlderNeighbor() const
  {
    return _neighbor_data->secondSln(Moose::Older);
  }
  const FieldVariableSecond & secondSlnPreviousNLNeighbor() const
  {
    return _neighbor_data->secondSln(Moose::PreviousNL);
  }

  /// neighbor solution curls
  const FieldVariableCurl & curlSlnNeighbor() const
  {
    return _neighbor_data->curlSln(Moose::Current);
  }
  
  const FieldVariableValue & uDotNeighborResidual()
  {
    if (_sys.solutionUDot())
    {
      _need_u_dot_neighbor_residual = true;
      return _u_dot_neighbor_residual;
    }
    else
      mooseError("MooseVariableFE: Time derivative of solution (`u_dot`) is not stored. Please set "
                 "uDotRequested() to true in FEProblemBase before requesting `u_dot`.");
  }

  const FieldVariableCurl & curlSlnOldNeighbor() const
  {
    return _neighbor_data->curlSln(Moose::Old);
  }

  const FieldVariableValue & uDotDotNeighborResidual()
  {
    if (_sys.solutionUDotDot())
    {
      _need_u_dotdot_neighbor_residual = true;
      return _u_dotdot_neighbor_residual;
    }
    else
      mooseError("MooseVariableFE: Second time derivative of solution (`u_dotdot`) is not stored. "
                 "Please set uDotDotRequested() to true in FEProblemBase before requesting "
                 "`u_dotdot`");
  }

  const FieldVariableCurl & curlSlnOlderNeighbor() const
  {
    return _neighbor_data->curlSln(Moose::Older);
  }

  /// neighbor dots
  const FieldVariableValue & uDotNeighbor() const { return _neighbor_data->uDot(); }
  const FieldVariableValue & uDotDotNeighbor() const { return _neighbor_data->uDotDot(); }
  const FieldVariableValue & uDotOldNeighbor() const { return _neighbor_data->uDotOld(); }
  const FieldVariableValue & uDotDotOldNeighbor() const { return _neighbor_data->uDotDotOld(); }
  const VariableValue & duDotDuNeighbor() const { return _neighbor_data->duDotDu(); }
  const VariableValue & duDotDotDuNeighbor() const { return _neighbor_data->duDotDotDu(); }

  /// lower-d element solution
  template <ComputeStage compute_stage>
  const typename VariableValueType<OutputType, compute_stage>::type & adSlnLower() const
  {
    return _lower_data->template adSln<compute_stage>();
  }
  const FieldVariableValue & slnLower() const { return _lower_data->sln(Moose::Current); }

  /// Actually compute variable values
  virtual void computeElemValues() override;
  virtual void computeElemValuesFace() override;
  virtual void computeNeighborValuesFace() override;
  virtual void computeNeighborValues() override;
  virtual void computeLowerDValues() override;

  void setNodalValue(OutputType value, unsigned int idx = 0);
  void setDofValues(const DenseVector<Number> & value) override;
  Number getNodalValue(const Node & node) override;
  Number getNodalValueOld(const Node & node) override;
  Number getNodalValueOlder(const Node & node) override;
  Number getElementalValue(const Elem * elem, unsigned int idx = 0) const override;
  Number getElementalValueOld(const Elem * elem, unsigned int idx = 0) const override;
  Number getElementalValueOlder(const Elem * elem, unsigned int idx = 0) const override;

  void getDofIndices(const Elem * elem, std::vector<dof_id_type> & dof_indices) const override;
  const std::vector<dof_id_type> & dofIndices() const final { return _element_data->dofIndices(); }
  unsigned int numberOfDofs() const final { return _element_data->numberOfDofs(); }
  const std::vector<dof_id_type> & dofIndicesNeighbor() const final
  {
    return _neighbor_data->dofIndices();
  }
  const std::vector<dof_id_type> & dofIndicesLower() const final
  {
    return _lower_data->dofIndices();
  }

  unsigned int numberOfDofsNeighbor() override { return _neighbor_data->dofIndices().size(); }

  void insert(NumericVector<Number> & residual) override;
  void add(NumericVector<Number> & residual) override;

  const MooseArray<Number> & dofValue() override;
  const MooseArray<Number> & dofValues() override;
  const MooseArray<Number> & dofValuesOld() override;
  const MooseArray<Number> & dofValuesOlder() override;
  const MooseArray<Number> & dofValuesPreviousNL() override;
  const MooseArray<Number> & dofValuesNeighbor() override;
  const MooseArray<Number> & dofValuesOldNeighbor() override;
  const MooseArray<Number> & dofValuesOlderNeighbor() override;
  const MooseArray<Number> & dofValuesPreviousNLNeighbor() override;
  const MooseArray<Number> & dofValuesDot() override;
  const MooseArray<Number> & dofValuesDotResidual() override;
  const MooseArray<Number> & dofValuesDotNeighbor() override;
  const MooseArray<Number> & dofValuesDotNeighborResidual() override;
  const MooseArray<Number> & dofValuesDotOld() override;
  const MooseArray<Number> & dofValuesDotOldNeighbor() override;
  const MooseArray<Number> & dofValuesDotDot() override;
  const MooseArray<Number> & dofValuesDotDotResidual() override;
  const MooseArray<Number> & dofValuesDotDotNeighbor() override;
  const MooseArray<Number> & dofValuesDotDotNeighborResidual() override;
  const MooseArray<Number> & dofValuesDotDotOld() override;
  const MooseArray<Number> & dofValuesDotDotOldNeighbor() override;
  const MooseArray<Number> & dofValuesDuDotDu() override;
  const MooseArray<Number> & dofValuesDuDotDuNeighbor() override;
  const MooseArray<Number> & dofValuesDuDotDotDu() override;
  const MooseArray<Number> & dofValuesDuDotDotDuNeighbor() override;

  /**
   * Return the AD dof values
   */
  template <ComputeStage compute_stage>
  const MooseArray<typename Moose::RealType<compute_stage>::type> & adDofValues();

  /**
   * Compute and store incremental change in solution at QPs based on increment_vec
   */
  void computeIncrementAtQps(const NumericVector<Number> & increment_vec);

  /**
   * Compute and store incremental change at the current node based on increment_vec
   */
  void computeIncrementAtNode(const NumericVector<Number> & increment_vec);

  /**
   * Compute the variable value at a point on an element
   * @param elem The element we are computing on
   * @param phi Evaluated shape functions at a point
   * @return The variable value
   */
  OutputType getValue(const Elem * elem, const std::vector<std::vector<OutputType>> & phi) const;

  /**
   * Compute the variable gradient value at a point on an element
   * @param elem The element we are computing on
   * @param phi Evaluated shape functions at a point
   * @return The variable gradient value
   */
  typename OutputTools<OutputType>::OutputGradient
  getGradient(const Elem * elem,
              const std::vector<std::vector<typename OutputTools<OutputType>::OutputGradient>> &
                  grad_phi) const;

  /**
   * Return phi size
   */
  virtual size_t phiSize() const final { return _element_data->phiSize(); }
  /**
   * Return phiFace size
   */
  virtual size_t phiFaceSize() const final { return _element_data->phiFaceSize(); }
  /**
   * Return phiNeighbor size
   */
  virtual size_t phiNeighborSize() const final { return _neighbor_data->phiSize(); }
  /**
   * Return phiFaceNeighbor size
   */
  virtual size_t phiFaceNeighborSize() const final { return _neighbor_data->phiFaceSize(); }

  size_t phiLowerSize() const final { return _lower_data->phiSize(); }

  /**
   * Methods for retrieving values of variables at the nodes
   */
  const OutputType & nodalValue();
  const OutputType & nodalValueOld();
  const OutputType & nodalValueOlder();
  const OutputType & nodalValuePreviousNL();
  const OutputType & nodalValueDot();
  const OutputType & nodalValueDotResidual();
  const OutputType & nodalValueDotDot();
  const OutputType & nodalValueDotDotResidual();
  const OutputType & nodalValueDotOld();
  const OutputType & nodalValueDotDotOld();
  const OutputType & nodalValueDuDotDu();
  const OutputType & nodalValueDuDotDotDu();
  const OutputType & nodalValueNeighbor();
  const OutputType & nodalValueOldNeighbor();
  const OutputType & nodalValueOlderNeighbor();
  const OutputType & nodalValuePreviousNLNeighbor();
  const OutputType & nodalValueDotNeighbor();
  const OutputType & nodalValueDotNeighborResidual();
  const OutputType & nodalValueDotDotNeighbor();
  const OutputType & nodalValueDotDotNeighborResidual();
  const OutputType & nodalValueDotOldNeighbor();
  const OutputType & nodalValueDotDotOldNeighbor();
  const OutputType & nodalValueDuDotDuNeighbor();
  const OutputType & nodalValueDuDotDotDuNeighbor();

  /**
   * Methods for retrieving values of variables at the nodes in a MooseArray for AuxKernelBase
   */
  const MooseArray<OutputType> & nodalValueArray()
  {
    return _element_data->nodalValueArray(Moose::Current);
  }
  const MooseArray<OutputType> & nodalValueOldArray()
  {
    return _element_data->nodalValueArray(Moose::Old);
  }
  const MooseArray<OutputType> & nodalValueOlderArray()
  {
    return _element_data->nodalValueArray(Moose::Older);
  }

  const MooseArray<Real> & nodalVectorTagValue(TagID tag);
  const MooseArray<Real> & nodalMatrixTagValue(TagID tag);

  template <ComputeStage compute_stage>
  const typename Moose::ValueType<OutputType, compute_stage>::type & adNodalValue();

  virtual void computeNodalValues() override;
  virtual void computeNodalNeighborValues() override;

<<<<<<< HEAD
protected:
  const Assembly & _assembly;
=======
  void computeAD(const unsigned int & num_dofs,
                 const unsigned int & nqp,
                 const bool & is_transient,
                 const FieldVariablePhiValue & phi,
                 const FieldVariablePhiGradient & grad_phi,
                 const FieldVariablePhiSecond *& second_phi,
                 const typename VariableTestGradientType<OutputType, JACOBIAN>::type & ad_grad_phi);
  void computeADNeighbor(const unsigned int & num_dofs,
                         const unsigned int & nqp,
                         const bool & is_transient,
                         const FieldVariablePhiValue & phi,
                         const FieldVariablePhiGradient & grad_phi,
                         const FieldVariablePhiSecond *& second_phi);

  /**
   * Helper methods for assigning nodal values from their corresponding solution values (dof values
   * as they're referred to here in this class). These methods are only truly meaningful for nodal
   * basis families
   */
  void assignNodalValue(const Real & value, const unsigned int & component);
  void assignADNodalValue(const DualReal & value, const unsigned int & component);
  void assignNodalValueOld(const Real & value, const unsigned int & component);
  void assignNodalValueOlder(const Real & value, const unsigned int & component);
  void assignNodalValuePreviousNL(const Real & value, const unsigned int & component);
  void assignNodalValueDot(const Real & value, const unsigned int & component);
  void assignNodalValueDotResidual(const Real & value, const unsigned int & component);
  void assignNodalValueDotOld(const Real & value, const unsigned int & component);
  void assignNodalValueDotDot(const Real & value, const unsigned int & component);
  void assignNodalValueDotDotResidual(const Real & value, const unsigned int & component);
  void assignNodalValueDotDotOld(const Real & value, const unsigned int & component);
  void assignNeighborNodalValue(const Real & value, const unsigned int & component);
  void assignNeighborNodalValueOld(const Real & value, const unsigned int & component);
  void assignNeighborNodalValueOlder(const Real & value, const unsigned int & component);
  void assignNeighborNodalValuePreviousNL(const Real & value, const unsigned int & component);

protected:
  /// Whether this variable is on the displaced system
  const bool _displaced;

  /// Our assembly
  Assembly & _assembly;

  /// Quadrature rule for interior
  QBase *& _qrule;
  /// Quadrature rule for the face
  QBase *& _qrule_face;
  /// Quadrature rule for the neighbor
  QBase *& _qrule_neighbor;

  /// current element
  const Elem *& _elem;
  /// the side of the current element (valid when doing face assembly)
  unsigned int & _current_side;

  /// neighboring element
  const Elem *& _neighbor;

  /// DOF indices (neighbor)
  std::vector<dof_id_type> _dof_indices_neighbor;

  bool _need_u_old;
  bool _need_u_older;
  bool _need_u_previous_nl;

  bool _need_u_dot;
  bool _need_u_dot_residual;
  bool _need_u_dotdot;
  bool _need_u_dotdot_residual;
  bool _need_u_dot_old;
  bool _need_u_dotdot_old;
  bool _need_du_dot_du;
  bool _need_du_dotdot_du;

  bool _need_grad_old;
  bool _need_grad_older;
  bool _need_grad_previous_nl;
  bool _need_grad_dot;
  bool _need_grad_dotdot;

  bool _need_second;
  bool _need_second_old;
  bool _need_second_older;
  bool _need_second_previous_nl;

  bool _need_curl;
  bool _need_curl_old;
  bool _need_curl_older;

  bool _need_ad;
  bool _need_ad_u;
  bool _need_ad_grad_u;
  bool _need_ad_second_u;
  bool _need_neighbor_ad;
  bool _need_neighbor_ad_u;
  bool _need_neighbor_ad_grad_u;
  bool _need_neighbor_ad_second_u;

  bool _need_u_old_neighbor;
  bool _need_u_older_neighbor;
  bool _need_u_previous_nl_neighbor;

  bool _need_u_dot_neighbor;
  bool _need_u_dot_neighbor_residual;
  bool _need_u_dotdot_neighbor;
  bool _need_u_dotdot_neighbor_residual;
  bool _need_u_dot_old_neighbor;
  bool _need_u_dotdot_old_neighbor;
  bool _need_du_dot_du_neighbor;
  bool _need_du_dotdot_du_neighbor;

  bool _need_grad_old_neighbor;
  bool _need_grad_older_neighbor;
  bool _need_grad_previous_nl_neighbor;
  bool _need_grad_neighbor_dot;
  bool _need_grad_neighbor_dotdot;

  bool _need_second_neighbor;
  bool _need_second_old_neighbor;
  bool _need_second_older_neighbor;
  bool _need_second_previous_nl_neighbor;

  bool _need_curl_neighbor;
  bool _need_curl_old_neighbor;
  bool _need_curl_older_neighbor;

  bool _need_dof_values;
  bool _need_dof_values_old;
  bool _need_dof_values_older;
  bool _need_dof_values_previous_nl;
  bool _need_dof_values_dot;
  bool _need_dof_values_dot_residual;
  bool _need_dof_values_dotdot;
  bool _need_dof_values_dotdot_residual;
  bool _need_dof_values_dot_old;
  bool _need_dof_values_dotdot_old;
  bool _need_dof_du_dot_du;
  bool _need_dof_du_dotdot_du;
  bool _need_dof_values_neighbor;
  bool _need_dof_values_old_neighbor;
  bool _need_dof_values_older_neighbor;
  bool _need_dof_values_previous_nl_neighbor;
  bool _need_dof_values_dot_neighbor;
  bool _need_dof_values_dot_neighbor_residual;
  bool _need_dof_values_dotdot_neighbor;
  bool _need_dof_values_dotdot_neighbor_residual;
  bool _need_dof_values_dot_old_neighbor;
  bool _need_dof_values_dotdot_old_neighbor;
  bool _need_dof_du_dot_du_neighbor;
  bool _need_dof_du_dotdot_du_neighbor;

  std::vector<bool> _need_vector_tag_dof_u;
  std::vector<bool> _need_matrix_tag_dof_u;

  /// Normals at QPs on faces
  const MooseArray<Point> & _normals;

  /// if variable is nodal
  bool _is_nodal;
  /// If we have dofs
  bool _has_dof_indices;
  /// If the neighor has dofs
  bool _neighbor_has_dof_indices;

  /// If true, the dof values get inserted on calling insert()
  bool _has_dof_values;
  bool _has_dof_values_neighbor;

  const Node *& _node;
  const Node *& _node_neighbor;

  dof_id_type _nodal_dof_index;
  dof_id_type _nodal_dof_index_neighbor;

  // dof solution stuff (which for nodal variables corresponds to values at the nodes)

  MooseArray<Real> _dof_values;
  MooseArray<Real> _dof_values_old;
  MooseArray<Real> _dof_values_older;
  MooseArray<Real> _dof_values_previous_nl;

  // Dof values of tagged vectors
  std::vector<MooseArray<Real>> _vector_tags_dof_u;
  // Dof values of the diagonal of tagged matrices
  std::vector<MooseArray<Real>> _matrix_tags_dof_u;

  /// nodal values of u_dot
  MooseArray<Real> _dof_values_dot;
  /// nodal values of u_dot_residual
  MooseArray<Real> _dof_values_dot_residual;
  /// nodal values of u_dotdot
  MooseArray<Real> _dof_values_dotdot;
  /// nodal values of u_dotdot_residual
  MooseArray<Real> _dof_values_dotdot_residual;
  /// nodal values of u_dot_old
  MooseArray<Real> _dof_values_dot_old;
  /// nodal values of u_dotdot_old
  MooseArray<Real> _dof_values_dotdot_old;
  /// nodal values of derivative of u_dot wrt u
  MooseArray<Real> _dof_du_dot_du;
  /// nodal values of derivative of u_dotdot wrt u
  MooseArray<Real> _dof_du_dotdot_du;

  MooseArray<Real> _dof_values_neighbor;
  MooseArray<Real> _dof_values_old_neighbor;
  MooseArray<Real> _dof_values_older_neighbor;
  MooseArray<Real> _dof_values_previous_nl_neighbor;
  MooseArray<Real> _dof_values_dot_neighbor;
  MooseArray<Real> _dof_values_dot_neighbor_residual;
  MooseArray<Real> _dof_values_dotdot_neighbor;
  MooseArray<Real> _dof_values_dotdot_neighbor_residual;
  MooseArray<Real> _dof_values_dot_old_neighbor;
  MooseArray<Real> _dof_values_dotdot_old_neighbor;
  MooseArray<Real> _dof_du_dot_du_neighbor;
  MooseArray<Real> _dof_du_dotdot_du_neighbor;

  // Shape function values, gradients, second derivatives
  const FieldVariablePhiValue & _phi;
  const FieldVariablePhiGradient & _grad_phi;
  const FieldVariablePhiSecond * _second_phi;
  const FieldVariablePhiCurl * _curl_phi;

  // Values, gradients and second derivatives of shape function on faces
  const FieldVariablePhiValue & _phi_face;
  const FieldVariablePhiGradient & _grad_phi_face;
  const FieldVariablePhiSecond * _second_phi_face;
  const FieldVariablePhiCurl * _curl_phi_face;

  // Values, gradients and second derivatives of shape function
  const FieldVariablePhiValue & _phi_neighbor;
  const FieldVariablePhiGradient & _grad_phi_neighbor;
  const FieldVariablePhiSecond * _second_phi_neighbor;
  const FieldVariablePhiCurl * _curl_phi_neighbor;

  // Values, gradients and second derivatives of shape function on faces
  const FieldVariablePhiValue & _phi_face_neighbor;
  const FieldVariablePhiGradient & _grad_phi_face_neighbor;
  const FieldVariablePhiSecond * _second_phi_face_neighbor;
  const FieldVariablePhiCurl * _curl_phi_face_neighbor;

  const typename VariableTestGradientType<OutputShape, JACOBIAN>::type & _ad_grad_phi;
  const typename VariableTestGradientType<OutputShape, JACOBIAN>::type & _ad_grad_phi_face;

  std::vector<FieldVariableValue> _vector_tag_u;
  std::vector<bool> _need_vector_tag_u;
  std::vector<FieldVariableValue> _matrix_tag_u;
  std::vector<bool> _need_matrix_tag_u;

  FieldVariableValue _u;
  FieldVariableValue _u_old;
  FieldVariableValue _u_older;
  FieldVariableValue _u_previous_nl;
  FieldVariableGradient _grad_u;
  FieldVariableGradient _grad_u_old;
  FieldVariableGradient _grad_u_older;
  FieldVariableGradient _grad_u_previous_nl;
  FieldVariableGradient _grad_u_dot;
  FieldVariableGradient _grad_u_dotdot;
  FieldVariableSecond _second_u;
  FieldVariableSecond _second_u_old;
  FieldVariableSecond _second_u_older;
  FieldVariableSecond _second_u_previous_nl;
  FieldVariableCurl _curl_u;
  FieldVariableCurl _curl_u_old;
  FieldVariableCurl _curl_u_older;

  typename VariableValueType<OutputShape, JACOBIAN>::type _ad_u;
  typename VariableGradientType<OutputShape, JACOBIAN>::type _ad_grad_u;
  typename VariableSecondType<OutputShape, JACOBIAN>::type _ad_second_u;
  MooseArray<DualReal> _ad_dof_values;
  MooseArray<DualReal> _ad_dofs_dot;
  typename VariableValueType<OutputShape, JACOBIAN>::type _ad_u_dot;

  typename VariableValueType<OutputShape, JACOBIAN>::type _neighbor_ad_u;
  typename VariableGradientType<OutputShape, JACOBIAN>::type _neighbor_ad_grad_u;
  typename VariableSecondType<OutputShape, JACOBIAN>::type _neighbor_ad_second_u;
  MooseArray<DualReal> _neighbor_ad_dof_values;
  MooseArray<DualReal> _neighbor_ad_dofs_dot;
  typename VariableValueType<OutputShape, JACOBIAN>::type _neighbor_ad_u_dot;

  FieldVariableValue _u_neighbor;
  FieldVariableValue _u_old_neighbor;
  FieldVariableValue _u_older_neighbor;
  FieldVariableValue _u_previous_nl_neighbor;
  FieldVariableGradient _grad_u_neighbor;
  FieldVariableGradient _grad_u_old_neighbor;
  FieldVariableGradient _grad_u_older_neighbor;
  FieldVariableGradient _grad_u_previous_nl_neighbor;
  FieldVariableGradient _grad_u_neighbor_dot;
  FieldVariableGradient _grad_u_neighbor_dotdot;
  FieldVariableSecond _second_u_neighbor;
  FieldVariableSecond _second_u_old_neighbor;
  FieldVariableSecond _second_u_older_neighbor;
  FieldVariableSecond _second_u_previous_nl_neighbor;
  FieldVariableCurl _curl_u_neighbor;
  FieldVariableCurl _curl_u_old_neighbor;
  FieldVariableCurl _curl_u_older_neighbor;

  // time derivatives

  /// u_dot (time derivative)
  FieldVariableValue _u_dot;
  FieldVariableValue _u_dot_neighbor;
  FieldVariableValue _u_dot_residual;
  FieldVariableValue _u_dot_neighbor_residual;

  /// u_dotdot (second time derivative)
  FieldVariableValue _u_dotdot, _u_dotdot_bak;
  FieldVariableValue _u_dotdot_neighbor, _u_dotdot_bak_neighbor;
  FieldVariableValue _u_dotdot_residual, _u_dotdot_bak_residual;
  FieldVariableValue _u_dotdot_neighbor_residual, _u_dotdot_bak_neighbor_residual;

  /// u_dot_old (time derivative)
  FieldVariableValue _u_dot_old, _u_dot_old_bak;
  FieldVariableValue _u_dot_old_neighbor, _u_dot_old_bak_neighbor;

  /// u_dotdot_old (second time derivative)
  FieldVariableValue _u_dotdot_old, _u_dotdot_old_bak;
  FieldVariableValue _u_dotdot_old_neighbor, _u_dotdot_old_bak_neighbor;

  /// derivative of u_dot wrt u
  VariableValue _du_dot_du;
  VariableValue _du_dot_du_neighbor;

  /// derivative of u_dotdot wrt u
  VariableValue _du_dotdot_du, _du_dotdot_du_bak;
  VariableValue _du_dotdot_du_neighbor, _du_dotdot_du_bak_neighbor;

  /// Continuity type of the variable
  FEContinuity _continuity;

  /// Increment in the variable used in dampers
  FieldVariableValue _increment;

  /// Nodal values
  OutputType _nodal_value;
  OutputType _nodal_value_old;
  OutputType _nodal_value_older;
  OutputType _nodal_value_previous_nl;
  OutputType _neighbor_nodal_value;
  OutputType _neighbor_nodal_value_old;
  OutputType _neighbor_nodal_value_older;
  OutputType _neighbor_nodal_value_previous_nl;

  /// nodal values of u_dot
  OutputType _nodal_value_dot;
  /// nodal values of u_dot_residual
  OutputType _nodal_value_dot_residual;
  /// nodal values of u_dotdot
  OutputType _nodal_value_dotdot;
  /// nodal values of u_dotdot_residual
  OutputType _nodal_value_dotdot_residual;
  /// nodal values of u_dot_old
  OutputType _nodal_value_dot_old;
  /// nodal values of u_dotdot_old
  OutputType _nodal_value_dotdot_old;

  /// AD nodal value
  typename Moose::ValueType<OutputType, JACOBIAN>::type _ad_nodal_value;

  /// A zero AD variable
  const DualReal _ad_zero;

  /// A pointer to TimeIntegrator. nullptr if _sys is not a NonlinearSystemBase
  TimeIntegrator * _time_integrator;

  friend class NodeFaceConstraint;
  friend class NodeElemConstraint;
  friend class ValueThresholdMarker;
  friend class ValueRangeMarker;
};

template <>
template <>
const VariableValue & MooseVariableFE<Real>::adSln<RESIDUAL>();

template <>
template <>
const VariableGradient & MooseVariableFE<Real>::adGradSln<RESIDUAL>();

template <>
template <>
const VariableSecond & MooseVariableFE<Real>::adSecondSln<RESIDUAL>();

template <>
template <>
const VariableValue & MooseVariableFE<Real>::adUDot<RESIDUAL>();

template <>
template <>
const VariableValue & MooseVariableFE<Real>::adSlnNeighbor<RESIDUAL>();

template <>
template <>
const VariableGradient & MooseVariableFE<Real>::adGradSlnNeighbor<RESIDUAL>();

template <>
template <>
const VariableSecond & MooseVariableFE<Real>::adSecondSlnNeighbor<RESIDUAL>();

template <>
template <>
const VariableValue & MooseVariableFE<Real>::adUDotNeighbor<RESIDUAL>();

template <>
template <>
const VectorVariableValue & MooseVariableFE<RealVectorValue>::adSln<RESIDUAL>();
>>>>>>> New changes to framework

  /// Holder for all the data associated with the "main" element
  std::unique_ptr<MooseVariableData<OutputType>> _element_data;

  /// Holder for all the data associated with the neighbor element
  std::unique_ptr<MooseVariableData<OutputType>> _neighbor_data;

  /// Holder for all the data associated with the lower dimeensional element
  std::unique_ptr<MooseVariableData<OutputType>> _lower_data;
};

template <typename OutputType>
template <ComputeStage compute_stage>
inline const MooseArray<typename Moose::RealType<compute_stage>::type> &
MooseVariableFE<OutputType>::adDofValues()
{
  return _element_data->template adDofValues<compute_stage>();
}

template <typename OutputType>
template <ComputeStage compute_stage>
inline const typename Moose::ValueType<OutputType, compute_stage>::type &
MooseVariableFE<OutputType>::adNodalValue()
{
  return _element_data->template adNodalValue<compute_stage>();
}
