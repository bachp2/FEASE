require("geometry")
local matrix = require("lua_matrix")
local symbol = matrix.symbol
table.sort(fe_constraints)

tau = {} -- angle of each element in global coordinate
for i = 1,#fe_elems do
	tau[i] = elem_angle(fe_elems[i])
	--print(theta[i]*180/math.pi)
end

element_stiffness_matrix = {} -- in global coordinate
for i = 1,#fe_elems do
	local L = elem_length(fe_elems[i]); local L3 = L^3; local L2 = L^2
	local a = A*E/L
	local b = E*Iz
	local c = 12*b/L3
	local d = 6*b/L2
	local e = 4*b/L
	local f = e/2
	local ke = matrix{
		{ a, 0, 0,-a, 0, 0},
		{ 0, c, d, 0,-c, d},	
		{ 0, d, e, 0,-d, f},
		{-a, 0, 0, a, 0, 0},
		{ 0,-c,-d, 0, c,-d},
		{ 0, d, f, 0,-d, e}};
	local c = math.cos(tau[i])
	local s = math.sin(tau[i])
	local R = matrix{
		{ c, s, 0, 0, 0, 0},
		{-s, c, 0, 0, 0, 0},	
		{ 0, 0, 1, 0, 0, 0},
		{ 0, 0, 0, c, s, 0},
		{ 0, 0, 0,-s, c, 0},
		{ 0, 0, 0, 0, 0, 1}};
	element_stiffness_matrix[i] = R^'T'*ke*R
end

local function _getEFT(ni,nj,n)
	local eft = {}
	for i = 1,n do
		table.insert(eft, n*ni-n+i)
	end
	for i = 1,n do
		table.insert(eft, n*nj-n+i)
	end

    return eft
end

global_stiffness_matrix = zeros(#fe_nodes*3,#fe_nodes*3);

