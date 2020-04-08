#include "AppScreen.h"
#include <Bar.h>
#include <StatusBar.h>
#include <ButtonInfoBar.h>
#include <WidgetMosaicComp.h>

using namespace Codingfield::UI;

AppScreen::AppScreen(Size size, Color color) : Screen(size, color) {

}

AppScreen::AppScreen(Size size, Color color, StatusBar* topBar, ButtonInfoBar* bottomBar, WidgetMosaicComp* centreWidget) : Screen(size, color), topBar{topBar}, bottomBar{bottomBar}, centreWidget{centreWidget} {
  if(topBar != nullptr) {
    topBar->SetParent(this);
    topBar->SetPosition(Point(0,0));
    topBar->SetSize(Size(this->GetSize().width, barHeight));
    AddChild(topBar);
  }

  if(bottomBar != nullptr) {
    bottomBar->SetParent(this);
    bottomBar->SetPosition(Point(0, this->GetSize().height-barHeight));
    bottomBar->SetSize(Size(this->GetSize().width, barHeight));
    AddChild(bottomBar);
  }

  AddCentreWidget(centreWidget);

}

void AppScreen::AddCentreWidget(WidgetMosaicComp* centreWidget) {

  if(centreWidget != nullptr) {

    Point centerPosition;
    centerPosition.y = position.y + ((topBar != nullptr) ? topBar->GetSize().height : 0) + padding;
    centerPosition.x = position.x + padding;

    Size centerSize;
    centerSize.height = size.height - (((bottomBar != nullptr) ? bottomBar->GetSize().height : 0) + ((topBar != nullptr) ? topBar->GetSize().height : 0) + padding);
    centerSize.width = size.width;

    //centreWidget->SetParent(this);
    centreWidget->SetPosition(centerPosition);
    centreWidget->SetSize(centerSize);

    centreWidget->SetMosaicActive(true);
    centreWidget->SetInZoom(true);

    AddChild(centreWidget);

  }
}


void AppScreen::OnButtonAPressed() {
  if(centreWidget != nullptr)
    centreWidget->OnButtonAPressed();
}

void AppScreen::OnButtonBPressed() {
  if(centreWidget != nullptr)
    centreWidget->OnButtonBPressed();
}

void AppScreen::OnButtonBLongPush() {
  if(centreWidget != nullptr)
    centreWidget->OnButtonBLongPush();
}

void AppScreen::OnButtonCPressed() {
  if(centreWidget != nullptr)
    centreWidget->OnButtonCPressed();
}

void AppScreen::Draw(Widget* focus){

  _ManagePhisicalButtons(focus);

  if ((millis() - _lastTs) > REFRESHTIME) {
    Screen::Draw();
    _lastTs = millis();
  }
  
}

void AppScreen::_ManagePhisicalButtons(Widget* focus){

  // Notify the widgets that physical buttons are pressed
  if (focus != nullptr) {
    if(M5.BtnA.wasPressed()) {
        focus->OnButtonAPressed();
        Serial.println("Butt A");
    }

    if(M5.BtnB.pressedFor(1000)) {
        if(!_longPush) {
        focus->OnButtonBLongPush();
        _longPush = true;
        Serial.println("Butt B Long");
        }
    }
    else if(M5.BtnB.wasReleased()) {
        if(!_longPush) {
        focus->OnButtonBPressed();
        Serial.println("Butt B");
        }
        else {
        _longPush = false;
        }
    }

    if(M5.BtnC.wasPressed()) {
        focus->OnButtonCPressed();
        Serial.println("Butt C");
    }
  }
}

