#include "scu_timers_view.hpp"

#include <ymir/hw/scu/scu.hpp>

namespace app::ui {

SCUTimersView::SCUTimersView(SharedContext &context)
    : m_context(context)
    , m_scu(context.saturn.GetSCU()) {}

void SCUTimersView::Display() {
    auto &probe = m_scu.GetProbe();

    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.medium);
    const float hexCharWidth = ImGui::CalcTextSize("F").x;
    ImGui::PopFont();

    bool t1mode = probe.GetTimer1Mode();

    bool enable = probe.IsTimerEnabled();
    if (ImGui::Checkbox("Enabled##timer", &enable)) {
        probe.SetTimerEnabled(enable);
    }

    if (ImGui::BeginTable("timer", 3, ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableNextRow();
        if (ImGui::TableNextColumn()) {
            ImGui::PushFont(m_context.fonts.sansSerif.bold, m_context.fonts.sizes.medium);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Timer 0");
            ImGui::PopFont();
        }
        if (ImGui::TableNextColumn()) {
            uint16 counter = probe.GetTimer0Counter();
            ImGui::BeginGroup();
            ImGui::SetNextItemWidth(ImGui::GetStyle().FramePadding.x * 2 + hexCharWidth * 3);
            ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.medium);
            if (ImGui::InputScalar("##t0cnt", ImGuiDataType_U16, &counter, nullptr, nullptr, "%03X",
                                   ImGuiInputTextFlags_CharsHexadecimal)) {
                probe.SetTimer0Counter(counter);
            }
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Counter");
            ImGui::EndGroup();
        }
        if (ImGui::TableNextColumn()) {
            uint16 compare = probe.GetTimer0Compare();
            ImGui::BeginGroup();
            ImGui::SetNextItemWidth(ImGui::GetStyle().FramePadding.x * 2 + hexCharWidth * 3);
            ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.medium);
            ImGui::InputScalar("##t0cmp", ImGuiDataType_U16, &compare, nullptr, nullptr, "%03X",
                               ImGuiInputTextFlags_CharsHexadecimal);
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Compare");
            ImGui::EndGroup();
        }

        ImGui::TableNextRow();
        if (ImGui::TableNextColumn()) {
            ImGui::PushFont(m_context.fonts.sansSerif.bold, m_context.fonts.sizes.medium);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Timer 1");
            ImGui::PopFont();
        }
        if (ImGui::TableNextColumn()) {
            uint16 reload = probe.GetTimer1Reload();
            ImGui::BeginGroup();
            ImGui::SetNextItemWidth(ImGui::GetStyle().FramePadding.x * 2 + hexCharWidth * 3);
            ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.medium);
            ImGui::InputScalar("##t1rld", ImGuiDataType_U16, &reload, nullptr, nullptr, "%03X",
                               ImGuiInputTextFlags_CharsHexadecimal);
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Reload");
            ImGui::EndGroup();
        }
        if (ImGui::TableNextColumn()) {
            if (ImGui::RadioButton("Every line", !t1mode)) {
                probe.SetTimer1Mode(false);
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Timer 0 match", t1mode)) {
                probe.SetTimer1Mode(true);
            }
        }

        ImGui::EndTable();
    }

    // TODO: draw display diagram showing where Timer 1 will match on the screen
}

} // namespace app::ui
