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
        src/generator/nodes/Node.h
        src/generator/nodes/IDataHandle.h
        src/generator/nodes/implementations/AddNode.cpp
        src/generator/nodes/implementations/AddNode.h
        src/generator/nodes/IHandle.h
        src/generator/nodes/OutputHandle.h
        src/generator/nodes/InputHandle.h
        src/designer/nodes/Categories.h
        src/designer/nodes/VisualNode.h
        src/designer/nodes/VisualNode.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 imgui imgui_node_editor OpenGL::GL)

# copy assets
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake
)
add_dependencies(${PROJECT_NAME} copy_assets)