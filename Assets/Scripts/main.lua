-- Main game script
local player = AnimatedSprite.new()
player:loadFrames("Assets/Images/BOYFRIEND.png", "Assets/Images/BOYFRIEND.xml")

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
end

-- Call init when script loads
init()