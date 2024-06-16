#include "UI.h"

void UI::Register() {

    if (!SKSEMenuFramework::IsInstalled()) {
        return;
    }
    Configuration::Example2::Buffer[0] = '\0';
    SKSEMenuFramework::SetSection("SKSE Menu Framework Compiled Example 4");
    SKSEMenuFramework::AddSectionItem("Font Awesome", Example4::Render);
    SKSEMenuFramework::AddSectionItem("Add Item", Example1::Render);
    SKSEMenuFramework::AddSectionItem("Folder Example/Example 1", Example2::Render);
    UI::Example2::ExampleWindow = SKSEMenuFramework::AddWindow(Example2::RenderWindow);
    SKSEMenuFramework::AddSectionItem("Folder Example/Example 2", Example3::Render);
}

void UI::Example1::LookupForm() {
    auto addForm = RE::TESForm::LookupByID(AddFormId);
    if (addForm) {
        AddBoundObject = addForm->As<RE::TESBoundObject>();
    } else {
        AddBoundObject = nullptr;
    }
}

void __stdcall UI::Example1::Render() {
    ImGui::InputScalar("form id", ImGuiDataType_U32, &AddFormId, NULL, NULL, "%08X");

    if (ImGui::Button("Search")) {
        LookupForm();
    }

    if (AddBoundObject) {
        ImGui::Text("How much %s would you like to add?", AddBoundObject->GetName());
        ImGui::SliderInt("number", &Configuration::Example1::Number, 1, 100);
        if (ImGui::Button("Add")) {
            auto player = RE::PlayerCharacter::GetSingleton()->As<RE::TESObjectREFR>();
            player->AddObjectToContainer(AddBoundObject, nullptr, Configuration::Example1::Number, nullptr);
        }
    } else {
        ImGui::Text("Form not found");
    }
}

void __stdcall UI::Example2::Render() {
    if (ImGui::Button("Open Window")) {
        ExampleWindow->IsOpen = true;
    }

    ImGui::InputText("string", Configuration::Example2::Buffer, 256);
    ImGui::ColorEdit4("Color", &Configuration::Example2::Color);
    float samples[100];
    for (int n = 0; n < 100; n++) samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
    ImGui::PlotLines("Samples", samples, 100);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
    ImGui::BeginChild("Scrolling");
    for (int n = 0; n < 50; n++) ImGui::Text("%04d: Some text", n);
    ImGui::EndChild();
}

void __stdcall UI::Example2::RenderWindow() {
    auto viewport = ImGui::GetMainViewport();

    auto center = ImGui::ImVec2Manager::Create();
    ImGui::ImGuiViewportManager::GetCenter(center, viewport);
    ImGui::SetNextWindowPos(*center, ImGuiCond_Appearing, ImVec2{0.5f, 0.5f});
    ImGui::ImVec2Manager::Destroy(center);
    ImGui::SetNextWindowSize(ImVec2{viewport->Size.x * 0.4f, viewport->Size.y * 0.4f}, ImGuiCond_Appearing);
    ImGui::Begin("My First Tool##MenuEntiryFromMod",nullptr, ImGuiWindowFlags_MenuBar); // If two mods have the same window name, and they open at the same time.
                                                                                         // The window content will be merged, is good practice to add ##ModName after the window name.
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */
            }
            if (ImGui::MenuItem("Close", "Ctrl+W")) {
                ExampleWindow->IsOpen = false;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    if (ImGui::Button("Close Window")) {
        ExampleWindow->IsOpen = false;
    }
    ImGui::End();
}

void __stdcall UI::Example3::Render() {
    static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |
                                   ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                   ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY;

    filter->Draw();

    if (ImGui::BeginTable("table1", 3, flags)) {
        ImGui::TableSetupColumn("One");
        ImGui::TableSetupColumn("Two");
        ImGui::TableSetupColumn("Three");
        ImGui::TableHeadersRow();

        for (int row = 0; row < 5; row++) {
            ImGui::TableNextRow();
            for (int column = 0; column < 3; column++) {
                ImGui::TableSetColumnIndex(column);
                char buf[32];
                sprintf(buf, "Hello %d,%d", column, row);

                if (filter->PassFilter(buf)) {
                    FontAwesome::PushSolid();
                    ImGui::Text(CheckMark.c_str());
                    FontAwesome::Pop();
                    ImGui::SameLine();
                }

                if (column == 2) {
                    ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                } else {
                    ImGui::TextUnformatted(buf);
                }
            }
        }
        ImGui::EndTable();
    }
}

void __stdcall UI::Example4::Render() {
    FontAwesome::PushBrands();
    ImGui::Text(TitleText.c_str());
    FontAwesome::Pop();

    FontAwesome::PushSolid();
    ImGui::Button(Button1Text.c_str());
    FontAwesome::Pop();

    ImGui::SameLine();

    FontAwesome::PushRegular();
    ImGui::Button(Button2Text.c_str());
    FontAwesome::Pop();
}