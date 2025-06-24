#pragma once

#include <app/ui/defs/settings_defs.hpp>

#include <app/ui/window_base.hpp>

#include <app/ui/views/settings/audio_settings_view.hpp>
#include <app/ui/views/settings/cartridge_settings_view.hpp>
#include <app/ui/views/settings/cdblock_settings_view.hpp>
#include <app/ui/views/settings/general_settings_view.hpp>
#include <app/ui/views/settings/hotkeys_settings_view.hpp>
#include <app/ui/views/settings/input_settings_view.hpp>
#include <app/ui/views/settings/ipl_settings_view.hpp>
#include <app/ui/views/settings/system_settings_view.hpp>
#include <app/ui/views/settings/video_settings_view.hpp>
#include <app/ui/views/settings/tweaks_settings_view.hpp>

namespace app::ui {

class SettingsWindow : public WindowBase {
public:
    SettingsWindow(SharedContext &context);

    void OpenTab(SettingsTab tab);

protected:
    void PrepareWindow() override;
    void DrawContents() override;

private:
    SettingsTab m_selectedTab = SettingsTab::None;

    GeneralSettingsView m_generalSettingsView;
    HotkeysSettingsView m_hotkeysSettingsView;
    SystemSettingsView m_systemSettingsView;
    IPLSettingsView m_iplSettingsView;
    InputSettingsView m_inputSettingsView;
    VideoSettingsView m_videoSettingsView;
    AudioSettingsView m_audioSettingsView;
    CartridgeSettingsView m_cartSettingsView;
    CDBlockSettingsView m_cdblockSettingsView;
    TweaksSettingsView m_tweaksSettingsView;
};

} // namespace app::ui
