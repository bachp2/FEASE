local matrix = require("lua_matrix")
local symbol = matrix.symbol
require("truss_structs")
--- FEM NODES
fe_nodes[1] = {x=0,y=0}
fe_nodes[2] = {x=0,y=40}
fe_nodes[3] = {x=40,y=0}
fe_nodes[4] = {x=40,y=40}
fe_nodes[5] = {x=80,y=0}
fe_nodes[6] = {x=80,y=40}
--- FEM ELEMENTS
fe_elems[1] = {n0=1,n1=3}
fe_elems[2] = {n0=1,n1=4}
fe_elems[3] = {n0=2,n1=4}
fe_elems[4] = {n0=3,n1=4}
fe_elems[5] = {n0=3,n1=5}
fe_elems[6] = {n0=4,n1=5}
fe_elems[7] = {n0=4,n1=6}
fe_elems[8] = {n0=5,n1=6}

fe_forces[3] = {fy = -2000}
fe_forces[5] = {fx = 2000}
fe_forces[6] = {fy = 6000, fx = 4000}

fe_constraints = {1,2}
table.sort(fe_constraints)
A = 1.5 -- cross sectional area
E = 10e6 --modulus of elasticity

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
print(matrix.tostring(force_at_boundary_conds,"%.2f"))