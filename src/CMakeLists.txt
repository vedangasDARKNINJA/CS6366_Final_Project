function(copy_data_to_executable_dir target)
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/data/"
            "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/data"
    )
endfunction()

add_executable( ${PROJECT_NAME} main.cpp)

list(APPEND INCLUDE_PATHS 
                "${CMAKE_SOURCE_DIR}/include" 
                "${CMAKE_SOURCE_DIR}/vendor/GLFW/include"
                "${CMAKE_SOURCE_DIR}/vendor/glad/include"
                "${CMAKE_SOURCE_DIR}/vendor/glm"
                "${CMAKE_SOURCE_DIR}/vendor/assimp/include"
                "${CMAKE_SOURCE_DIR}/vendor/stb_image/"
                "${CMAKE_SOURCE_DIR}/vendor/imgui"
                "${CMAKE_SOURCE_DIR}/vendor/imgui/backends")

set(PROJECT_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/include")

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_PATHS})
target_sources(
    ${PROJECT_NAME}
        PRIVATE
            # Core API
            ${PROJECT_INCLUDE_PATH}/ApplicationSettings.hpp
            ${PROJECT_INCLUDE_PATH}/RendererPBR.hpp
            ${PROJECT_INCLUDE_PATH}/Renderer.hpp
            ${PROJECT_INCLUDE_PATH}/Subsystem.hpp
            ${PROJECT_INCLUDE_PATH}/Application.hpp
            ${PROJECT_INCLUDE_PATH}/Window.hpp
            ${PROJECT_INCLUDE_PATH}/Keyboard.hpp
            ${PROJECT_INCLUDE_PATH}/Mouse.hpp
            ${PROJECT_INCLUDE_PATH}/ImGuiSubsystem.hpp
            ${PROJECT_INCLUDE_PATH}/Camera.hpp
            Application.cpp
            Window.cpp
            Keyboard.cpp
            Mouse.cpp
            Renderer.cpp
            Camera.cpp
            ImGuiSubsystem.cpp

            # Rendering
            ${PROJECT_INCLUDE_PATH}/Shader.hpp
            Shader.cpp
            ${PROJECT_INCLUDE_PATH}/VertexBufferLayout.hpp
            ${PROJECT_INCLUDE_PATH}/VertexBuffer.hpp
            VertexBuffer.cpp
            ${PROJECT_INCLUDE_PATH}/IndexBuffer.hpp
            IndexBuffer.cpp
            ${PROJECT_INCLUDE_PATH}/VertexArray.hpp
            VertexArray.cpp
            ${PROJECT_INCLUDE_PATH}/Texture.hpp
            Texture.cpp
            ${PROJECT_INCLUDE_PATH}/Framebuffer.hpp
            Framebuffer.cpp
            ${PROJECT_INCLUDE_PATH}/Renderbuffer.hpp
            Renderbuffer.cpp
            ${PROJECT_INCLUDE_PATH}/Transform.hpp
            Transform.cpp
            "${CMAKE_SOURCE_DIR}/vendor/stb_image/stb_image.cpp"

            #Objects
            ${PROJECT_INCLUDE_PATH}/Mesh.hpp
            Mesh.cpp
            ${PROJECT_INCLUDE_PATH}/TestQuad.hpp
            TestQuad.cpp
            ${PROJECT_INCLUDE_PATH}/FBQuad.hpp
            FBQuad.cpp
            ${PROJECT_INCLUDE_PATH}/PrimitiveSphere.hpp
            PrimitiveSphere.cpp
            ${PROJECT_INCLUDE_PATH}/AssimpModel.hpp
            AssimpModel.cpp
            ${PROJECT_INCLUDE_PATH}/Light.hpp
            Light.cpp

)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad glm imgui assimp)

target_link_directories(${PROJECT_NAME} PRIVATE ${LIBRARY_OUTPUT_PATH})

copy_data_to_executable_dir(${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")