require("default")
local matrix = require("lua_matrix")
local symbol = matrix.symbol

table.sort(fe_constraints)

element_stiffness = {}
for i = 1,#fe_elems do
	element_stiffness[i] = A*E/elem_length(fe_elems[i])
	--print(element_stiffness[i])
end

theta = {}
for i = 1,#fe_elems do
	theta[i] = elem_angle(fe_elems[i])
	--print(theta[i]*180/math.pi)
end
theta[6] = math.pi*0.75

element_stiffness_matrix = {}
for i = 1,#fe_elems do
	element_stiffness_matrix[i] = get_elem_stiff_mtx(theta[i],element_stiffness[i])
	--print(element_stiffness_matrix[i])
	--print()
end

global_stiffness_matrix = zeros(#fe_nodes*2,#fe_nodes*2);
global_stiffness_matrix = assemble_glob_stiff_mtx(global_stiffness_matrix, element_stiffness_matrix, fe_elems) 

F = get_forces_vector(fe_forces, fe_constraints)

K = strike_out_zeros_constraint(global_stiffness_matrix, fe_constraints)
--F = force_strike_out_zero_constraints(forces_vec, fe_constraints)
for i=1,#K do
	for j=1,#K[1] do
		K[i][j] = tonumber(string.format("%.7f", K[i][j]))
	end
end
U = matrix.invert(K)*F

K_f = zeros(#fe_constraints*2,matrix.rows(U))
for i=1,#fe_constraints do
	K_f[i*2-1] = global_stiffness_matrix[2*fe_constraints[i]-1]
	K_f[i*2] = global_stiffness_matrix[2*fe_constraints[i]]
end

for i=#fe_constraints,1,-1 do
	for j=1,#K_f do 
		table.remove(K_f[j], fe_constraints[i]*2)
		table.remove(K_f[j], fe_constraints[i]*2-1)
	end
end
force_at_boundary_conds = K_f*U
print(matrix.tostring(force_at_boundary_conds,"%f"))

axial_strains = calc_axial_strain(U, fe_constraints, fe_elems, theta)
--print(matrix.tostring(axial_strains))
axial_stresses = E*axial_strains
print(matrix.tostring(axial_stresses))