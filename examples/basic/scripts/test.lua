local function start(node, data, _)
    data.test = "Idk, something"
end

return {
    start = start,
    update = function(node, data, dt, _)
        print(data.test)
    end,
}
