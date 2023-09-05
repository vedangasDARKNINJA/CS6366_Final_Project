# Results:

## Blinn Phong Lighting:

Implemented a basic lighting using blinn phong model.
![Blinn Phong](https://github.com/vedangasDARKNINJA/CS6366_Final_Project/tree/master/images/Blinn-Phong.png)

## Camera Settings:

Added an orbit camera with configuration exposed in the application through Dear imgui.
![Camera imgui](https://github.com/vedangasDARKNINJA/CS6366_Final_Project/tree/master/images/CameraImGUI.png)

## Physically Based Rendering(PBR):

![RedCloseUp](https://github.com/vedangasDARKNINJA/CS6366_Final_Project/tree/master/images/PBR_Red_CloseUp.png)

![Red](https://github.com/vedangasDARKNINJA/CS6366_Final_Project/tree/master/images/PBR_Red.png)

![Textured_CloseUp](https://github.com/vedangasDARKNINJA/CS6366_Final_Project/tree/master/images/PBR_Textured_CloseUp.png)

![Textured_Textured](https://github.com/vedangasDARKNINJA/CS6366_Final_Project/tree/master/images/PBR_Textured.png)

# Generate Project Files

As the project uses CMake as the build system, just running the following command works:

```shell
cmake -G "Visual Studio 17 2022" -A "x64" -B "./build"
```

### Windows

You may just want to run the `generateProjectFiles.bat` file.

# Building The Project

- Once the cmake is finished running, open the `build/RendererPBR.sln`.
- Build the `ALL-BUILD` project from the `CMakePredefinedConfigs` folder.
- Now set the startup project to be `RendererPBR`
- You may run the project from within the Visual Studio.
