return function(params)
    local info = params or {
        color = PURPLE:set_alpha(0.3),
        gridSize = 1,
        lineThickness = 0.02,
        height = 0,
        floorScale = 1e3,
    }

    local mesh = y3.get_quad()

    local main_grid_material = y3.create_grid_material_transparent({
        color = INVISIBLE,
        gridColor = info.color,
        gridSpacing = info.gridSize / info.floorScale,
        thickness = info.lineThickness / info.floorScale,
    })

    local sub_grid_material = y3.create_grid_material_transparent({
        color = INVISIBLE,
        gridColor = info.color * 0.8,
        gridSpacing = info.gridSize / (info.floorScale * 2),
        thickness = info.lineThickness / info.floorScale
    })

    local transform = Transform.new({
        position = Vec3.new(0, info.height, 0),
        scale = Vec3.new(info.floorScale),
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
