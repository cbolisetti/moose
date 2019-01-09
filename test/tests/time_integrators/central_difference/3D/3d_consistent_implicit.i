# One element test to test the central difference time integrator in 3D.
# The back surface of the element (y=0) is fixed and the front surface (y=1)
# is moved by applying a cyclic force.

[Mesh]
  type = GeneratedMesh # Can generate simple lines, rectangles and rectangular prisms
  dim = 3 # Dimension of the mesh
  nx = 1 # Number of elements in the x direction
  ny = 1 # Number of elements in the y direction
  nz = 1 # Number of elements in the z direction
  xmin = 0.0
  xmax = 1
  ymin = 0.0
  ymax = 1
  zmin = 0.0
  zmax = 1
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
[]

[AuxVariables]
  [./vel_x]
  [../]
  [./accel_x]
  [../]
  [./vel_y]
  [../]
  [./accel_y]
  [../]
  [./vel_z]
  [../]
  [./accel_z]
  [../]
  # [./stress_xy]
  #   order = CONSTANT
  #   family = MONOMIAL
  # [../]
  # [./stress_yz]
  #   order = CONSTANT
  #   family = MONOMIAL
  # [../]
  # [./stress_zx]
  #   order = CONSTANT
  #   family = MONOMIAL
  # [../]
  # [./strain_xy]
  #   order = CONSTANT
  #   family = MONOMIAL
  # [../]
  # [./strain_yz]
  #   order = CONSTANT
  #   family = MONOMIAL
  # [../]
  # [./strain_zx]
  #   order = CONSTANT
  #   family = MONOMIAL
  # [../]
  # [./stress_xx]
  #   order = CONSTANT
  #   family = MONOMIAL
  # [../]
  # [./stress_yy]
  #   order = CONSTANT
  #   family = MONOMIAL
  # [../]
  # [./stress_zz]
  #   order = CONSTANT
  #   family = MONOMIAL
  # [../]
  # [./strain_xx]
  #   order = CONSTANT
  #   family = MONOMIAL
  # [../]
  # [./strain_yy]
  #   order = CONSTANT
  #   family = MONOMIAL
  # [../]
  # [./strain_zz]
  #   order = CONSTANT
  #   family = MONOMIAL
  # [../]
[]

[Kernels]
  [./DynamicTensorMechanics]
    displacements = 'disp_x disp_y disp_z'
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
  [./inertia_y]
    type = InertialForce
    variable = disp_y
    # velocity = vel_y
    # acceleration = accel_y
    # beta = 0.25
    # gamma = 0.5
    # eta = 7.854
  [../]
  [./inertia_z]
    type = InertialForce
    variable = disp_z
    # velocity = vel_z
    # acceleration = accel_z
    # beta = 0.25
    # gamma = 0.5
    # eta = 7.854
  [../]
[]

[AuxKernels]
  [./accel_x]
    type = TestNewmarkTI
    variable = accel_x
    displacement = disp_x
    first = false
  [../]
  [./vel_x]
    type = TestNewmarkTI
    variable = vel_x
    displacement = disp_x
  [../]
  [./accel_y]
    type = TestNewmarkTI
    variable = accel_y
    displacement = disp_y
    first = false
  [../]
  [./vel_y]
    type = TestNewmarkTI
    variable = vel_y
    displacement = disp_x
  [../]
  [./accel_z]
    type = TestNewmarkTI
    variable = accel_z
    displacement = disp_z
    first = false
  [../]
  [./vel_z]
    type = TestNewmarkTI
    variable = vel_z
    displacement = disp_z
  [../]
