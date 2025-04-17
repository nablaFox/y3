local outlined_material_template = y3.create_material_template({
  shaders = { "brick_outline.frag", y3.default_vert_shader() },
})

return function(info)
  info = info or {}
  info.name = info.name or "Outlined Brick"
  info.color = info.color or WHITE
  info.outline = info.outline or BLACK
  info.outline_width = info.outline_width or 0.01

  local outlined_material = y3.create_material({
    template = outlined_material_template,
    params = {
      info.color,
      info.outline,
      info.outline_width,
    },
  })

  return y3.create_mesh({
    name = info.name,
    mesh = y3.get_cube(),
    material = outlined_material,
    position = info.position,
    scale = info.scale
  })
end
