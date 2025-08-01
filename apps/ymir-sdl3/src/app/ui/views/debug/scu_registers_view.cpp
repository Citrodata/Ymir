#include "scu_registers_view.hpp"

#include <ymir/hw/scu/scu.hpp>

namespace app::ui {

SCURegistersView::SCURegistersView(SharedContext &context)
    : m_scu(context.saturn.GetSCU()) {}

void SCURegistersView::Display() {
    auto &probe = m_scu.GetProbe();

    ImGui::BeginGroup();

    bool wramSizeSelect = probe.GetWRAMSizeSelect();
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("WRAM size:");
    ImGui::SameLine();
    if (ImGui::RadioButton("512 KiB (2x2 Mbit)", !wramSizeSelect)) {
        probe.SetWRAMSizeSelect(false);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("1 MiB (2x4 Mbit)", wramSizeSelect)) {
        probe.SetWRAMSizeSelect(true);
    }

    ImGui::EndGroup();
}

} // namespace app::ui
