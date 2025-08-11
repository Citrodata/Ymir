#include "sh2_divu_trace_view.hpp"

#include <cinttypes>

using namespace ymir;

namespace app::ui {

SH2DivisionUnitTraceView::SH2DivisionUnitTraceView(SharedContext &context, SH2Tracer &tracer)
    : m_context(context)
    , m_tracer(tracer) {}

void SH2DivisionUnitTraceView::Display() {
    ImGui::BeginGroup();

    ImGui::Checkbox("Enable", &m_tracer.traceDivisions);
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
        ImGui::TextUnformatted("You must also enable tracing in Debug > Enable tracing (F11)");
        ImGui::EndTooltip();
    }
    ImGui::SameLine();
    ImGui::Checkbox("Display numbers in hexadecimal", &m_showHex);
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        m_tracer.divisions.Clear();
        m_tracer.divStats.Clear();
        m_tracer.ResetDivisionCounter();
    }

    const float paddingWidth = ImGui::GetStyle().FramePadding.x;
    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
    const float hexCharWidth = ImGui::CalcTextSize("F").x;
    ImGui::PopFont();

    if (ImGui::BeginTable("divu_trace", 7,
                          ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Sortable)) {
        ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_PreferSortDescending);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort);
        ImGui::TableSetupColumn("Dividend", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort,
                                paddingWidth * 2 + hexCharWidth * 16);
        ImGui::TableSetupColumn("Divisor", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort,
                                paddingWidth * 2 + hexCharWidth * 8);
        ImGui::TableSetupColumn("Quotient", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort,
                                paddingWidth * 2 + hexCharWidth * 8);
        ImGui::TableSetupColumn("Remainder", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort,
                                paddingWidth * 2 + hexCharWidth * 8);
        ImGui::TableSetupColumn("Overflow", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoSort);
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableHeadersRow();

        const size_t count = m_tracer.divisions.Count();
        for (size_t i = 0; i < count; i++) {
            auto *sort = ImGui::TableGetSortSpecs();
            bool reverse = false;
            if (sort != nullptr && sort->SpecsCount == 1) {
                reverse = sort->Specs[0].SortDirection == ImGuiSortDirection_Descending;
            }

            auto trace = reverse ? m_tracer.divisions.ReadReverse(i) : m_tracer.divisions.Read(i);

            ImGui::TableNextRow();
            if (ImGui::TableNextColumn()) {
                ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
                ImGui::Text("%u", trace.counter);
                ImGui::PopFont();
            }
            if (ImGui::TableNextColumn()) {
                ImGui::Text("%dx32", trace.div64 ? 64 : 32);
            }
            if (ImGui::TableNextColumn()) {
                if (m_showHex) {
                    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
                    if (trace.div64) {
                        ImGui::Text("%016" PRIX64, trace.dividend);
                    } else {
                        ImGui::Text("%08X", (sint32)trace.dividend);
                    }
                    ImGui::PopFont();
                } else {
                    ImGui::Text("%" PRId64, trace.dividend);
                }
            }
            if (ImGui::TableNextColumn()) {
                if (m_showHex) {
                    ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
                    ImGui::Text("%08X", trace.divisor);
                    ImGui::PopFont();
                } else {
                    ImGui::Text("%d", trace.divisor);
                }
            }
            if (ImGui::TableNextColumn()) {
                if (trace.finished) {
                    if (m_showHex) {
                        ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
                        ImGui::Text("%08X", trace.quotient);
                        ImGui::PopFont();
                    } else {
                        ImGui::Text("%d", trace.quotient);
                    }
                }
            }
            if (ImGui::TableNextColumn()) {
                if (trace.finished) {
                    if (m_showHex) {
                        ImGui::PushFont(m_context.fonts.monospace.regular, m_context.fontSizes.medium);
                        ImGui::Text("%08X", trace.remainder);
                        ImGui::PopFont();
                    } else {
                        ImGui::Text("%d", trace.remainder);
                    }
                }
            }
            if (ImGui::TableNextColumn()) {
                if (trace.overflow) {
                    if (trace.overflowIntrEnable) {
                        ImGui::TextUnformatted("yes+IRQ");
                    } else {
                        ImGui::TextUnformatted("yes");
                    }
                }
            }
        }

        ImGui::EndTable();
    }

    ImGui::EndGroup();
}

} // namespace app::ui
