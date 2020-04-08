#pragma once

#include "Widget.h"
#include "CircularQueue.h"

// Necessary to deal with ButtonInfoBar / StatusBar handlers
#include <ButtonInfoBar.h>
#include <StatusBar.h>

namespace Codingfield {
  namespace UI {
    class Button : public Widget {
    public:
      Button(Widget* parent);
      Button(Widget* parent, int32_t* ptrVar, unsigned long* ptrTime=nullptr); // NEW
      Button(Widget* parent, Point position, Size size);

      void SetBackgroundColor(Color c);
      void SetTextColor(Color c);
      virtual void SetSelected(bool s) override;
      void SetText(const std::string& t);
      void SetTitle(const std::string& t);
      void SetUnit(const std::string& t);
      void Draw() override;
      void DrawNormal(std::string strText="");
      void DrawZoom();
      void DrawZoomGraph();


      /* To be able to manage the bars inside the widget*/

      /*
      void SetBottomBarHandler(ButtonInfoBar* w){bottomBar = w;}
      ButtonInfoBar* GetBottomBarHandler() {return (bottomBar);};
      void SetTopBarHandler(StatusBar* w){topBar = w;}
      StatusBar* GetTopBarHandler() {return (topBar);};
      */


      //Override Widget by button
      //virtual void AddChild(Button* butt) { children.push_back(butt); }

    protected:
      Color backgroundColor = BLACK;
      bool backgroundColorUpdated = true;
      Color textColor = BLACK;
      std::string text="";
      std::string oldText;
      std::string title;
      std::string oldTitle;

      std::string unit; // NEW
      std::string oldUnit;

      std::string stringFromVarUnit(int32_t strVal, std::string unitStr);
      std::string stringFromVarUnit();
      

      int lastX=0;
      int lastY=0;
      bool coldStart=true;

      bool wasSelected = false;

      // GRaph
      
      CircularQueue circBuffer;
      unsigned long oldTime;
      int scaleMin;
      int scaleMax;

      int valueMin;
      int valueMax;


    };
  }
}
