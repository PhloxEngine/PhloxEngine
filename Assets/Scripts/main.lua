-- Main game script
local player = AnimatedSprite.new()
player:loadFrames("Assets/Images/BOYFRIEND.png", "Assets/Images/BOYFRIEND.xml")

local text = Text.new()
text:setPosition(100, 100)
text:setWidth(500)
text:setFormat("Assets/Fonts/vcr.ttf", 32, 1, 1, 1, Text.ALIGN_CENTER) -- font, size, r, g, b, align
text:setBorderStyle(Text.BORDER_OUTLINE, 1, 0, 0, 2) 
text:setText("Gay by Kawai Sprite")

function init()
    print("Game initialized!")
    player:setPosition(400, 300)
    
    player:addAnimation("idle", "BF idle dance", 24, true)
    player:playAnimation("idle")
end

function update(deltaTime)
    player:update(deltaTime)
end

function render()
    player:render()
    text:render()
end

-- Call init when script loads
init()