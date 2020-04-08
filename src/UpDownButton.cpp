#include "UpDownButton.h"
#include "Free_Fonts.h"
using namespace Codingfield::UI;

UpDownButton::UpDownButton(Widget* parent) : Button(parent) {
  isEditable = true;
}

UpDownButton::UpDownButton(Widget* parent, int* ptrVar) : Button(parent, ptrVar) {
  isEditable = true;
}

void UpDownButton::Draw() {
  if(IsHidden()) return;

  bool wasInvalidated = isInvalidated;


  if (IsInZoom()) Button::DrawNormal(stringFromVarUnit(currentValue, unit));
  else Button::DrawNormal();
  
  isInvalidated = false;

  if(wasInvalidated) {
    if(controlsEnabled) {
      M5.Lcd.setTextDatum(MC_DATUM);
      M5.Lcd.setTextColor(textColor);
      M5.Lcd.setTextSize(textSize);
      M5.Lcd.drawString("-", position.x + (size.width/6), position.y + (size.height/2));
      M5.Lcd.drawString("+", position.x + (size.width - (size.width/6)), position.y + (size.height/2));

    }
  }
}


void UpDownButton::EnableControls()  {
  controlsEnabled = true;

  // Get the current value
  currentValue = *ptrVar;
}

void UpDownButton::DisableControls() {
  controlsEnabled = false;
}

void UpDownButton::OnButtonAPressed() {
  if(upCallback != nullptr) {
    downCallback(this);
  } else {
    //*ptrVar = *ptrVar - 1;
    currentValue--;
    Invalidate();
  }
}

void UpDownButton::OnButtonBPressed() {
  if(applyCallback != nullptr) {
    applyCallback(this);
  } else {
    *ptrVar = currentValue;
    SetText(stringFromVarUnit());
    Invalidate();
  }
}

void UpDownButton::OnButtonBLongPush() {
  if(cancelCallback != nullptr) {
    cancelCallback(this);
  } else {
    //*ptrVar = oldValue;
    SetText(stringFromVarUnit());
    Invalidate();
  }
}

void UpDownButton::OnButtonCPressed() {
  if(upCallback != nullptr) {
    upCallback(this);
  } else {
    currentValue++;
    Invalidate();
  }
}

void UpDownButton::SetUpCallback(std::function<bool (UpDownButton* widget)> callback) {
  upCallback = callback;
}

void UpDownButton::SetDownCallback(std::function<bool (UpDownButton* widget)> callback) {
  downCallback = callback;
}

void UpDownButton::SetApplyCallback(std::function<bool (UpDownButton* widget)> callback) {
  applyCallback = callback;
}

void UpDownButton::SetCancelCallback(std::function<bool (UpDownButton* widget)> callback) {
  cancelCallback = callback;
}
