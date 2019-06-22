fe_nodes  = {}
fe_elems  = {}
fe_forces = {}

function elem_length(e)
	local dx = fe_nodes[e.n1].x-fe_nodes[e.n0].x
	local dy = fe_nodes[e.n1].y-fe_nodes[e.n0].y
	return math.sqrt(dx*dx+dy*dy)
end

function elem_angle(e)
	local dx = fe_nodes[e.n1].x-fe_nodes[e.n0].x
	local dy = fe_nodes[e.n1].y-fe_nodes[e.n0].y
	return math.atan(dy/dx);
end

function get_elem_stiff_mtx(theta, ke)
	local mtx = {}
	c = math.cos(theta)
	s = math.sin(theta)
	cc = c*c; sc = s*c; ss = s*s
	for i=1,4 do
		for j=1,4 do
			mtx
		end
	end
end