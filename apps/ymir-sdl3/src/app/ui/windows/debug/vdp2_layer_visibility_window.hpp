#pragma once

#include "vdp_window_base.hpp"

#include <app/ui/views/debug/vdp2_layer_visibility_view.hpp>

namespace app::ui {

class VDP2LayerVisibilityWindow : public VDPWindowBase {
public:
    VDP2LayerVisibilityWindow(SharedContext &context);

protected:
    void DrawContents() override;

private:
    VDP2LayerVisibilityView m_layerVisibilityView;
};

} // namespace app::ui