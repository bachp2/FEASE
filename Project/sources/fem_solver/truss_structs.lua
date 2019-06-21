fe_nodes  = {}
fe_elems  = {}
fe_forces = {}

function elem_length(e)
	local dx = fe_nodes[e.n1].x-fe_nodes[e.n0].x
	local dy = fe_nodes[e.n1].y-fe_nodes[e.n0].y
	return math.sqrt(dx*dx+dy*dy)
end
