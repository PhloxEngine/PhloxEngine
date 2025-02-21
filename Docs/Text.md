# Text Documentation

The Text class provides functionality for rendering text with various styles and effects in the Phlox Engine.

## Basic Usage

```lua
local text = Text.new()
text:setPosition(100, 100)
text:setText("Hello World!")
text:setFormat("Assets/Fonts/arial.ttf", 32, 1, 1, 1, Text.ALIGN_CENTER)
```

## Methods

### Creation and Setup
- `Text.new()` - Creates a new text object
- `setPosition(x, y)` - Sets the position of the text
- `setText(string)` - Sets the text content
- `setWidth(width)` - Sets the width constraint for text alignment
- `setScale(scale)` - Sets the scale of the text

### Formatting
- `setFormat(font, size, r, g, b, alignment)` - Sets the main text properties
  - `font`: Path to the font file (.ttf)
  - `size`: Font size in pixels
  - `r, g, b`: Color values (0-1)
  - `alignment`: Text alignment (optional, defaults to LEFT)

### Styling
- `setColor(r, g, b, [a])` - Sets the text color (alpha is optional)
- `setBorderStyle(style, r, g, b, [size])` - Sets the border/shadow style
  - `style`: Border style (see constants below)
  - `r, g, b`: Border color values (0-1)
  - `size`: Border thickness (optional, defaults to 1)

### Constants

#### Text Alignment
- `Text.ALIGN_LEFT`
- `Text.ALIGN_CENTER`
- `Text.ALIGN_RIGHT`

#### Border Styles
- `Text.BORDER_NONE`
- `Text.BORDER_OUTLINE`
- `Text.BORDER_SHADOW`

## Examples

### Centered Text with Border

```lua
local text = Text.new()
text:setPosition(400, 300)
text:setWidth(800)
text:setText("Welcome to Phlox Engine!")
text:setFormat("Assets/Fonts/arial.ttf", 48, 1, 1, 1, Text.ALIGN_CENTER)
text:setBorderStyle(Text.BORDER_OUTLINE, 1, 0, 0, 2) -- Red outline, 2px thick
```