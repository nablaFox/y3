local function update(node, data, dt, _)
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

local function start(node, data, _)
  data.fly = true
  data.speed = 10

  print("Hello world from", node:get_name())
end

local function sleep(node, data, _)
  print("Goodbye world from", node:get_name())

  print("Final Data:", data.speed)
end

local camera_script = y3.create_script({
  name = "camera_script",
  update = update,
  start = start,
  sleep = sleep,
  data = {
    speed = 10,
    fly = true,
    sensitivity = 0.001,
  },
})

return y3.create_camera({
  name = "default_camera",
  position = Vec3.new(0, 0, 5),
  scripts = camera_script,
})
