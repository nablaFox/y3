function update(node, data, dt, scene)
    local transform = node:get_transform()

    transform.yaw = transform.yaw + y3.mouse_dx() * data.sensitivity
    transform.pitch = transform.pitch + y3.mouse_dy() * data.sensitivity

    if transform.pitch > math.pi / 2 then
        transform.pitch = math.pi / 2
    elseif transform.pitch < -math.pi / 2 then
        transform.pitch = -math.pi / 2
    end

    local position = transform.position
    local yaw = -transform.yaw

    local playerForward = data.fly
        and transform:forward()
        or Vec3.new(-math.sin(yaw), 0, -math.cos(yaw))

    local playerRight = data.fly
        and transform:right()
        or Vec3.new(math.cos(yaw), 0, -math.sin(yaw))

    local up = data.fly and transform:up() or Vec3.new(0, 0, 0)

    if y3.is_key_down(KEY_0) then
        position = Vec3.new(0, 1, 0)
    end

    if y3.is_key_down(KEY_W) then
        position = position + playerForward * data.speed * dt
    end

    if y3.is_key_down(KEY_S) then
        position = position - playerForward * data.speed * dt
    end

    if y3.is_key_down(KEY_D) then
        position = position + playerRight * data.speed * dt
    end

    if y3.is_key_down(KEY_A) then
        position = position - playerRight * data.speed * dt
    end

    if y3.is_key_down(KEY_SPACE) then
        position = position + up * data.speed * dt
    end

    if y3.is_key_down(KEY_LSHIFT) then
        position = position - up * data.speed * dt
    end


    transform.position = position
    node:update_transform(transform)
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
        fly = true,
    },
})

return y3.create_camera({
    name = "MainCamera",
    scripts = camera_script,
    position = Vec3.new(0, 0, 5),
})
