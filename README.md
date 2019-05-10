# Game Engine

**University of Pennsylvania, CIS 497: Senior Design**

![Developer Hannah](https://img.shields.io/badge/Developer-Hannah-0f97ff.svg?style=flat) ![Built](https://img.shields.io/appveyor/ci/gruntjs/grunt.svg)

[//]: #(![gpu.js](https://img.shields.io/badge/GPGPU-gpu.js-yellow.svg))
[//]: #(![WebGL2.0](https://img.shields.io/badge/WebGL-2.0-lightgrey.svg))
[//]: #(![Built](https://img.shields.io/appveyor/ci/gruntjs/grunt.svg))
[//]: #![Progress](https://img.shields.io/badge/implementation-in%20progress-orange.svg)
[//]: #(![Issues](https://img.shields.io/badge/issues-none-green.svg))

____________________________________________________________________________________

### About

This is a small version of a game engine with a short cat game built using it.

- ![Final Project VIDEO](./SeniorDesignGameEngine_smallfile.mp4)

____________________________________________________________________________________

### Other repos I used / built in this development process:

**Last Semester:**
- ![Game-Engine-Base](https://github.com/hanbollar/Game-Engine)
   -  the project I built and refactored over and over to get a rewrite of the shaderprogram setup.

**This Semester:**
- ![Research](https://github.com/hanbollar/some_useless_gameengineinfo)
   - a research repo I used for storing random game engine info and links that were important or helpful so that I didnt need to continue maintaining a lot of browsers tabs open, and it has some of my help log commentary and notes to help myself keep track of why I used certain implementations over others.
- ![PreBase](https://github.com/hanbollar/engine1)
   - a game engine implementation that i started with this semester that became too convoluted for me to refactor. ended up copying over some of these aspects over to this readme's engine repo but most of this ended up getting scrapped
- ![Engine](#)
   - this repo - the actual engine implementation with a game built off it 

**Resources Thy Built:**
- ![Cat](https://github.com/tatran5/sp19IPDCat)
- ![Character](https://github.com/tatran5/sp19IPDMainChar)
- ![Staff](https://github.com/tatran5/sp19Staff)
- ![Building](https://github.com/tatran5/sp19OnsenStreet)

^^ had issues with this due to how the texture was exported so loaded in some of them with / without the texture. The demoed game ultimately only has the Character in it for playing purposes.

**Note:** the exe files dont run for some reason because of a .dll connection issue with irrklang. It has to do with how I set up my project in terms of file locations where some of them should have been in the build folder instead of general resources etc. (which means a download and direct build from github doesnt work at the moment - resolving this when i might work on this project again at a later date)

____________________________________________________________________________________

### Resources

Online Resources:
- ![learnopengl.com](learnopengl.com)
   - used for initial project setup and some tutorials for possible frameworks to use
- ![assimp](http://www.assimp.org/)
   - general object and aspect loading
- ![SOIL](https://www.lonesock.net/soil.html)
   - tried using this originally for texture loading, had linking issues, then realized it doesnt work with the newer version of glfw (it cant find context)
- ![stbimage](https://github.com/nothings/stb)
   - another texture loading software I tried - worked for general aspects but not for every inputted
- ![collada](https://www.khronos.org/collada/)
   - main file setup while i was trying to figure out why the texture part wasn't working. also switched to this to aid project setup while trying to work on rigging.
- ![opengl-tutorial](http://www.opengl-tutorial.org/)
   - extremely helpful for texture and audio loading (especially in terms of possible libraries and how to get text overlay to work properly)
- ![opengl 4.2](https://www.khronos.org/registry/OpenGL/specs/gl/glspec42.core.pdf)
   - opengl version for this project
- ![glfw](https://github.com/glfw/glfw)
   - the window support library that works on mac and windows
- ![irrklang](https://www.ambiera.com/irrklang/downloads.html)
   - used for audio - also followed their tutorial for how to get pausing and playing to work to allow for switching between the 2d and 3d audio without the audio still playing (because that became a major issue)

Books:
- Game Engine Architecture, Jason Gregory, 3ed
- Game Programming Patterns, Robet Nystrom
- The Ultimate Guide to Video Game Writing and Design, Dille & Platten
- Video Game Storytelling, Evan Skolnick

