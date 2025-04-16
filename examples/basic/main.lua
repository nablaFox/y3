local scene = {
    y3.create_camera({
        name = "MainCamera",
        position = Vec3.new(0, 0, 5),
        scripts = {
            y3.script("camera", {
                speed = 5,
                fly = true,
                sensitivity = 0.001,
            }),
            y3.script("test", {}),
        },
    }),

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