#   [./accel_x]
#     type = NewmarkAccelAux
#     variable = accel_x
#     displacement = disp_x
#     velocity = vel_x
#     beta = 0.25
#     execute_on = timestep_end
#   [../]
#   [./vel_x]
#     type = NewmarkVelAux
#     variable = vel_x
#     acceleration = accel_x
#     gamma = 0.5
#     execute_on = timestep_end
#   [../]
#   [./accel_y]
#     type = NewmarkAccelAux
#     variable = accel_y
#     displacement = disp_y
#     velocity = vel_y
#     beta = 0.25
#     execute_on = timestep_end
#   [../]
#   [./vel_y]
#     type = NewmarkVelAux
#     variable = vel_y
#     acceleration = accel_y
#     gamma = 0.5
#     execute_on = timestep_end
#   [../]
#   [./accel_z]
#     type = NewmarkAccelAux
#     variable = accel_z
#     displacement = disp_z
#     velocity = vel_z
#     beta = 0.25
#     execute_on = timestep_end
#   [../]
#   [./vel_z]
#     type = NewmarkVelAux
#     variable = vel_z
#     acceleration = accel_z
#     gamma = 0.5
#     execute_on = timestep_end
#   [../]
#   [./stress_xy]
#     type = RankTwoAux
#     rank_two_tensor = stress
#     variable = stress_xy
#     index_i = 1
#     index_j = 0
#   [../]
#   [./stress_yz]
#     type = RankTwoAux
#     rank_two_tensor = stress
#     variable = stress_yz
#     index_i = 2
#     index_j = 1
#   [../]
#   [./stress_zx]
#     type = RankTwoAux
#     rank_two_tensor = stress
#     variable = stress_zx
#     index_i = 0
#     index_j = 2
#   [../]
#   [./strain_xy]
#     type = RankTwoAux
#     rank_two_tensor = total_strain
#     variable = stress_xy
#     index_i = 1
#     index_j = 0
#   [../]
#   [./strain_yz]
#     type = RankTwoAux
#     rank_two_tensor = total_strain
#     variable = strain_yz
#     index_i = 2
#     index_j = 1
#   [../]
#   [./strain_zx]
#     type = RankTwoAux
#     rank_two_tensor = total_strain
#     variable = strain_zx
#     index_i = 0
#     index_j = 2
#   [../]
#   [./stress_xx]
#     type = RankTwoAux
#     rank_two_tensor = stress
#     variable = stress_xx
#     index_i = 0
#     index_j = 0
#   [../]
#   [./stress_yy]
#     type = RankTwoAux
#     rank_two_tensor = stress
#     variable = stress_yy
#     index_i = 1
#     index_j = 1
#   [../]
#   [./stress_zz]
#     type = RankTwoAux
#     rank_two_tensor = stress
#     variable = stress_zz
#     index_i = 2
#     index_j = 2
#   [../]
#   [./strain_xx]
#     type = RankTwoAux
#     rank_two_tensor = total_strain
#     variable = strain_xx
#     index_i = 0
#     index_j = 0
#   [../]
#   [./strain_yy]
#     type = RankTwoAux
#     rank_two_tensor =total_strain
#     variable = strain_yy
#     index_i = 1
#     index_j = 1
#   [../]
#   [./strain_zz]
#     type = RankTwoAux
#     rank_two_tensor = total_strain
#     variable = strain_zz
#     index_i = 2
#     index_j = 2
#   [../]
[]

[BCs]
  # [./x_bot]
  #   type = PresetBC
  #   variable = disp_x
  #   boundary = bottom
  #   value = 0.0
  # [../]
  [./y_bot]
    type = PresetBC
    variable = disp_y
    boundary = 'back'
    value = 0.0
  [../]
  [./z_bot]
    type = PresetBC
    variable = disp_z
    boundary = 'back'
    value = 0.0
  [../]
  [./x_bot]
    type = PresetDisplacement
    boundary = 'back'
    variable = disp_x
    beta = 0.25
    velocity = vel_x
    acceleration = accel_x
    function = disp
  [../]
  [./Periodic]
    [./x_dir]
      variable = 'disp_x disp_y disp_z'
      primary = 'left'
      secondary = 'right'
      translation = '1.0 0.0 0.0'
    [../]
    [./y_dir]
      variable = 'disp_x disp_y disp_z'
      primary = 'bottom'
      secondary = 'top'
      translation = '0.0 1.0 0.0'
    [../]
  [../]
  # [./top_x]
  #   type = PresetDisplacement
  #   boundary = 5
  #   variable = disp_x
  #   beta = 0.25
  #   velocity = vel_x
  #   acceleration = accel_x
  #   function = top_disp
  # [../]
[]

[NodalKernels]
  # [./x_nodal_mass]
  #   type = NodalTranslationalInertia
  #   block = '0'
  #   variable = disp_x
  #   boundary = right
  #   mass = 1e3
  #   central_difference = true
  # [../]
  # [./force_x]
  #   type = UserForcingFunctionNodalKernel
  #   variable = disp_x
  #   boundary = top
  #   function = force
  # [../]
  # [./force_y]
  #   type = UserForcingFunctionNodalKernel
  #   variable = disp_y
  #   boundary = top
  #   function = force
  # [../]
