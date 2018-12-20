# Test for central difference integration for a 1D element

[Mesh]
  type = GeneratedMesh
  xmin = 0
  xmax = 10
  nx = 1
  dim = 1
[]

[Variables]
  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]
  # [./disp_y]
  #   order = FIRST
  #   family = LAGRANGE
  # [../]
  # [./disp_z]
  #   order = FIRST
  #   family = LAGRANGE
  # [../]
[]

[AuxVariables]
  # [./vel_x]
  # [../]
  # [./accel_x]
  # [../]
  # [./vel_y]
  # [../]
  # [./accel_y]
  # [../]
  # [./vel_z]
  # [../]
  # [./accel_z]
  # [../]
#   [./axial_stress]
#     order = CONSTANT
#     family = MONOMIAL
#   [../]
#   [./e_over_l]
#     order = CONSTANT
#     family = MONOMIAL
#   [../]
#   [./area]
#     order = CONSTANT
#     family = MONOMIAL
# #    initial_condition = 1.0
#   [../]
#   [./react_x]
#     order = FIRST
#     family = LAGRANGE
#   [../]
#   [./react_y]
#     order = FIRST
#     family = LAGRANGE
#   [../]
#   [./react_z]
#     order = FIRST
#     family = LAGRANGE
#   [../]
[]

[AuxKernels]
  # [./axial_stress]
  #   type = MaterialRealAux
  #   block = '0'
  #   property = axial_stress
  #   variable = axial_stress
  # [../]
  # [./e_over_l]
  #   type = MaterialRealAux
  #   block = '0'
  #   property = e_over_l
  #   variable = e_over_l
  # [../]
  # [./area]
  #   type = ConstantAux
  #   block = '0'
  #   variable = area
  #   value = 1.0
  #   execute_on = 'initial timestep_begin'
  # [../]
  # [./accel_x]
  #   type = NewmarkAccelAux
  #   variable = accel_x
  #   displacement = disp_x
  #   velocity = vel_x
  #   beta = 0.25
  #   execute_on = timestep_end
  # [../]
  # [./vel_x]
  #   type = NewmarkVelAux
  #   variable = vel_x
  #   acceleration = accel_x
  #   gamma = 0.5
  #   execute_on = timestep_end
  # [../]
  # [./accel_y]
  #   type = NewmarkAccelAux
  #   variable = accel_y
  #   displacement = disp_y
  #   velocity = vel_y
  #   beta = 0.25
  #   execute_on = timestep_end
  # [../]
  # [./vel_y]
  #   type = NewmarkVelAux
  #   variable = vel_y
  #   acceleration = accel_y
  #   gamma = 0.5
  #   execute_on = timestep_end
  # [../]
  # [./accel_z]
  #   type = NewmarkAccelAux
  #   variable = accel_z
  #   displacement = disp_z
  #   velocity = vel_z
  #   beta = 0.25
  #   execute_on = timestep_end
  # [../]
  # [./vel_z]
  #   type = NewmarkVelAux
  #   variable = vel_z
  #   acceleration = accel_z
  #   gamma = 0.5
  #   execute_on = timestep_end
  # [../]
[]

[Kernels]
  [./DynamicTensorMechanics]
    displacements = 'disp_x'
    # zeta = 0.00006366
  [../]
  [./inertia_x]
    type = InertialForce
    variable = disp_x
    # velocity = vel_x
    # acceleration = accel_x
    # beta = 0.25
    # gamma = 0.5
    # eta = 7.854
  [../]
  # [./solid_x]
  #   type = StressDivergenceTensorsTruss
  #   block = '0'
  #   displacements = 'disp_x'
  #   component = 0
  #   variable = disp_x
  #   area = area
  #   # save_in = react_x
  # [../]
  # [./solid_y]
  #   type = StressDivergenceTensorsTruss
  #   block = '0'
  #   displacements = 'disp_x disp_y disp_z'
  #   component = 1
  #   variable = disp_y
  #   area = area
  #   # save_in = react_y
  # [../]
  # [./solid_z]
  #   type = StressDivergenceTensorsTruss
  #   block = '0'
  #   displacements = 'disp_x disp_y disp_z'
  #   component = 2
  #   variable = disp_z
  #   area = area
  #   # save_in = react_z
  # [../]
[]

[NodalKernels]
  # [./x_nodal_mass]
  #   type = NodalTranslationalInertia
  #   block = '0'
  #   variable = disp_x
  #   boundary = right
  #   mass = 1e3
  #   # velocity = vel_x
  #   # acceleration = accel_x
  #   # beta = 0.25
  #   # gamma = 0.5
  #   # eta = 0.0162 # Rayleigh damping
  # [../]
  [./force_x]
    type = UserForcingFunctionNodalKernel
    variable = disp_x
    boundary = right
    function = force_x
  [../]
[]

[Functions]
  # [./x_right]
  #   type = PiecewiseLinear
  #   x = '0   1'
  #   y = '0 0.1'
  # [../]
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
  # [./fixx2]
  #   type = FunctionDirichletBC
  #   variable = disp_x
  #   boundary = right
  #   function = x_right
  #   # value = 0.0
  # [../]
  # [./fixy1]
  #   type = DirichletBC
  #   variable = disp_y
  #   boundary = left
  #   value = 0.0
  # [../]
  # [./fixy2]
  #   type = DirichletBC
  #   variable = disp_y
  #   boundary = right
  #   value = 0.0
  # [../]
  # [./fixz1]
  #   type = DirichletBC
  #   variable = disp_z
  #   boundary = left
  #   value = 0.0
  # [../]
  # [./fixz2]
  #   type = DirichletBC
  #   variable = disp_z
  #   boundary = right
  #   value = 0.0
  # [../]
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
    # store_stress_old = true
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
  end_time = 8
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
  # [./disp_y]
  #   type = PointValue
  #   point = '1.0 0.0 0.0'
  #   variable = disp_y
  # [../]
  # [./disp_z]
  #   type = PointValue
  #   point = '1.0 0.0 0.0'
  #   variable = disp_z
  # [../]
[]

# [Controls]
#   [./release_x_right]
#     type = TimePeriod
#     disable_objects = '*::fixx2'
#     start_time = '1'
#     end_time = '100'
#   [../]
# []

[Outputs]
  exodus = false
  csv = true
  perf_graph = false
[]
