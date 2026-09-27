# Sparky

This was my first substantial end-to-end programming project, built in summer 2023 after my IGCSEs. I wrote the game engine code entirely by hand (without AI because AI didn't existed then, shocker, I know) as I worked through The Cherno's Sparky series.

I did not use Git during the original development, which I uploaded this
 codebase afterwards, so, unfortunately the commit history does not show the full development process.

[![Watch the Sparky demonstration][thumbnail]][demo]

[Watch the engine demonstration on YouTube][demo]

*The video shows simple game built with the game engine, where a face-emoji 2D sprite is controllable using arrow-keys, and the mouse cursor is the light source.*

## Overview

Sparky is a 2D game engine written in C++ and OpenGL. The project covers the systems that connect an application to its graphics and audio output.

The work extends from custom-written individual vector operations to a reusable application interface. The example game demonstrated in recorded video uses that interface to control sprites and display a frame-rate label.

The gane engine includes:

- **Application framework.** Separate methods for initialisation,
  game updates, frame rendering, and periodic tasks. The loop also
  records frame and update rates.
- **Window and input system.** A GLFW wrapper with resize callbacks,
  keyboard states, mouse button states, and cursor coordinates.
- **Two rendering approaches.** A simple renderer draws individual
  sprites. A batch renderer combines sprite data in shared GPU buffers
  and manages texture slots across draw calls.
- **Shader and texture support.** Image loading, texture uploads, and
  GLSL shaders for textured sprites with mouse-controlled lighting.
- **Layers and object groups.** Render layers combine objects with
  shaders and projection matrices. Groups use a transform stack to
  apply shared and nested transforms.
- **Vector and matrix library.** Vector arithmetic, matrix products,
  and matrix inversion. It also provides transform and projection
  matrices for use throughout the graphics code.
- **Font system.** Font management and glyph atlases through FreeType
  and freetype-gl. Text labels use the same batch renderer as sprites.
- **Audio system.** Named sounds and playback controls through Gorilla
  Audio, including pause, resume, loop, and gain controls.
- **Browser build work.** An Emscripten build script, browser-specific
  code paths, and separate OpenGL ES shader assets.

The game engine design follows [The Cherno's Sparky project][upstream].
This repository records my implementation and study of that design,
alongside the third-party libraries it uses.

## Engine systems

### Application loop

The [Sparky application class][application] defines the engine lifecycle.
A game supplies its own `init()`, `update()`, `render()`, and `tick()` methods.

The update timer targets 60 updates per second.
The engine calls `render()` on each loop iteration.
A separate tick records the frame and update counts once per second.

The [example game][example] shows how these methods fit together.

### Window and input

The [Window class][window] wraps GLFW and creates the OpenGL context.
It processes window events and updates the viewport after a resize.

The input interface provides:

- Key states and new key presses.
- Mouse button states and new clicks.
- Cursor coordinates.

The example uses the arrow keys to move a sprite.
It uses the mouse position to control the light position in the shader.

### Sprites, textures, and shaders

The [batch renderer][batch] writes sprite vertices into a shared buffer.
An index buffer defines two triangles for each sprite.
Each batch uses one indexed draw call.

Sprites carry texture coordinates and a texture slot identifier.
The renderer starts a new batch when it reaches eight texture slots.

The [GLSL shaders][shaders] apply projection and model transforms.
The fragment shader combines the sprite colour with its texture.
It then applies a light intensity based on distance from the light.

The project also retains a [simple renderer][simple-renderer].
It issues a separate draw call for each submitted sprite.

### Layers and groups

A [Layer][layers] contains renderable objects and a renderer.
It also holds the shader and projection matrix for those objects.

A `Group` gives several objects a common transform.
The renderer combines this transform with its current transform stack.
Nested groups can therefore compose their transforms.

### Vector and matrix library

The [maths library][maths] defines `vec2`, `vec3`, `vec4`, and `mat4`.

It contains:

- Vector arithmetic and matrix multiplication.
- Translation, rotation, and scale matrices.
- Orthographic and perspective projection matrices.
- Matrix inversion.

These types support sprite positions and the renderer's transform stack.

### Fonts and audio

The [font system][font] uses FreeType through freetype-gl.
It stores glyphs in a texture atlas.
The batch renderer draws text labels with textured quads.
The example uses a label to display its frame rate.

The [audio system][audio] wraps Gorilla Audio.
It manages sounds by name and provides playback controls.
These include pause and resume, plus loop and gain controls.

## Explore the code

Start with [`examples/game.cpp`][example].
It creates a window and adds a textured sprite to a render layer.
It also adds a text label and defines the input controls.

Follow its calls into [`src/sparky.h`][application] to inspect the loop.
Then inspect the [batch renderer][batch] and [transform stack][renderer].

The older [`main.cpp` example][older-example] retains additional experiments.
Its `#if 0` block disables that entry point.
The active example entry point is in `examples/game.cpp`.

## Repository structure

```text
Sparky.sln                   Visual Studio solution.
Sparky-core/
├── examples/game.cpp        Example application and entry point.
├── src/
│   ├── sparky.h             Application lifecycle and main loop.
│   ├── graphics/            Renderers, layers, fonts, and window input.
│   ├── maths/               Vector and matrix library.
│   ├── audio/               Sound controls and sound manager.
│   ├── shaders/             Native GLSL shaders.
│   └── utils/               File, image, string, and timer helpers.
├── ext/                     Third-party source libraries.
├── textures/                Example images.
├── arial.ttf                Example font.
└── dogBark.wav              Example sound.
Dependencies/                Supporting libraries.
libogg/                      Ogg library project.
libvorbis/                   Vorbis library project.
bin/                         Saved builds and browser build resources.
```

## Build configuration

The native solution uses Visual Studio 2022 with the MSVC `v143` toolset.
Its project files target Windows SDK `10.0`.

The projects retain absolute paths from my original Windows machine.
Update their include and library directories before you build.

The native dependencies include:

| Library | Purpose |
| --- | --- |
| GLFW and GLEW | Window input, OpenGL context, and OpenGL function access |
| FreeImage | Image files and texture data |
| FreeType and freetype-gl | Fonts and glyph atlases |
| Gorilla Audio and OpenAL | Audio management and output |
| libogg and libvorbis | Ogg and Vorbis support |

To prepare the saved Windows solution:

1. Open `Sparky.sln` in Visual Studio.
2. Select the `Debug` configuration and `x64` platform.
3. Set `Sparky-core` as the startup project.
4. Update dependency paths in all three projects.
5. Build `libogg` and `libvorbis`, then build `Sparky-core`.
6. Set the debugger's working directory to the `Sparky-core` folder.
7. Make the required runtime DLLs available to the executable.

The working directory lets the example find its shaders and assets.
A clean build on a fresh Windows installation has not been verified.

### Browser build files

The repository also contains an [Emscripten build script][web-script].
It selects the browser code through `SPARKY_EMSCRIPTEN`.

The saved script produces HTML and JavaScript, with WebAssembly disabled.
[Browser assets][web-assets] include separate OpenGL ES shaders.
The script retains its original library inputs and toolchain options.

## Credits and licence

I followed The Cherno's Sparky series to learn C++ game engine development.
[The original Sparky repository][upstream] provides the source project.

This repository includes an [MIT licence](LICENSE).
The third-party libraries retain their own licence notices.

[demo]: https://youtube.com/shorts/ccZTDrBazjE
[thumbnail]: https://img.youtube.com/vi/ccZTDrBazjE/hqdefault.jpg
[upstream]: https://github.com/TheCherno/Sparky
[application]: Sparky-core/src/sparky.h
[example]: Sparky-core/examples/game.cpp
[window]: Sparky-core/src/graphics/window.cpp
[batch]: Sparky-core/src/graphics/batch2drenderer.cpp
[simple-renderer]: Sparky-core/src/graphics/simple2drenderer.cpp
[shaders]: Sparky-core/src/shaders/
[layers]: Sparky-core/src/graphics/layers/
[maths]: Sparky-core/src/maths/
[font]: Sparky-core/src/graphics/font.cpp
[audio]: Sparky-core/src/audio/
[renderer]: Sparky-core/src/graphics/renderer2d.h
[older-example]: Sparky-core/main.cpp
[web-script]: bin/em_build_sparky.bat
[web-assets]: bin/res/