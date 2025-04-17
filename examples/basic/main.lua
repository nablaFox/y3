local scene = {
    require('entities.camera')(),

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

return scene
