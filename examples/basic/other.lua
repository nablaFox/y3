local materials = require('assets.materials')

local scene = {
  require('entities.camera'),

  y3.create_mesh({
    name = "Cube",
    mesh = y3.get_cube(),
    material = materials.outlined_material,
  }),
}

return scene
