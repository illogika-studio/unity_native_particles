# Unity Native Particles

## Goal

Investigate and benchmark various optimizations to create a particle system tuned for millions of simple particles. Starting target is 4 million particles at 60fps.

## Build

To build, open CMakelist.txt and point it to your Unity folder (currently C:/Program Files/Unity 5.5.1f).

`mkdir build && cd build && cmake .. -G "Visual Studio 14 2015 Win64"`

Open the generated solution in Visual Studio. When building in Visual Studio, make sure Unity is closed as a post-build process tries to copy the dll and Visual Studio will fail.
Use the UnityNativeParticles.cs script to interface with the library.

## Debugging

Crucial, **crucial** to read this: https://msdn.microsoft.com/en-us/library/605a12zt.aspx

Project Properties > Debugging.
In Command, paste your Unity path : C:\Program Files\Unity 5.5.2f1\Editor\Unity.exe
In Command Arguments, paste the projectPath command with the unity project path : -projectPath D:\illogika\unity_native_particles\unity_project

**This has the added benefit of launching automatically Unity and the project when pressing "play" in Visual Studio.**

## Technical Details

~~One of the only optimization that is used is SOA for the simulation data (particle transforms).
The system updates position, rotation and scale on the CPU called by Unity's Update loop.
Things are rendered later during the frame. Rendering is quite hacky and needs love (copying uniforms for every particle).~~

Love has been given, the code appreciated it and rewarded us with more particles.

## Todo and Future

- Architecture that allows enabling and disabling various optimizations to benchmark easily.
- Add basic features such as life time, continuous spawning, bursts, respawn, multiple systems, world spawning...
- Investigate CPU simulation vs. SIMD CPU simulation vs. GPU simulation (not transform feedback/stream output, compute shader instead).
- Investigate Unity UI and Post Processes.
- Investigate whether we can render while in edit-mode in Unity.
- ~~OpenGL : Use instancing and buffers to pass data to the GPU, instead of uniforms (really bad).~~ DONE x10 particles.
- Investigate keeping different particle systems contiguous in memory, and use texture atlas for rendering.
- Investigate smooth particles (test Z-buffer, need render target).
- Investigate fill rate optimization, by modeling the particle's mesh to hug the sprite.
- Investigate Texture animations.
- Investigate AMD async extension.
- Investigate stream compression (example in microsoft miniengine).
- Investigate round-robin buffer uploads.
- Particle physics!?

## Benchmarking

- Unity Shuriken : 300 000 to 400 000 particles at 30fps.
- Basic CPU + copying data in uniforms for every particle : 100 000 at 30fps.
- Basic CPU + Don't use vertexAttribPointer for each particle : 200 000 at 30fps.
- Basic CPU + Instancing : 2 000 000 at 30fps.
