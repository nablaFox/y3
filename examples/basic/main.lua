y3.add_global_script(
  y3.create_script({
    name = "switch_scene",
    update = function()
      if (y3.key_clicked(KEY_1)) then
        y3.switch_scene("other")
      end

      if (y3.key_clicked(KEY_2)) then
        y3.switch_scene("main")
      end
    end,
  })
)

return {
  require('entities.camera'),
  require('entities.floor')(),

  require('entities.outlined_brick')({
    position = Vec3.new(-2.5, 0.5, -5),
  }),

  y3.create_mesh({
    name = "Sphere1",
    mesh = y3.get_sphere(),
    position = Vec3.new(1.5, 0.5, -5),
    pitch = math.pi / 2,
    scale = Vec3.new(0.5),
    material = y3.create_grid_material({
      color = BLUE * 0.08,
      gridColor = BLUE,
      gridSpacing = 0.1,
      thickness = 0.01,
    })
  }),

  y3.create_mesh({
    name = "Sphere2",
    mesh = y3.get_sphere(),
    material = y3.create_point_material(GREEN),
    position = Vec3.new(0, 2.5, -9),
  }),
}
