return function(info)
  info = info or {}
  info.name = info.name or "Floor"
  info.color = info.color or PURPLE:set_alpha(0.3)
  info.grid_size = 1
  info.line_thickness = 0.02
  info.height = 0
  info.floor_scale = 1e3

  local mesh = y3.get_quad()

  local main_grid_material = y3.create_grid_material_transparent({
    color = INVISIBLE,
    gridColor = info.color,
    gridSpacing = info.grid_size / info.floor_scale,
    thickness = info.line_thickness / info.floor_scale,
  })

  local sub_grid_material = y3.create_grid_material_transparent({
    color = INVISIBLE,
    gridColor = info.color * 0.8,
    gridSpacing = info.grid_size / (info.floor_scale * 2),
    thickness = info.line_thickness / info.floor_scale
  })

  local transform = Transform.new({
    position = Vec3.new(0, info.height, 0),
    scale = Vec3.new(info.floor_scale),
    pitch = -math.pi / 2,
  })

  local floor = y3.create_root("Floor", transform)

  floor:add(y3.create_mesh({
    name = "Main Grid",
    mesh = mesh,
    material = main_grid_material,
  }))

  floor:add(y3.create_mesh({
    name = "Sub Grid",
    mesh = mesh,
    material = sub_grid_material,
  }))

  return floor
end
