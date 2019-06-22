-- Matrix = {}
-- Matrix.row = 0
-- Matrix.col = 0
-- Matrix._mat = {}

-- function Matrix:new(r,c)
-- 	--self = matrix
-- 	local mtx = {}
-- 	setmetatable(mtx, Matrix)
-- 	mtx.row = r
-- 	mtx.col = c
-- 	return mtx
-- end

-- function Matrix:set(i, j, value)
-- 	if(i>self.col or j>self.row) then
-- 		print("out of bound access")
-- 		return
-- 	end
-- 	-- _mat[i*row+j] = value;
-- end

-- function Matrix:get(i, j)
-- 	return _mat[i*row+j]
-- end
-- -- matrix:__len = function()
-- -- 	return row*col
-- -- end
-- mtx = Matrix:new(10,2)

-- mtx.set(2,2, 15.5)

-- print(mtx.get(2,2))
-- print(matrix)
local matrix = require("lua_matrix")
local symbol = matrix.symbol