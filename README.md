# FlexiParts

## Build

To build, open CMakelist and point it to your Unity folder (currently C:/Program Files/Unity 5.5.1f).

`mkdir build && cd build && cmake .. -G "Visual Studio 14 2015 Win64"`

Open the generated solution in Visual Studio. When building in Visual Studio, make sure Unity is closed as a post-build process tries to copy the dll and will fail.
Use the FlexiParts.cs script to interface with the library.

## Technical Details

One of the only optimization that is used is SOA for the simulation data (particle transforms).
The system updates position, rotation and scale on the CPU called by Unity's Update loop.
Things are rendered later during the frame. Rendering is quite hacky and needs love (copying uniforms for every particle).

## Todo and future

- Architecture that permits enabling and disabling various optimizations to benchmark easily.
- Add basic features such as life time, continuous spawning, bursts, respawn, multiple systems...
- Investigate CPU simulation vs. SIMD CPU simulation vs. GPU simulation.
- Investigate Unity UI and Post Processes.
- Investigate whether we can render while in edit-mode in Unity.
- OpenGL : Use instancing and buffers to pass data to the GPU, instead of uniforms (really bad).
- Investigate keeping different systems particles together, and use texture atlas for rendering.
- Investigate smooth particles (test Z-buffer, need render target).
- Investigate fill rate optimization, by modeling the particle to hug the image.
- Particle physics!?
- Investigate Texture animations.

## Benchmarking

Unity easily renders 300 000 to 400 000 particles at 30fps. We currently only render 100 000. BUT, we are copying so much data, this is a great sign we can improve a lot!

- Unity Shuriken : 300 000 to 400 000 particles at 30fps.
- Basic CPU simulation copying data in uniforms for every particle : 100 000 at 30fps.