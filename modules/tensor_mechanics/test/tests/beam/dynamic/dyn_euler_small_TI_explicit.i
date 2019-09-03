# Test for small strain euler beam vibration in y direction

# An impulse load is applied at the end of a cantilever beam of length 4m.
# The properties of the cantilever beam are as follows:
# Young's modulus (E) = 1e4
# Shear modulus (G) = 4e7
# Shear coefficient (k) = 1.0
# Cross-section area (A) = 0.01
# Iy = 1e-4 = Iz
# Length (L)= 4 m
# density (rho) = 1.0

# For this beam, the dimensionless parameter alpha = kAGL^2/EI = 6.4e6
# Therefore, the beam behaves like a Euler-Bernoulli beam.

# The theoretical first and third frequencies of this beam are:
# f1 = 1/(2 pi) * (3.5156/L^2) * sqrt(EI/rho)
# f2 = 6.268 f1

# This implies that the corresponding time period of this beam are 2.858 s and 0.455s

# The FEM solution for this beam with 10 element gives time periods of 2.856 s and 0.4505s with a time step of 0.01.
# A smaller time step is required to obtain a closer match for the lower time periods/higher frequencies.
# A higher time step of 0.05 is used in this test to reduce testing time.

# The time history from this analysis matches with that obtained from Abaqus.

# Values from the first few time steps are as follows:
# time       disp_y            vel_y            accel_y
# 0     0.0                  0.0                0.0
# 0.05  0.0016523559162602   0.066094236650407  2.6437694660163
# 0.1   0.0051691308901533   0.07457676230532  -2.3044684398197
# 0.15  0.0078956772343372   0.03448509146203   4.7008016060883
# 0.2   0.0096592517031463   0.03605788729033  -0.63788977295649
# 0.25  0.011069233444348    0.020341382357756  0.0092295756535376

[Mesh]
  type = GeneratedMesh
  xmin = 0.0
  xmax = 4.0
  dim = 1
  nx = 1
  displacements = 'disp_x disp_y disp_z'
[]

[Variables]
  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]
  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]
  [./disp_z]
    order = FIRST
    family = LAGRANGE
  [../]
  [./rot_x]
    order = FIRST
    family = LAGRANGE
  [../]
  [./rot_y]
    order = FIRST
    family = LAGRANGE
  [../]
  [./rot_z]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./vel_x]
  order = FIRST
  family = LAGRANGE
  [../]
  [./vel_y]
  order = FIRST
  family = LAGRANGE
  [../]
  [./vel_z]
  order = FIRST
  family = LAGRANGE
  [../]
  [./accel_x]
  order = FIRST
  family = LAGRANGE
  [../]
  [./accel_y]
  order = FIRST
  family = LAGRANGE
  [../]
  [./accel_z]
  order = FIRST
  family = LAGRANGE
  [../]
  [./rot_vel_x]
  order = FIRST
  family = LAGRANGE
  [../]
  [./rot_vel_y]
  order = FIRST
  family = LAGRANGE
  [../]
  [./rot_vel_z]
  order = FIRST
  family = LAGRANGE
  [../]
  [./rot_accel_x]
  order = FIRST
  family = LAGRANGE
  [../]
  [./rot_accel_y]
  order = FIRST
  family = LAGRANGE
  [../]
  [./rot_accel_z]
  order = FIRST
  family = LAGRANGE
  [../]
[]

