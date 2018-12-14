# Test for central difference integration for a single truss element

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
[]

[AuxVariables]
  [./area]
    order = CONSTANT
    family = MONOMIAL
#    initial_condition = 1.0
  [../]
[]

[AuxKernels]
  [./area]
    type = ConstantAux
    block = '0'
    variable = area
    value = 1.0
    execute_on = 'initial timestep_begin'
  [../]
[]

[Kernels]
  [./solid_x]
    type = StressDivergenceTensorsTruss
    block = '0'
    displacements = 'disp_x'
    component = 0
    variable = disp_x
    area = area
    # save_in = react_x
  [../]
#  [./inertial_force]
#    type = InertialForce
#    block = '0'
#    variable = 'disp_x'
#  [../]
[]

[NodalKernels]
  [./x_nodal_mass]
    type = NodalTranslationalInertia
    block = '0'
    variable = disp_x
    boundary = 'left right'
    mass = 1e3
  [../]
  [./force_x]
    type = UserForcingFunctionNodalKernel
    variable = disp_x
    boundary = right
    function = force_x
  [../]
[]

[Functions]
  [./x_right]
    type = PiecewiseLinear
    x = '0   1'
    y = '0 0.1'
  [../]
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
  #   type = DirichletBC
  #   variable = disp_x
  #   boundary = right
  #   # function = x_right
  #   value = 0.0
  # [../]
[]

[Materials]
  [./linelast]
    type = LinearElasticTruss
    block = '0'
    youngs_modulus = 1e6
    displacements = 'disp_x'
  [../]
  # [./density]
  #   type = GenericConstantMaterial
  #   prop_names = 'density'
  #   prop_values = '1000'
  # [../]
[]

[Executioner]
  type = Transient
  solve_type = PJFNK
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-8
  dtmin = 1e-4
  timestep_tolerance = 1e-6
  start_time = -0.05
  end_time = 2.0
  dt = 0.005
  [./TimeIntegrator]
    type = CentralDifference
#    solve_type = lumped
  [../]
[]

[Postprocessors]
  [./disp_x]
    type = PointValue
    point = '10.0 0.0 0.0'
    variable = disp_x
  [../]
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
  exodus = true
  csv = true
  perf_graph = true
[]
