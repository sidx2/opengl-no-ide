#include "TestClearColor.h"
#include <imgui/imgui.h>

namespace test {
    TestClearColor::TestClearColor()
        : m_ClearColor { 0.2, 0.3, 0.8, 1.0 }
    {
    }

    TestClearColor::~TestClearColor() {
    }

    void TestClearColor::OnUpdate(float deltaTime) {
    }

    void TestClearColor::OnRender() {
        GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestClearColor::OnImGuiRender() {
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
    }
}