[AuxKernels]
  [./accel_x] # These auxkernels are only to check output
    type = TestNewmarkTI
    displacement = disp_x
    variable = accel_x
    first = false
  [../]
  [./accel_y]
    type = TestNewmarkTI
    displacement = disp_y
    variable = accel_y
    first = false
  [../]
  [./accel_z]
    type = TestNewmarkTI
    displacement = disp_z
    variable = accel_z
    first = false
  [../]
  [./vel_x]
    type = TestNewmarkTI
    displacement = disp_x
    variable = vel_x
  [../]
  [./vel_y]
    type = TestNewmarkTI
    displacement = disp_y
    variable = vel_y
  [../]
  [./vel_z]
    type = TestNewmarkTI
    displacement = disp_z
    variable = vel_z
  [../]
  [./rot_accel_x]
    type = TestNewmarkTI
    displacement = rot_x
    variable = rot_accel_x
    first = false
  [../]
  [./rot_accel_y]
    type = TestNewmarkTI
    displacement = rot_y
    variable = rot_accel_y
    first = false
  [../]
  [./rot_accel_z]
    type = TestNewmarkTI
    displacement = rot_z
    variable = rot_accel_z
    first = false
  [../]
  [./rot_vel_x]
    type = TestNewmarkTI
    displacement = rot_x
    variable = rot_vel_x
  [../]
  [./rot_vel_y]
    type = TestNewmarkTI
    displacement = rot_y
    variable = rot_vel_y
  [../]
  [./rot_vel_z]
    type = TestNewmarkTI
    displacement = rot_z
    variable = rot_vel_z
  [../]
[]

[BCs]
  [./fixx1]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0.0
  [../]
  [./fixy1]
    type = DirichletBC
    variable = disp_y
    boundary = left
    value = 0.0
  [../]
  [./fixz1]
    type = DirichletBC
    variable = disp_z
    boundary = left
    value = 0.0
  [../]
  [./fixr1]
    type = DirichletBC
    variable = rot_x
    boundary = left
    value = 0.0
  [../]
  [./fixr2]
    type = DirichletBC
    variable = rot_y
    boundary = left
    value = 0.0
  [../]
  [./fixr3]
    type = DirichletBC
    variable = rot_z
    boundary = left
    value = 0.0
  [../]
[]

[NodalKernels]
  [./force_y2]
    type = UserForcingFunctionNodalKernel
    variable = disp_y
    boundary = right
    function = force
  [../]
[]

[Functions]
  [./force]
    type = PiecewiseLinear
    x = '0.0 0.05 0.1 10.0'
    y = '0.0 0.01  0.0  0.0'
  [../]
[]

