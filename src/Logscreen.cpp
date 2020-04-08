#include "Logscreen.h"
#include "Free_Fonts.h"

using namespace Codingfield::UI;

/* Please, please, M5STack, use namespaces around your classes! */

int lcdWidth = 320;

Codingfield::UI::Logscreen::Logscreen(Widget* parent) : Button(parent) {

}

Codingfield::UI::Logscreen::Logscreen(Widget* parent, int32_t* ptrVar, String* ptrTextLine) : Button(parent, ptrVar, nullptr) {
  this->ptrTextLine = ptrTextLine;
}

Codingfield::UI::Logscreen::Logscreen(Widget* parent, Point position, Size size) : Button(parent, position, size) {

}

void Codingfield::UI::Logscreen::Draw() {
  if(IsHidden()){
    return;
  }

  if(!IsInZoom()) {

    // TODO: Draw a black rectagle to remove all text.
    //if (_oldIsInZoom) M5.Lcd.fillRect(0, position.y-1, lcdWidth, size.height+2, BLACK);

    Button::DrawNormal();
  }
  else if (ptrTextLine!=nullptr) DrawZoom();
  else DrawNormal();

  _oldIsInZoom = IsInZoom();
  isInvalidated = false;
}


void Codingfield::UI::Logscreen::DrawZoom() {

  if (!_oldIsInZoom || M5.Lcd.getCursorY() > (position.y + size.height - 5) ) {
    M5.Lcd.fillRect(0, position.y-1, lcdWidth, size.height+2, BLACK);
    M5.Lcd.setCursor(0, position.y+10);
  }

  if (ptrTextLine != nullptr && oldTextLine != *ptrTextLine) {  
    
    M5.Lcd.setFreeFont(TT1);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(WHITE);
    M5.lcd.println(*ptrTextLine);
    oldTextLine=*ptrTextLine;
  }

}

