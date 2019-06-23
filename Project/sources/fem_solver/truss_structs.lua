local matrix = require("lua_matrix")
local symbol = matrix.symbol

fe_nodes  = {}
fe_elems  = {}
fe_forces = {}

fe_constraints = {}

--fef_meta = {}; fef_meta.__index = fef_meta
--fe_forces = fef_meta

--hacky solution find better way to refer to indirectly refer to fe_forces
-- function fef_meta.__len()
-- 	km = 0
-- 	if not fe_forces then return 0 end
-- 	for k, v in ipairs(fe_forces) do
--     	if k and k > km then km = k end
--     end
--     return km
-- end

--setmetatable(fe_forces, fef_meta)

function vec_len(a)
	--if not self then return 0 end
	max_key = 0
	for k, _ in pairs(a) do
		if k > max_key then max_key = k end
	end
	return max_key
end

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
	c = math.cos(theta)
	s = math.sin(theta)
	local R = matrix{{c,s,0,0},{0,0,c,s}}
	return ke*R^'T'*matrix{{1,-1},{-1,1}}*R
end

function _getEFT(ni,nj)
    eft = {2*ni-1,2*ni,2*nj-1,2*nj}
    return eft
end

function assemble_glob_stiff_mtx(K, Ke, elems)
	for e=1,#elems do
		ni, nj = elems[e].n0, elems[e].n1
		eft = _getEFT(ni,nj)
		ke = Ke[e]
		for i=1,4 do
			ii = eft[i]
			for j=i,4 do
				jj = eft[j]
				K[ii][jj] = K[ii][jj] + ke[i][j]
				K[jj][ii] = K[ii][jj]
			end
		end
	end
	return K
end

function strike_out_zeros_constraint(K, constraints)
	--print(constraints)
	local k = matrix.copy(K)
	--TODO: not efficient but it works for now
	for i=#constraints,1,-1 do
		for j=1,#K do 
			table.remove(k[j], constraints[i]*2)
			table.remove(k[j], constraints[i]*2-1)
		end
	end
	for i=#constraints,1,-1 do
		table.remove(k, constraints[i]*2)
		table.remove(k, constraints[i]*2-1)
	end
	--print(matrix.tostring(K,"%.0f"))
	return k
end

function get_forces_vector(f,c)
	-- parameter forces and constraints
	local _f = zeros(vec_len(f)*2,1)
	for k, t in pairs(f) do
		if t.fx then _f[k*2-1][1] = t.fx end
		if t.fy then _f[k*2][1] = t.fy end
	end
	for i=#c,1,-1 do
		table.remove(_f, c[i]*2)
		table.remove(_f, c[i]*2-1)
	end
	--print(#_f)
	return _f
end	

-- mtx = matrix{{1,2,3},{4,5,6},{7,8,9}}
-- for i=1,#mtx do 
-- 	print(mtx[i][1])
-- end
