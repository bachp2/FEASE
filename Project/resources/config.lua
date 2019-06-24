

shader_paths = {
	-- for rendering text and bitmap icons
	text_shader = { nid = "bitmap", "resources/shaders/texture.vs", "resources/shaders/text.fs"},
	-- render line with one of 3 fundamental colors, eg. green, red, blue. Only usecase for this is rendering pointer axes
	solid_shader = { nid = "sline", "resources/shaders/solid.vs", "resources/shaders/solid.fs"},
	-- a generic shader to render object
	object_shader = { nid = "object", "resources/shaders/object.vs", "resources/shaders/object.fs"}
}

-- print(shader_paths.text_shader.shader_id)
-- print(shader_paths.text_shader[1])
-- print(shader_paths.text_shader[2])

dot = { 
	color = "#FF002D"
}

line = {
	color = "#995c0d"
}

background = {
	-- color = {"#4286f4"}
	color_gradient = {"#4286f4", "#85aef2"}
}

grid = {
	color = "#C0C0C0"
}

text = {
	color = "#1c1c1c"
}
