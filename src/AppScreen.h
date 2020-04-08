#pragma once

#include <Screen.h>
#include <StatusBar.h>
#include <ButtonInfoBar.h>
#include <WidgetMosaicComp.h>


#define REFRESHTIME 10

namespace Codingfield {
  namespace UI {
    class Bar;
    class AppScreen : public Screen {
    public:
      AppScreen(Size size, Color color);
      AppScreen(Size size, Color color, StatusBar* topBar, ButtonInfoBar* bottomBar, WidgetMosaicComp* centreWidget);

      void Draw (Widget* focus=nullptr);

      virtual void OnButtonAPressed() override;
      virtual void OnButtonBPressed() override;
      virtual void OnButtonBLongPush() override;
      virtual void OnButtonCPressed() override;

      // Phisiscal

      void _ManagePhisicalButtons();

    private:
      StatusBar* topBar = nullptr;
      ButtonInfoBar* bottomBar = nullptr;
      Widget* centreWidget = nullptr;
      int32_t padding = 5;
      int32_t barHeight = 25;

      void _ManagePhisicalButtons(Widget* focus=nullptr);
      unsigned long _lastTs;
      bool _longPush=false;

      void AddCentreWidget(WidgetMosaicComp* centreWidget);

    };
  }
}

