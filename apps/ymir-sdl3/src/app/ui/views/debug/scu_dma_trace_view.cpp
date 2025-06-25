#include "scu_dma_trace_view.hpp"

namespace app::ui {

SCUDMATraceView::SCUDMATraceView(SharedContext &context)
    : m_context(context)
    , m_tracer(context.tracers.SCU) {}

void SCUDMATraceView::Display() {
    const float paddingWidth = ImGui::GetStyle().FramePadding.x;
    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.medium);
    const float hexCharWidth = ImGui::CalcTextSize("F").x;
    ImGui::PopFont();

    ImGui::BeginGroup();

    ImGui::Checkbox("Enable", &m_tracer.traceDMA);
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
        ImGui::TextUnformatted("You must also enable tracing in Debug > Enable tracing (F11)");
        ImGui::EndTooltip();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        m_tracer.ClearDMATransfers();
    }

    if (ImGui::BeginTable("dma_trace", 6,
                          ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Sortable)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_PreferSortDescending);
        ImGui::TableSetupColumn("Ch", ImGuiTableColumnFlags_PreferSortDescending);
        ImGui::TableSetupColumn("Indirect", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort,
                                paddingWidth * 2 + hexCharWidth * 7);
        ImGui::TableSetupColumn("Source", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort,
                                paddingWidth * 2 + hexCharWidth * 10);
        ImGui::TableSetupColumn("Destination", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort,
                                paddingWidth * 2 + hexCharWidth * 10);
        ImGui::TableSetupColumn("Length", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort,
                                paddingWidth * 2 + hexCharWidth * 7);
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableHeadersRow();

        const size_t count = m_tracer.dmaTransfers.Count();
        for (size_t i = 0; i < count; i++) {
            auto *sort = ImGui::TableGetSortSpecs();
            bool reverse = false;
            if (sort != nullptr && sort->SpecsCount == 1) {
                reverse = sort->Specs[0].SortDirection == ImGuiSortDirection_Descending;
            }

            auto trace = reverse ? m_tracer.dmaTransfers.ReadReverse(i) : m_tracer.dmaTransfers.Read(i);

            ImGui::TableNextRow();
            if (ImGui::TableNextColumn()) {
                ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.medium);
                ImGui::Text("%u", trace.counter);
                ImGui::PopFont();
            }
            if (ImGui::TableNextColumn()) {
                ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.medium);
                ImGui::Text("%u", trace.channel);
                ImGui::PopFont();
            }
            if (ImGui::TableNextColumn()) {
                if (trace.indirect) {
                    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.medium);
                    ImGui::Text("%07X", trace.indirectAddr);
                    ImGui::PopFont();
                } else {
                    ImGui::TextUnformatted("no");
                }
            }
            if (ImGui::TableNextColumn()) {
                ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.medium);
                ImGui::Text("%07X", trace.srcAddr);
                ImGui::PopFont();
                ImGui::SameLine();
                ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.small);
                ImGui::TextDisabled("+%d", trace.srcAddrInc);
                ImGui::PopFont();
            }
            if (ImGui::TableNextColumn()) {
                ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.medium);
                ImGui::Text("%07X", trace.dstAddr);
                ImGui::PopFont();
                ImGui::SameLine();
                ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fonts.sizes.small);
                ImGui::TextDisabled("+%d", trace.dstAddrInc);
                ImGui::PopFont();
            }
            if (ImGui::TableNextColumn()) {
                ImGui::Text("%u", trace.xferCount);
            }
        }

        ImGui::EndTable();
    }

    ImGui::EndGroup();
}

} // namespace app::ui