[]

[Functions]
  [./disp]
    type = PiecewiseLinear
    x = '0.0 1.0 2.0 3.0 4.0' # time
    y = '0.0 1.0 0.0 -1.0 0.0'  # displacement
  [../]
  [./force_x]
    type = PiecewiseLinear
    x = '0.0 1.0 2.0 3.0 4.0' # time
    y = '0.0 1.0 0.0 -1.0 0.0'  # force
    # x = '0.0 0.1 0.11 3.0 4.0' # time
    # y = '0.0 1.0 0 0 0' # force
    scale_factor = 1e3
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
    displacements = 'disp_x disp_y disp_z'
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
    prop_values = 1e4
  [../]
[]

[Preconditioning]
  [./andy]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-08
  timestep_tolerance = 1e-6
  start_time = -0.01
  end_time = 8
  dt = 0.001
  [./TimeIntegrator]
    type = NewmarkBeta
    beta = 0.25
    gamma = 0.5
  [../]
[]

[Postprocessors]
  [./_dt]
    type = TimestepSize
  [../]
  [./disp_6x]
    type = NodalVariableValue
    nodeid = 6
    variable = disp_x
  [../]
  [./disp_6y]
    type = NodalVariableValue
    nodeid = 6
    variable = disp_y
  [../]
  [./disp_6z]
    type = NodalVariableValue
    nodeid = 6
    variable = disp_z
  [../]
  # [./disp_6z]
  #   type = NodalVariableValue
  #   nodeid = 6
  #   variable = disp_z
  # [../]
  # [./vel_6x]
  #   type = NodalVariableValue
  #   nodeid = 6
  #   variable = vel_x
  # [../]
  # [./vel_6y]
  #   type = NodalVariableValue
  #   nodeid = 6
  #   variable = vel_y
  # [../]
  # [./vel_6z]
  #   type = NodalVariableValue
  #   nodeid = 6
  #   variable = vel_z
  # [../]
  # [./accel_6x]
  #   type = NodalVariableValue
  #   nodeid = 6
  #   variable = accel_x
  # [../]
  # [./accel_6y]
  #   type = NodalVariableValue
  #   nodeid = 6
  #   variable = accel_y
  # [../]
  # [./accel_6z]
  #   type = NodalVariableValue
  #   nodeid = 6
  #   variable = accel_z
  # [../]
  # [./stress_xy_el]
  #   type = ElementalVariableValue
  #   variable = stress_xy
  #   elementid = 0
  # [../]
  # [./stress_yz_el]
  #   type = ElementalVariableValue
  #   variable = stress_yz
  #   elementid = 0
  # [../]
  # [./stress_zx_el]
  #   type = ElementalVariableValue
  #   variable = stress_zx
  #   elementid = 0
  # [../]
  # [./strain_xy_el]
  #   type = ElementalVariableValue
  #   variable = strain_xy
  #   elementid = 0
  # [../]
  # [./strain_yz_el]
  #   type = ElementalVariableValue
  #   variable = strain_yz
  #   elementid = 0
  # [../]
  # [./strain_zx_el]
  #   type = ElementalVariableValue
  #   variable = strain_zx
  #   elementid = 0
  # [../]
  # [./stress_xx_el]
  #   type = ElementalVariableValue
  #   variable = stress_xx
  #   elementid = 0
  # [../]
  # [./stress_yy_el]
  #   type = ElementalVariableValue
  #   variable = stress_yy
  #   elementid = 0
  # [../]
  # [./stress_zz_el]
  #   type = ElementalVariableValue
  #   variable = stress_zz
  #   elementid = 0
  # [../]
  # [./strain_xx_el]
  #   type = ElementalVariableValue
  #   variable = strain_xx
  #   elementid = 0
  # [../]
  # [./strain_yy_el]
  #   type = ElementalVariableValue
  #   variable = strain_yy
  #   elementid = 0
  # [../]
  # [./strain_zz_el]
  #   type = ElementalVariableValue
  #   variable = strain_zz
  #   elementid = 0
  # [../]
[]

[Outputs]
  exodus = false
  csv = true
  perf_graph = false
[]
