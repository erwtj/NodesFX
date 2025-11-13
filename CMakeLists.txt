cmake_minimum_required(VERSION 3.31)
project(NodesFX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Cuda
add_definitions(-DCUDA) # TODO This enables cuda in code, but could I not just check if the cuda package was found?
set(CMAKE_CUDA_ARCHITECTURES 75)
include(CheckLanguage)
check_language(CUDA)
enable_language(CUDA)
find_package(CUDAToolkit)

# sdl
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# opengl
find_package(OpenGL REQUIRED)

# imgui
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)
include_directories(vendored/imgui vendored/imgui/backends)
add_library(imgui STATIC
        vendored/imgui/imgui.cpp
        vendored/imgui/imgui_demo.cpp
        vendored/imgui/imgui_draw.cpp
        vendored/imgui/imgui_tables.cpp
        vendored/imgui/imgui_widgets.cpp
        vendored/imgui/backends/imgui_impl_opengl3.cpp
        vendored/imgui/backends/imgui_impl_sdl3.cpp
)
target_link_libraries(imgui PRIVATE SDL3::SDL3 OpenGL::GL)

# imgui node editor
include_directories(vendored/imgui-node-editor)
add_library(imgui_node_editor STATIC
        vendored/imgui-node-editor/imgui_node_editor.cpp
        vendored/imgui-node-editor/imgui_node_editor_api.cpp
        vendored/imgui-node-editor/imgui_canvas.cpp
        vendored/imgui-node-editor/crude_json.cpp
)
target_link_libraries(imgui_node_editor PRIVATE imgui)

# nlohmann_json
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(vendored/json)

# inja
add_library(inja INTERFACE)
target_include_directories(inja INTERFACE vendored/inja)

# imgproc lib
include_directories(imgproc/include)
add_library(imgproc STATIC
        imgproc/include/imgproc.h

        # types
        imgproc/include/Color.h
        imgproc/include/Vec.h
        imgproc/include/TexData.h

        # cpu
        imgproc/cpu/blur_cpu.cpp

        # cuda
        imgproc/cuda/blur_cuda.cu

        # opencl
)
# Add CUDA
target_link_libraries(imgproc CUDA::cudart CUDA::cuda_driver)

# main executable
add_executable(${PROJECT_NAME} main.cpp
        # generator
        src/generator/nodes/INode.h
        src/generator/nodes/IDataHandle.h
        src/generator/nodes/IHandle.h
        src/generator/nodes/OutputHandle.h
        src/generator/nodes/InputHandle.h
        src/generator/nodes/ColorTraits.h

        # implementations
        src/implementations/NodeRegistry.h
        src/implementations/math/AddNode.cpp
        src/implementations/math/AddNode.h
        src/implementations/math/vectors/SplitVectorNode.h
        src/implementations/math/vectors/SplitVectorNode.cpp
        src/implementations/effects/SolidColorNode.cpp
        src/implementations/effects/SolidColorNode.h
        src/implementations/effects/BlurNode.cpp
        src/implementations/effects/BlurNode.h
        src/implementations/io/SampleTextureNode.cpp
        src/implementations/io/SampleTextureNode.h
        src/implementations/io/LoadTextureNode.cpp
        src/implementations/io/LoadTextureNode.h

        # util
        src/util/LoadTexture.h
        imgproc/include/Vec.h
        src/util/TextUtil.h
        imgproc/include/TexData.h
        imgproc/include/Color.h

        # designer
        src/designer/ProjectWindow.cpp
        src/designer/ProjectWindow.h
        src/designer/IdManager.h
        src/designer/nodes/LinkInfo.h
        src/designer/App.cpp
        src/designer/App.h
            # designer/nodes
            src/designer/nodes/IVisualNode.h
            src/designer/nodes/VisualNode.h
            src/designer/nodes/VisualNode.cpp
            src/designer/nodes/VisualHandle.cpp
            src/designer/nodes/VisualHandle.h
                # designer/nodes/input
                src/designer/nodes/input/InputNodeRegistry.h
                src/designer/nodes/input/InputTraits.h
                src/designer/nodes/input/InputNode.h
                src/designer/nodes/input/VisualInputNode.h
                src/designer/nodes/input/VisualInputNode.cpp
        src/implementations/math/vectors/VectorLengthNode.cpp
        src/implementations/math/vectors/VectorLengthNode.h
        src/implementations/math/trigonometry/SinNode.cpp
        src/implementations/math/trigonometry/SinNode.h
        src/implementations/math/trigonometry/CosNode.cpp
        src/implementations/math/trigonometry/CosNode.h
        src/implementations/math/trigonometry/TanNode.cpp
        src/implementations/math/trigonometry/TanNode.h
        src/implementations/math/SubtractNode.cpp
        src/implementations/math/SubtractNode.h
        imgproc/cpu/blur_cpu.cpp
        imgproc/include/imgproc.h
        src/designer/rendering/ITextureHandle.h
        src/designer/rendering/GLTextureHandle.h
)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 imgui imgui_node_editor OpenGL::GL inja imgproc nlohmann_json::nlohmann_json)

# copy assets
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake
)
add_dependencies(${PROJECT_NAME} copy_assets)