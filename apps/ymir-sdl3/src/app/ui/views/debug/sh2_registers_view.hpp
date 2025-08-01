#pragma once

#include <app/shared_context.hpp>

namespace app::ui {

class SH2RegistersView {
public:
    SH2RegistersView(SharedContext &context, ymir::sh2::SH2 &sh2);

    void Display();

    float GetViewWidth();

private:
    SharedContext &m_context;
    ymir::sh2::SH2 &m_sh2;
};

} // namespace app::ui
