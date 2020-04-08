#pragma once

#include "Button.h"

namespace Codingfield {
  namespace UI {
    class Logscreen : public Button {
    public:
      Logscreen(Widget* parent);
      Logscreen(Widget* parent, int32_t* ptrVar, String* ptrTextLine=nullptr); // NEW
      Logscreen(Widget* parent, Point position, Size size);

      void Draw() override;
      void DrawZoom();

    protected:
      String* ptrTextLine=nullptr;
      String oldTextLine="";

      bool _oldIsInZoom=false;

    };
  }
}
