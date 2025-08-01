#include "scu_dsp_data_ram_view.hpp"

#include <ymir/hw/scu/scu.hpp>

namespace app::ui {

SCUDSPDataRAMView::SCUDSPDataRAMView(SharedContext &context)
    : m_context(context)
    , m_scu(context.saturn.GetSCU()) {}

void SCUDSPDataRAMView::Display() {
    const float paddingWidth = ImGui::GetStyle().FramePadding.x;
    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.small);
    const float hexCharWidth = ImGui::CalcTextSize("F").x;
    ImGui::PopFont();

    auto &dsp = m_scu.GetDSP();

    ImGui::BeginGroup();

    if (ImGui::BeginTable("dsp_data_ram", 5, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, hexCharWidth * 2);
        ImGui::TableSetupColumn("Bank 0", ImGuiTableColumnFlags_WidthFixed, paddingWidth * 2 + hexCharWidth * 8);
        ImGui::TableSetupColumn("Bank 1", ImGuiTableColumnFlags_WidthFixed, paddingWidth * 2 + hexCharWidth * 8);
        ImGui::TableSetupColumn("Bank 2", ImGuiTableColumnFlags_WidthFixed, paddingWidth * 2 + hexCharWidth * 8);
        ImGui::TableSetupColumn("Bank 3", ImGuiTableColumnFlags_WidthFixed, paddingWidth * 2 + hexCharWidth * 8);
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableHeadersRow();

        for (uint32 i = 0; i < 64; i++) {
            ImGui::TableNextRow();
            if (ImGui::TableNextColumn()) {
                ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.small);
                ImGui::AlignTextToFramePadding();
                ImGui::Text("%02X", i);
                ImGui::PopFont();
            }
            for (uint32 bank = 0; bank < 4; bank++) {
                if (ImGui::TableNextColumn()) {
                    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.small);
                    ImGui::SetNextItemWidth(paddingWidth * 2 + hexCharWidth * 8);
                    ImGui::InputScalar(fmt::format("##data_{}_{}", bank, i).c_str(), ImGuiDataType_U32,
                                       &dsp.dataRAM[bank][i], nullptr, nullptr, "%08X",
                                       ImGuiInputTextFlags_CharsHexadecimal);
                    ImGui::PopFont();
                }
            }
        }

        ImGui::EndTable();
    }

    ImGui::EndGroup();
}

} // namespace app::ui
