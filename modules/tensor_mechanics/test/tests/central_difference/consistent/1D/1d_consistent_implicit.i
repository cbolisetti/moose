# Test for central difference integration for a 1D element

[Mesh]
  type = GeneratedMesh
  xmin = 0
  xmax = 10
  nx = 5
  dim = 1
[]

[Variables]
  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./DynamicTensorMechanics]
    displacements = 'disp_x'
  [../]
  [./inertia_x]
    type = InertialForce
    variable = disp_x
  [../]
[]

[NodalKernels]
  [./force_x]
    type = UserForcingFunctionNodalKernel
    variable = disp_x
    boundary = right
    function = force_x
  [../]
[]

[Functions]
  [./force_x]
    type = PiecewiseLinear
    x = '0.0 1.0 2.0 3.0 4.0' # time
    y = '0.0 1.0 0.0 -1.0 0.0'  # force
    scale_factor = 1e3
  [../]
[]

[BCs]
  [./fixx1]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0.0
  [../]
[]

[Materials]
  [./elasticity_tensor_block]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1e6
    poissons_ratio = 0.25
    block = 0
  [../]
  [./strain_block]
    type = ComputeIncrementalSmallStrain
    block = 0
    displacements = 'disp_x'
  [../]
  [./stress_block]
    type = ComputeFiniteStrainElasticStress
    block = 0
  [../]
  [./density]
    type = GenericConstantMaterial
    block = 0
    prop_names = density
    prop_values = 150
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-8
  dtmin = 1e-4
  timestep_tolerance = 1e-6
  start_time = -0.005
  end_time = 4
  dt = 0.005
  [./TimeIntegrator]
    type = NewmarkBeta
    beta = 0.25
    gamma = 0.5
  [../]
[]

[Postprocessors]
  [./disp_x]
    type = PointValue
    point = '10.0 0.0 0.0'
    variable = disp_x
  [../]
[]

[Outputs]
  exodus = false
  csv = true
  perf_graph = false
  interval = 100
[]