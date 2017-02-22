# FlexiParts

## Goal

Investigate and benchmark various optimizations to create a particle system tuned for millions of simple particles. Starting target is 4 million particles at 60fps.

## Build

To build, open CMakelist and point it to your Unity folder (currently C:/Program Files/Unity 5.5.1f).

`mkdir build && cd build && cmake .. -G "Visual Studio 14 2015 Win64"`

Open the generated solution in Visual Studio. When building in Visual Studio, make sure Unity is closed as a post-build process tries to copy the dll and Visual Studio will fail.
Use the FlexiParts.cs script to interface with the library.

## Technical Details

One of the only optimization that is used is SOA for the simulation data (particle transforms).
The system updates position, rotation and scale on the CPU called by Unity's Update loop.
Things are rendered later during the frame. Rendering is quite hacky and needs love (copying uniforms for every particle).

## Todo and Future

- Architecture that allows enabling and disabling various optimizations to benchmark easily.
- Add basic features such as life time, continuous spawning, bursts, respawn, multiple systems, world spawning...
- Investigate CPU simulation vs. SIMD CPU simulation vs. GPU simulation (transform feedback).
- Investigate Unity UI and Post Processes.
- Investigate whether we can render while in edit-mode in Unity.
- OpenGL : Use instancing and buffers to pass data to the GPU, instead of uniforms (really bad).
- Investigate keeping different particle systems contiguous in memory, and use texture atlas for rendering.
- Investigate smooth particles (test Z-buffer, need render target).
- Investigate fill rate optimization, by modeling the particle's mesh to hug the sprite.
- Investigate Texture animations.
- Investigate AMD async extension.
- Investigate stream compression (example in microsoft miniengine).
- Investigate round-robin buffer uploads.
- Particle physics!?

## Benchmarking

Unity easily renders 300 000 to 400 000 particles at 30fps. We currently only render 100 000. BUT, we are copying so much data, this is a great sign we can improve a lot!

- Unity Shuriken : 300 000 to 400 000 particles at 30fps.
- Basic CPU simulation copying data in uniforms for every particle : 100 000 at 30fps.
- Don't use vertexAttribPointer for each particle : 200 000 at 30fps.

## Original Quote - Système de particules flexible et personnalisable

### Objectifs techniques
Créer un système de particules flexible et personnalisable. Les systèmes de particules (système qui permet de gérer plusieurs milliers d’objets à la fois, par exemple des étincelles) existants ne permettent pas de modifier ou de changer les propriétés des particules. Par exemple, on ne peut pas modifier individuellement des propriétés autres que la couleur sur chaque particule. Il est donc impossible d'ajouter des effets comme la distorsion ou du noise sur chacune d'entre elles, en gardant la couleur intacte.

### Incertitudes/défis techniques
Le système de particules de Unity est très limité quant à sa capacité à faire le rendu d'un très grand nombre de particules. Il est aussi très difficile à modifier puisque la plupart des propriétés ne sont pas accessibles. Nous devons implémenter nous­mêmes un système qui s'intègre parfaitement à Unity, qui peut faire le rendu de plusieurs millions de particules et qui est facile à modifier. De plus, nous voulons créer un shader de particules générique permettant de faire des effets communs tels que de la distorsion, des fonctions de bruit aléatoire et des options de luminosité.

### Solutions envisagées
La solution serait de créer une librairie dynamique en C++ afin de profiter des fonctions graphiques de bas niveau et des instructions SSE2. Ceci nous permettra d'avoir un système de particules très performant. Le système aurait la capacité de balancer entre le CPU et le GPU dépendamment de la densité de particules. Un système de callback serait implémenté pour effectuer des actions lorsqu'une collision est détectée entre une particule et la scène. Le shader de particules serait en mode de transparence premultiplied alpha réglable, ce qui nous permettrait de l'ajuster entre l'alpha blending et l'additive blending. La distorsion serait appliquée en perturbant les UVs de la texture principale avec une normal map. Finalement, le bruit de perlin pourra être utilisé comme masque alpha ou comme depth test en profondeur, ce qui donnerait l'impression que les particules sont émises dans un espace 3D.