[Kernels]
  [./solid_disp_x]
    type = StressDivergenceBeam
    block = '0'
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    component = 0
    variable = disp_x
  [../]
  [./solid_disp_y]
    type = StressDivergenceBeam
    block = '0'
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    component = 1
    variable = disp_y
  [../]
  [./solid_disp_z]
    type = StressDivergenceBeam
    block = '0'
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    component = 2
    variable = disp_z
  [../]
  [./solid_rot_x]
    type = StressDivergenceBeam
    block = '0'
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    component = 3
    variable = rot_x
  [../]
  [./solid_rot_y]
    type = StressDivergenceBeam
    block = '0'
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    component = 4
    variable = rot_y
  [../]
  [./solid_rot_z]
    type = StressDivergenceBeam
    block = '0'
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    component = 5
    variable = rot_z
  [../]
  [./inertial_force_x]
    type = InertialForceBeam
    block = 0
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    # velocities = 'vel_x vel_y vel_z'
    # accelerations = 'accel_x accel_y accel_z'
    # rotational_velocities = 'rot_vel_x rot_vel_y rot_vel_z'
    # rotational_accelerations = 'rot_accel_x rot_accel_y rot_accel_z'
    # beta = 0.25
    # gamma = 0.5
    area = 0.01
    Iy = 1e-4
    Iz = 1e-4
    Ay = 0.0
    Az = 0.0
    component = 0
    variable = disp_x
  [../]
  [./inertial_force_y]
    type = InertialForceBeam
    block = 0
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    # velocities = 'vel_x vel_y vel_z'
    # accelerations = 'accel_x accel_y accel_z'
    # rotational_velocities = 'rot_vel_x rot_vel_y rot_vel_z'
    # rotational_accelerations = 'rot_accel_x rot_accel_y rot_accel_z'
    # beta = 0.25
    # gamma = 0.5
    area = 0.01
    Iy = 1e-4
    Iz = 1e-4
    Ay = 0.0
    Az = 0.0
    component = 1
    variable = disp_y
  [../]
  [./inertial_force_z]
    type = InertialForceBeam
    block = 0
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    # velocities = 'vel_x vel_y vel_z'
    # accelerations = 'accel_x accel_y accel_z'
    # rotational_velocities = 'rot_vel_x rot_vel_y rot_vel_z'
    # rotational_accelerations = 'rot_accel_x rot_accel_y rot_accel_z'
    # beta = 0.25
    # gamma = 0.5
    area = 0.01
    Iy = 1e-4
    Iz = 1e-4
    Ay = 0.0
    Az = 0.0
    component = 2
    variable = disp_z
  [../]
  [./inertial_force_rot_x]
    type = InertialForceBeam
    block = 0
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    # velocities = 'vel_x vel_y vel_z'
    # accelerations = 'accel_x accel_y accel_z'
    # rotational_velocities = 'rot_vel_x rot_vel_y rot_vel_z'
    # rotational_accelerations = 'rot_accel_x rot_accel_y rot_accel_z'
    # beta = 0.25
    # gamma = 0.5
    area = 0.01
    Iy = 1e-4
    Iz = 1e-4
    Ay = 0.0
    Az = 0.0
    component = 3
    variable = rot_x
  [../]
  [./inertial_force_rot_y]
    type = InertialForceBeam
    block = 0
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    # velocities = 'vel_x vel_y vel_z'
    # accelerations = 'accel_x accel_y accel_z'
    # rotational_velocities = 'rot_vel_x rot_vel_y rot_vel_z'
    # rotational_accelerations = 'rot_accel_x rot_accel_y rot_accel_z'
    # beta = 0.25
    # gamma = 0.5
    area = 0.01
    Iy = 1e-4
    Iz = 1e-4
    Ay = 0.0
    Az = 0.0
    component = 4
    variable = rot_y
  [../]
  [./inertial_force_rot_z]
    type = InertialForceBeam
    block = 0
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    # velocities = 'vel_x vel_y vel_z'
    # accelerations = 'accel_x accel_y accel_z'
    # rotational_velocities = 'rot_vel_x rot_vel_y rot_vel_z'
    # rotational_accelerations = 'rot_accel_x rot_accel_y rot_accel_z'
    # beta = 0.25
    # gamma = 0.5
    area = 0.01
    Iy = 1e-4
    Iz = 1e-4
    Ay = 0.0
    Az = 0.0
    component = 5
    variable = rot_z
  [../]
[]

# [Preconditioning]
#   [./smp]
#     type = SMP
#     full = true
#   [../]
# []

[Executioner]
  type = Transient
  # solve_type = NEWTON

  dt = 1e-4
  end_time = 5.0
  timestep_tolerance = 1e-10
  # [./TimeIntegrator]
  #   type = NewmarkBeta
  #   beta = 0.25
  #   gamma = 0.5
  # [../]
  [./TimeIntegrator]
    type = CentralDifference
  [../]
[]

[Materials]
  [./elasticity]
    type = ComputeElasticityBeam
    youngs_modulus = 1.0e4
    poissons_ratio = -0.999875
    shear_coefficient = 1.0
    block = 0
  [../]
  [./strain]
    type = ComputeIncrementalBeamStrain
    block = '0'
    displacements = 'disp_x disp_y disp_z'
    rotations = 'rot_x rot_y rot_z'
    area = 0.01
    Ay = 0.0
    Az = 0.0
    Iy = 1.0e-4
    Iz = 1.0e-4
    y_orientation = '0.0 1.0 0.0'
    implicit = false
  [../]
  [./stress]
    type = ComputeBeamResultants
    block = 0
  [../]
  [./density]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'density'
    prop_values = '1.0'
  [../]
[]

[Postprocessors]
  [./disp_x]
    type = PointValue
    point = '4.0 0.0 0.0'
    variable = disp_x
  [../]
  [./disp_y]
    type = PointValue
    point = '4.0 0.0 0.0'
    variable = disp_y
  [../]
  [./vel_y]
    type = PointValue
    point = '4.0 0.0 0.0'
    variable = vel_y
  [../]
  [./accel_y]
    type = PointValue
    point = '4.0 0.0 0.0'
    variable = accel_y
  [../]
[]

[Outputs]
  exodus = true
  csv = true
  perf_graph = true
[]
