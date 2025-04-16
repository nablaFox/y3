function update(node, data, dt, scene)
    data.speed = data.speed + dt * 0.1
end

function start(node, data, scene)
    print("Hello world from", node:get_name())

    print("Initial Data:", data.speed)
end

function sleep(node, data, scene)
    print("Goodbye world from", node:get_name())

    print("Final Data:", data.speed)
end

local camera_script = y3.create_script({
    name = "CameraScript",
    start = start,
    sleep = sleep,
    update = update,
    data = {
        speed = 5,
        sensitivity = 0.001,
    },
})

return y3.create_camera({
    name = "MainCamera",
    scripts = camera_script,
    position = Vec3.new(0, 0, 5),
})
