local outlined_material_template = y3.create_material_template({
  shaders = { "brick_outline.frag", y3.default_vert_shader() },
})

local function create_outlined_material(params)
  return y3.create_material({
    template = outlined_material_template,
    params = params or {
      WHITE,
      BLACK,
      0.01,
    },
  })
end

return {
  create_outlined_material = create_outlined_material,
  outlined_material = create_outlined_material(),
}
