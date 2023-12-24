#include <imgui/imgui.h>
#include "imgui/imgui.h"
#include "../vendor/imgui/imgui.h"
#include "Test.h"

namespace test {
    TestMenu::TestMenu(Test*& CurrentTestPointer) 
        : m_CurrentTest(CurrentTestPointer)
    {

    }

    void TestMenu::OnImGuiRender() {
        for (auto& test : m_Tests) {
            if (ImGui::Button(test.first.c_str())) 
                m_CurrentTest = test.second();
        }
    }
}