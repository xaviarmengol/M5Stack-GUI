#pragma once

#include "Widget.h"
#include "Button.h"
#include "Point.h"
#include "Free_Fonts.h"

#define PADDING 5

namespace Codingfield {
  namespace UI {
    class WidgetMosaicComp : public Button {
    public:
      WidgetMosaicComp(int32_t nbColumnsp = 3, int32_t nbRowsp = 2);
      WidgetMosaicComp(Widget* parent, Point position, Size size, int32_t nbColumns = 3, int32_t nbRows = 2);
      WidgetMosaicComp(Widget* parent, int32_t nbColumns, int32_t nbRows, StatusBar* topBar, ButtonInfoBar* bottomBar, int32_t* ptrVar=nullptr) ;

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

      void DrawZoom();


    protected:
      int32_t indexSelected = 0;
      int32_t border = 5;
      int32_t nbRows;
      int32_t nbColumns;

      Size ComputeWidgetSize();
      Size ComputeWidgetSize(const Size& sizeParam, int32_t nbColumns, int32_t nbRows);
      Point ComputeWidgetPosition(const Size& widgetSize, const Point& pos, int32_t position);
      Widget* selectedWidget = nullptr;
      bool zoomOnSelected = false;
      void ZoomOnSelected(bool enabled);
      std::function<void (Widget* widget, bool)> zoomOnSelectedCallback = nullptr;

      ButtonInfoBar* bottomBar;
      StatusBar* topBar;

      bool oldIsInZoom = false;

      bool oldIsActiveMosaic=false;

    };
  }
}
