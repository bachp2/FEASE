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
fe_elems[8] = {n0=6,n1=5}

fe_forces[3] = {fy = -2000}
fe_forces[5] = {fx = 2000}
fe_forces[6] = {fy = 6000, fx = 4000}


A = 1.5 -- cross sectional area
E = 10e6 --modulus of elasticity

element_stiffness = {}
for i = 1,8 do
	element_stiffness[i] = A*E/elem_length(fe_elems[i])
	--print(element_stiffness[i])
end


