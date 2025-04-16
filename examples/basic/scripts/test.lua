local function start(node, _, _)
    print("Hello world from", node:get_name())

    local data = node:get_script_data("camera")

    data.speed = 1.0
    data.fly = false

    print("Initial Data:", data.speed)
end

return {
    start = start,
}
