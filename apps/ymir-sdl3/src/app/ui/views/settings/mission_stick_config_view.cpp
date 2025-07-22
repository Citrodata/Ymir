#include "mission_stick_config_view.hpp"

namespace app::ui {

MissionStickConfigView::MissionStickConfigView(SharedContext &context)
    : SettingsViewBase(context)
    , m_inputCaptureWidget(context, m_unboundActionsWidget)
    , m_unboundActionsWidget(context) {}

void MissionStickConfigView::Display(Settings::Input::Port::MissionStick &controllerSettings, uint32 portIndex) {
    auto &binds = controllerSettings.binds;

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Mode:");
    ImGui::SameLine();
    if (ImGui::RadioButton("Three-axis", !m_context.missionStickInputs[portIndex].sixAxisMode)) {
        m_context.missionStickInputs[portIndex].sixAxisMode = false;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Six-axis", m_context.missionStickInputs[portIndex].sixAxisMode)) {
        m_context.missionStickInputs[portIndex].sixAxisMode = true;
    }

    if (ImGui::Button("Restore defaults")) {
        m_unboundActionsWidget.Capture(m_context.settings.ResetBinds(binds, true));
        MakeDirty();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear all")) {
        m_unboundActionsWidget.Capture(m_context.settings.ResetBinds(binds, false));
        MakeDirty();
    }

    ImGui::TextUnformatted("Left-click a button to assign a hotkey. Right-click to clear.");
    m_unboundActionsWidget.Display();
    if (ImGui::BeginTable("hotkeys", 1 + input::kNumBindsPerInput,
                          ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("Button", ImGuiTableColumnFlags_WidthFixed, 120.0f * m_context.displayScale);
        for (size_t i = 0; i < input::kNumBindsPerInput; i++) {
            ImGui::TableSetupColumn(fmt::format("Hotkey {}", i + 1).c_str(), ImGuiTableColumnFlags_WidthStretch, 1.0f);
        }
        ImGui::TableHeadersRow();

        auto drawRow = [&](input::InputBind &bind) {
            ImGui::TableNextRow();
            if (ImGui::TableNextColumn()) {
                ImGui::AlignTextToFramePadding();
                ImGui::TextUnformatted(bind.action.name);
            }
            for (uint32 i = 0; i < input::kNumBindsPerInput; i++) {
                if (ImGui::TableNextColumn()) {
                    m_inputCaptureWidget.DrawInputBindButton(bind, i, &m_context.analogPadInputs[portIndex]);
                }
            }
        };

        drawRow(binds.a);
        drawRow(binds.b);
        drawRow(binds.c);
        drawRow(binds.x);
        drawRow(binds.y);
        drawRow(binds.z);
        drawRow(binds.l);
        drawRow(binds.r);
        drawRow(binds.start);
        drawRow(binds.mainUp);
        drawRow(binds.mainDown);
        drawRow(binds.mainLeft);
        drawRow(binds.mainRight);
        drawRow(binds.mainStick);
        drawRow(binds.mainThrottle);
        drawRow(binds.mainThrottleUp);
        drawRow(binds.mainThrottleDown);
        drawRow(binds.mainThrottleMax);
        drawRow(binds.mainThrottleMin);
        drawRow(binds.subUp);
        drawRow(binds.subDown);
        drawRow(binds.subLeft);
        drawRow(binds.subRight);
        drawRow(binds.subStick);
        drawRow(binds.subThrottle);
        drawRow(binds.subThrottleUp);
        drawRow(binds.subThrottleDown);
        drawRow(binds.subThrottleMax);
        drawRow(binds.subThrottleMin);
        drawRow(binds.switchMode);

        m_inputCaptureWidget.DrawCapturePopup();

        ImGui::EndTable();
    }
}

} // namespace app::ui
