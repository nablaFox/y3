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
    data = {}
  })
)

return {
  require('entities.camera'),

  y3.create_mesh({
    name = "Sphere",
    mesh = y3.get_sphere(),
    material = y3.create_grid_material({
      color = BLUE * 0.08,
      gridColor = BLUE,
      gridSpacing = 0.1,
      thickness = 0.01,
    }),
    pitch = math.pi / 3,
  }),
}
