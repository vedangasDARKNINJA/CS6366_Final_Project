# Generate Project Files

As the project uses CMake as the build system, just running the following command works:

```shell
cmake -G "Visual Studio 17 2022" -A "x64" -B "./build"
```

### Windows

You may just want to run the `generateProjectFiles.bat` file.

# Building The Project

-   Once the cmake is finished running, open the `build/RendererPBR.sln`.
-   Build the `ALL-BUILD` project from the `CMakePredefinedConfigs` folder.
-   Now set the startup project to be `RendererPBR`
-   You may run the project from within the Visual Studio.
