require("truss_structs")
--- FEM NODES
fe_nodes[0] = {x=-0.100,y=-0.050}
fe_nodes[1] = {x=-0.100,y=0.050}
fe_nodes[2] = {x=0.000,y=0.050}
fe_nodes[3] = {x=0.000,y=-0.050}
fe_nodes[4] = {x=0.100,y=-0.050}
fe_nodes[5] = {x=0.100,y=0.050}
--- FEM ELEMENTS
fe_elems[0] = {n0=0,n1=3}
fe_elems[2] = {n0=3,n1=4}
fe_elems[4] = {n0=5,n1=4}
fe_elems[6] = {n0=2,n1=5}
fe_elems[8] = {n0=1,n1=2}
fe_elems[10] = {n0=2,n1=3}
fe_elems[12] = {n0=1,n1=3}
fe_elems[14] = {n0=3,n1=5}

