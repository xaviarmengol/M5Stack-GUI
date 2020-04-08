#pragma once

#include <vector>

#include "Point.h"
#include "Size.h"


#include <M5Stack.h>

namespace Codingfield {
  namespace UI {
    using Color = uint32_t;

    /* Base class for all widgets
     * A widget is an object that has a parent widget, children, a position and a size,
     * and is able to draw itself on the screen.*/
    class Widget {

    protected:
      virtual void OnSizeUpdated() { }
      virtual void OnPositionUpdated() { }

      Point position;
      Size size;
      Widget* parent = nullptr;
      std::vector<Widget*> children;
      bool isSelected = false;
      bool isVisible = true;
      bool isEditable = false;
      bool isInvalidated = true;

      // Mosaic

      bool isMosaic = false;
      bool mosaicActive = false;

      int32_t* ptrVar = nullptr; // New ptr to variable
      unsigned long* ptrTime = nullptr; // New ptr to time variable (to be able to make graphs)
      uint8_t textSize = 1; // New Size of text

      bool isZoomOn = false;

      String codeId="";



    public:
      Widget(Widget* parent);
      Widget(Widget* parent, Point position, Size size);

      Widget(Widget* parent, int32_t* ptrVar, unsigned long* ptrMillis=nullptr); // NEW automatically get variable to show
      
      void SetParent(Widget* parent) { this->parent = parent; }

      const Size& GetSize() const { return size; }
      void SetSize(const Size& s);

      const Point& GetPosition() const {return position; }
      void SetPosition(const Point& p);

      uint8_t GetTextSize();
      void SetTextSize(uint8_t textSize);

      // DEBUG

      void println(String str="") {
        Serial.print("CID:" + String(codeId));
        Serial.print(" Siz: (" + String(size.width) + " " + String(size.height) + ")");
        Serial.print(" Pos: (" + String(position.x) + " " + String(position.y) + ")");
        Serial.print(" Zoom: " + String(IsInZoom()));
        Serial.print(" MosAct: " + String(IsMosaicActive()));
        Serial.println(" " + str);
      }



      /* The widget will be visible (drawn) the next time Draw() will be called */
      void Show() {
        if (!isVisible) {
          Invalidate();
          isVisible = true;
        }
      }

      /* The widget will not be visible (not drawn) the next time Draw() will be called */
      void Hide() {
        if (isVisible) {
          Invalidate();
          isVisible = false;
        }
      }

      bool IsVisible() const { return isVisible;}
      bool IsHidden() const { return !isVisible;}

      /* Returns true if the value of the widget can be edited via buttons */
      bool IsEditable() const { return isEditable; }

      /* Returns true if it is in Zoom */
      bool IsInZoom() const { return isZoomOn; }
      void SetInZoom(bool isZoomp) {isZoomOn = isZoomp;}

      /* Returns true if it is Mosaic */
      bool IsMosaic() const { return isMosaic; }
      void SetMosaic(bool isMosaicPar) {isMosaic = isMosaicPar;}

      /* Returns true if it is Mosaic Active*/
      bool IsMosaicActive() const { return mosaicActive; }
      void SetMosaicActive(bool isMosaicActivePar) {mosaicActive = isMosaicActivePar;}


      /* Add a widget as a child of this one */
      virtual void AddChild(Widget* widget) { children.push_back(widget); }

      /* Invalidate children */
      void InvalidateChildren() {
        for(auto index = 0; index < children.size(); index++) {
          children[index]->Invalidate();
        }
      }

      /* Draw the widget. Implemented by concrete implementations of Widget */
      virtual void Draw() = 0;

      /* These methods must be called by the application when buttons are pressed
       * They allow the implementation of Widget to react to user actions on
       * physical buttons. */
      virtual void OnButtonAPressed() { }
      virtual void OnButtonBPressed() { }
      virtual void OnButtonBLongPush() { }
      virtual void OnButtonCPressed() { }

      /* This method must be called by the application to specify is this instance
       * of Widget is selected or not. */
      virtual void SetSelected(bool s) { }

      /* Enables/disables controls on the button */
      virtual void EnableControls() {}
      virtual void DisableControls() {}
      void Invalidate();

      /* Code Id */

      void SetCodeId(String code) {
        codeId = code;
      }

      String GetCodeId(){
        return(codeId);
      }



    };
  }
}

