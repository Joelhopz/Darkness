#pragma once

#include <vector>
#include <string>
#include <chrono>
#include "external/imgui/imgui.h"

namespace engine
{
    class LogWindow
    {
    public:
        LogWindow();
        void pushMessages(const std::vector<std::string>& messages);
        void render(int windowWidth, int windowHeight);

    private:
        std::vector<std::string> m_messages;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_lastShaderChangeUpdate;
        ImGuiTextBuffer m_buf;
        ImGuiTextFilter m_filter;
        ImVector<int> m_lineOffsets;
    };
}
