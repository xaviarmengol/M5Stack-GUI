#pragma once

#include "Widget.h"
#include "Point.h"
//#include "AppScreen.h"

// Necessary to deal with ButtonInfoBar / StatusBar handlers
#include <ButtonInfoBar.h>
#include <StatusBar.h>

namespace Codingfield {
  namespace UI {
    class WidgetMosaic : public Widget {
    public:
      WidgetMosaic(int32_t nbColumnsp = 3, int32_t nbRowsp = 2);
      WidgetMosaic(Widget* parent, Point position, Size size, int32_t nbColumns = 3, int32_t nbRows = 2);
      void Draw() override;
      virtual void AddChild(Widget* widget) override;
      virtual void OnButtonAPressed() override;
      virtual void OnButtonBPressed() override;
      virtual void OnButtonBLongPush() override;
      virtual void OnButtonCPressed() override;
      const Widget* GetSelected() const;
      bool IsZoomOnSelected() const;
      void SetZoomOnSelected(bool zoom);
      void SetZoomOnSelectedCallback(std::function<void (Widget* widget, bool)> func);

      // Get the handlers to bottom and top bar to be able to manage them inside Mosaic

      void SetBottomBarHandler(ButtonInfoBar* w){bottomBar = w;}
      Widget* GetBottomBarHandler() {return (bottomBar);};
      void SetTopBarHandler(StatusBar* w){topBar = w;}
      Widget* GetTopBarHandler() {return (topBar);};


    private:
      int32_t indexSelected = 0;
      int32_t border = 5;
      int32_t nbRows;
      int32_t nbColumns;

      Size ComputeWidgetSize();
      Size ComputeWidgetSize(int32_t nbColumns, int32_t nbRows);
      Point ComputeWidgetPosition(const Size& widgetSize, int32_t position);
      Widget* selectedWidget = nullptr;
      bool zoomOnSelected = false;
      void ZoomOnSelected(bool enabled);
      std::function<void (Widget* widget, bool)> zoomOnSelectedCallback = nullptr;

      ButtonInfoBar* bottomBar=nullptr;
      StatusBar* topBar=nullptr;

      void _ManagePhisicalButtons();
      bool _longPush;

    };
  }
}
