#include "scu_dma_state_view.hpp"

#include <ymir/hw/scu/scu.hpp>

using namespace ymir;

namespace app::ui {

SCUDMAStateView::SCUDMAStateView(SharedContext &context)
    : m_context(context)
    , m_scu(context.saturn.GetSCU()) {}

void SCUDMAStateView::Display(uint8 channel) {
    if (channel >= 3) {
        return;
    }

    auto &probe = m_scu.GetProbe();

    ImGui::BeginGroup();

    const bool enabled = probe.IsDMAEnabled(channel);
    const bool active = probe.IsDMATransferActive(channel);
    const bool indirect = probe.IsDMAIndirect(channel);

    if (!enabled) {
        ImGui::BeginDisabled();
    }
    if (enabled && active) {
        if (indirect) {
            const uint32 currIndirectAddr = probe.GetCurrentDMAIndirectSourceAddress(channel);
            ImGui::Text("Indirect transfer from ");
            ImGui::SameLine();
            ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
            ImGui::Text("%07X", currIndirectAddr);
            ImGui::PopFont();
        } else {
            ImGui::Text("Direct transfer in progress");
        }
    } else if (enabled) {
        ImGui::TextUnformatted("Idle");
    } else {
        ImGui::TextUnformatted("Disabled");
    }

    if (!active) {
        ImGui::BeginDisabled();
    }

    const uint32 currSrcAddr = probe.GetCurrentDMASourceAddress(channel);
    const uint32 currSrcAddrInc = probe.GetCurrentDMASourceAddressIncrement(channel);
    const uint32 currDstAddr = probe.GetCurrentDMADestinationAddress(channel);
    const uint32 currDstAddrInc = probe.GetCurrentDMADestinationAddressIncrement(channel);
    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
    ImGui::Text("%07X", currSrcAddr);
    ImGui::PopFont();
    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.small);
    ImGui::SameLine();
    if (currSrcAddrInc > 0) {
        ImGui::TextDisabled("+%-2X", currSrcAddrInc);
    } else {
        ImGui::TextDisabled("   ");
    }
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::TextUnformatted("->");
    ImGui::SameLine();
    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
    ImGui::Text("%07X", currDstAddr);
    ImGui::PopFont();
    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.small);
    ImGui::SameLine();
    if (currDstAddrInc > 0) {
        ImGui::TextDisabled("+%-2X", currDstAddrInc);
    } else {
        ImGui::TextDisabled("   ");
    }
    ImGui::PopFont();

    const uint32 currXferCount = probe.GetCurrentDMATransferCount(channel);
    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
    ImGui::Text("%X", currXferCount);
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::TextUnformatted("bytes remaining");

    if (!active) {
        ImGui::EndDisabled();
    }
    if (!enabled) {
        ImGui::EndDisabled();
    }

    ImGui::EndGroup();
}

} // namespace app::ui
