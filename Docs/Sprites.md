# Sprites Documentation
The Sprite system in Phlox Engine provides functionality for rendering both static and animated sprites.

## Basic Sprites

Creation and Setup:

```lua
local sprite = Sprite.new()
sprite:loadTexture("Assets/Images/player.png")
sprite:setPosition(100, 100)
```

## Methods

```Sprite.new()``` - Creates a new sprite
```loadTexture(path)``` - Loads an image file as the sprite's texture
```setPosition(x, y)``` - Sets the position of the sprite
```setScale(scaleX, scaleY)``` - Sets the scale of the sprite
```setVisible(boolean)``` - Shows or hides the sprite
```getWidth()``` - Returns the sprite's width
```getHeight()``` - Returns the sprite's height
```isVisible()``` - Returns whether the sprite is visible

# Animated Sprites (Sparrow V2 Spritesheet and XML)

### Basic Sprite

Creation and Setup:

```lua
local animSprite = AnimatedSprite.new()
animSprite:loadFrames("Assets/Images/character.png", "Assets/Images/character.xml")
animSprite:setPosition(100, 100)
animSprite:addAnimation("idle", "idleLoop", 24, true) // name, animation name, frames, loop
animSprite:playAnimation("idle")
```

## Methods
All basic Sprite methods, plus:
```loadFrames(imagePath, xmlPath)``` - Loads sprite sheet and XML animation data
```play(animationName)``` - Plays the specified animation
```stop()``` - Stops the current animation
```pause()``` - Pauses the current animation
```resume()``` - Resumes a paused animation
```setAnimationSpeed(speed)``` - Sets the playback speed multiplier

## XML Format (for reference)

```xml
<?xml version="1.0" encoding="utf-8"?>
<TextureAtlas imagePath="character.png">
    <SubTexture name="idle0" x="0" y="0" width="100" height="100"/>
    <SubTexture name="idle1" x="100" y="0" width="100" height="100"/>
</TextureAtlas>
```
