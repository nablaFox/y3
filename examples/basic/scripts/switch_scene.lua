return {
    update = function()
        if (y3.key_clicked(KEY_1)) then
            y3.switch_scene("other")
        end

        if (y3.key_clicked(KEY_2)) then
            y3.switch_scene("main")
        end
    end
}
