local scene = {
  require('entities.camera'),

  y3.create_mesh({
    name = "Cube",
    mesh = y3.get_cube(),
    material = y3.create_color_material(RED)
  }),
}

return scene
