#include "WidgetMosaic.h"
using namespace Codingfield::UI;

WidgetMosaic::WidgetMosaic(Widget* parent, Point position, Size size, int32_t nbColumns, int32_t nbRows) : Widget(parent, position, size),
                                                                                                           nbColumns {nbColumns},
                                                                                                           nbRows {nbRows} {

}

WidgetMosaic::WidgetMosaic(int32_t nbColumns, int32_t nbRows) : WidgetMosaic(nullptr, Point(), Size(), nbColumns, nbRows) {

}


void WidgetMosaic::Draw() {

  _ManagePhisicalButtons();

  if(IsHidden()) return;
  
  if(isInvalidated) {
    M5.Lcd.fillRect(position.x, position.y, size.width, size.height, BLACK);
  }

  for(Widget* w : children)
    w->Draw();

  isInvalidated = false;

}




void WidgetMosaic::AddChild(Widget* widget) {
  Widget::AddChild(widget);

  Size widgetSize = ComputeWidgetSize();
  widget->SetSize(widgetSize);

  int32_t position = ((children.size()-1) % (nbRows*nbColumns));
  widget->SetPosition(ComputeWidgetPosition(widgetSize, position));
  Invalidate();
}

const Widget* WidgetMosaic::GetSelected() const {
  return selectedWidget;
}


void WidgetMosaic::ZoomOnSelected(bool enabled) {

  bool oldValue = IsZoomOnSelected();

  if(selectedWidget != nullptr && enabled) {

    for(auto* w : children)
      w->Hide();
    
    SetZoomOnSelected(true);
    Size widgetSize = ComputeWidgetSize(1,1);
    selectedWidget->SetSize(widgetSize);
    selectedWidget->SetPosition(ComputeWidgetPosition(widgetSize, 0));
    selectedWidget->Show();
    selectedWidget->Invalidate();
    
    if(selectedWidget->IsEditable()) {
      selectedWidget->EnableControls();
    }
  }  else {
    SetZoomOnSelected(false);

    for(auto index = 0; index < children.size(); index++) {
      Widget* widget = children.at(index);
      Size widgetSize = ComputeWidgetSize();
      widget->SetSize(widgetSize);
      widget->SetPosition(ComputeWidgetPosition(widgetSize, index));
      widget->Show();
      if(selectedWidget->IsEditable()) {
        selectedWidget->DisableControls();
      }
    }
  }

  if(selectedWidget != nullptr && oldValue != IsZoomOnSelected()) {
    
    if (zoomOnSelectedCallback != nullptr) {
      zoomOnSelectedCallback(selectedWidget, IsZoomOnSelected());
    } else {

      if(IsZoomOnSelected()) {
            if(selectedWidget->IsEditable()){
                bottomBar->SetButtonAText("-");
                bottomBar->SetButtonBText("APPLY");
                bottomBar->SetButtonCText("+");
            } else {
                bottomBar->SetButtonAText("");
                bottomBar->SetButtonBText("BACK");
                bottomBar->SetButtonCText("");
            }
        } else {
            bottomBar->SetButtonAText("<");
            bottomBar->SetButtonBText("SELECT");
            bottomBar->SetButtonCText(">");
        }
    }
    Invalidate();
  }

  if (selectedWidget != nullptr && oldValue != IsZoomOnSelected() && !IsZoomOnSelected()) {
    //Serial.println("Sortint");
  }

}

void WidgetMosaic::OnButtonAPressed() {

  if(IsZoomOnSelected())
    return selectedWidget->OnButtonAPressed();

  else {
    children[indexSelected]->SetSelected(false);

    if(indexSelected > 0)
      indexSelected--;
    else {
        indexSelected = children.size()-1;
    }

    children[indexSelected]->SetSelected(true);
    selectedWidget = children[indexSelected];     
  }
}


void WidgetMosaic::OnButtonBPressed() {
  if(selectedWidget == nullptr) return;
  if(IsZoomOnSelected()) {
    ZoomOnSelected(false);
    selectedWidget->OnButtonBPressed();
    return;
  }

  ZoomOnSelected(true);
}


void WidgetMosaic::OnButtonBLongPush() {
  if(IsZoomOnSelected())
    selectedWidget->OnButtonBLongPush();
  ZoomOnSelected(false);
}


void WidgetMosaic::OnButtonCPressed() {

  if(IsZoomOnSelected())

    return selectedWidget->OnButtonCPressed();

  else {

    children[indexSelected]->SetSelected(false);

    if(indexSelected < children.size()-1)
      indexSelected++;
    else 
        indexSelected = 0;

    children[indexSelected]->SetSelected(true);
    selectedWidget = children[indexSelected];     
  }
}

Size WidgetMosaic::ComputeWidgetSize() {
  return ComputeWidgetSize(nbColumns, nbRows);
}

Size WidgetMosaic::ComputeWidgetSize(int32_t nbColumns, int32_t nbRows) {
  Size widgetSize;
  widgetSize.width = (size.width - ((2*border) + ((nbColumns-1)*border))) / nbColumns; // ERA 2 !!!
  widgetSize.height = (size.height - ((2*border) + ((nbRows-1)*border))) / nbRows;
  return widgetSize;
}

Point WidgetMosaic::ComputeWidgetPosition(const Size& widgetSize, int32_t position) {
  Point widgetPosition;

  int32_t row = ((int32_t)position / (int32_t)nbColumns);
  int32_t column = position % nbColumns;
  widgetPosition.x = ((column)*border) + (column * widgetSize.width) + this->position.x;
  widgetPosition.y = ((row)*border) + (row * widgetSize.height) + this->position.y;

  return widgetPosition;
}

bool WidgetMosaic::IsZoomOnSelected() const {
  return zoomOnSelected;
}

void WidgetMosaic::SetZoomOnSelected(bool zoom) {
  zoomOnSelected = zoom;
  selectedWidget->SetInZoom(zoom);
}


void WidgetMosaic::SetZoomOnSelectedCallback(std::function<void (Widget* widget, bool)>func) {
  zoomOnSelectedCallback = func;
}


void WidgetMosaic::_ManagePhisicalButtons(){

  // Notify the widgets that physical buttons are pressed
  if (this != nullptr) {
    if(M5.BtnA.wasPressed()) {
        this->OnButtonAPressed();
        Serial.println("Butt A");
    }

    if(M5.BtnB.pressedFor(1000)) {
        if(!_longPush) {
        this->OnButtonBLongPush();
        _longPush = true;
        Serial.println("Butt B Long");
        }
    }
    else if(M5.BtnB.wasReleased()) {
        if(!_longPush) {
        this->OnButtonBPressed();
        Serial.println("Butt B");
        }
        else {
        _longPush = false;
        }
    }

    if(M5.BtnC.wasPressed()) {
        this->OnButtonCPressed();
        Serial.println("Butt C");
    }
  }
}

