#include "App.h"

#include "Project.h"

App::App() {
    addNewProject();
}

void App::addNewProject() {
    const unsigned int id = idCounter++;
    ProjectInfo info {id, std::to_string(id)};
    projectWindows.emplace_back(std::make_unique<ProjectWindow>(std::move(info)));
}

void App::tick() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 pos = viewport->Pos;
    ImVec2 size = viewport->Size;

    // === MENU BAR ===
    float menuBarHeight = ImGui::GetTextLineHeightWithSpacing();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, menuBarHeight));
    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(ImVec2(size.x, menuBarHeight));

    ImGui::Begin("MenuBar", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {
                addNewProject();
            }
            if (ImGui::MenuItem("Open")) { }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo")) { }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
    ImGui::PopStyleVar();

    // === TAB BAR ===
    float tabBarHeight = 32;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, tabBarHeight));
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(size.x, tabBarHeight));

    ImGui::Begin("TabBar", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove);

    if (ImGui::BeginTabBar("Tabs")) {
        for (const auto& project : projectWindows) {
            const auto &[id, name] = project->info();
            if (ImGui::BeginTabItem(name.c_str())) {
                selectedProject = id;
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
    ImGui::PopStyleVar();

    for (const auto& project : projectWindows) {
        if (project->info().id == selectedProject)
            project->tick(ImVec2(pos.x, pos.y + menuBarHeight + tabBarHeight), ImVec2(size.x, size.y - menuBarHeight - tabBarHeight));
    }
}