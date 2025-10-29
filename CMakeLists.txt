cmake_minimum_required(VERSION 3.31)
project(NodesFX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

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

# main executable
add_executable(${PROJECT_NAME} main.cpp
        # generator
        src/generator/INode.h
        src/generator/IDataHandle.h
        src/generator/IHandle.h
        src/generator/OutputHandle.h
        src/generator/InputHandle.h
        src/generator/ColorTraits.h

        # implementations
        src/implementations/NodeRegistry.h
        src/implementations/math/AddNode.cpp
        src/implementations/math/AddNode.h
        src/implementations/math/SplitVectorNode.cpp
        src/implementations/math/SplitVectorNode.h
        src/implementations/effects/BlurNode.cpp
        src/implementations/effects/BlurNode.h
        src/implementations/io/SampleTextureNode.cpp
        src/implementations/io/SampleTextureNode.h
        src/implementations/io/LoadTextureNode.cpp
        src/implementations/io/LoadTextureNode.h
        src/implementations/input/FloatInputNode.cpp
        src/implementations/input/FloatInputNode.h
        src/implementations/math/SumAndDiffNode.cpp
        src/implementations/math/SumAndDiffNode.h

        # math
        src/util/Vec4.h
        src/util/LoadTexture.h
        src/util/TextUtil.h
        src/util/TexData.h

        # designer
        src/designer/nodes/IVisualNode.h
        src/designer/nodes/VisualNode.h
        src/designer/nodes/VisualNode.cpp
        src/designer/nodes/VisualHandle.cpp
        src/designer/nodes/VisualHandle.h
        src/designer/nodes/implementations/VisualFloatInputNode.cpp
        src/designer/nodes/implementations/VisualFloatInputNode.h
        src/designer/ProjectWindow.cpp
        src/designer/ProjectWindow.h
        src/designer/IdManager.h
        src/designer/nodes/LinkInfo.h
        src/designer/App.cpp
        src/designer/App.h
)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 imgui imgui_node_editor OpenGL::GL)

# copy assets
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake
)
add_dependencies(${PROJECT_NAME} copy_assets)