#include <Arduino.h>

#include "Button.h"
#include "Free_Fonts.h"

#define GRAPHRESOLUTION 30
#define GRAPHSPACE 10

using namespace Codingfield::UI;

/* Please, please, M5STack, use namespaces around your classes! */

Codingfield::UI::Button::Button(Widget* parent) : Widget(parent) {

}

Codingfield::UI::Button::Button(Widget* parent, int32_t* ptrVar, unsigned long* ptrTime) : Widget(parent, ptrVar, ptrTime) {

  // Define the buffer used in the graph
  circBuffer.SetSize(GRAPHRESOLUTION);

}

Codingfield::UI::Button::Button(Widget* parent, Point position, Size size) : Widget(parent, position, size) {

}


void Codingfield::UI::Button::SetBackgroundColor(Color c) {
  backgroundColorUpdated = true;
  this->backgroundColor = c;
}

void Codingfield::UI::Button::SetTextColor(Color c) {
  this->textColor = c;
}

void Codingfield::UI::Button::SetSelected(bool s) {
  if(isSelected != s) {
    wasSelected = isSelected;
    isSelected = s;
  }
}

void Codingfield::UI::Button::SetText(const std::string& t) {
  if(text != t) {
    if(text.empty())
      oldText = t;
    else
      this->oldText = this->text;
    this->text = t;
  }
}

void Codingfield::UI::Button::SetUnit(const std::string& u) {
  if(unit != u) {
    if(unit.empty())
      oldUnit = u;
    else
      this->oldUnit = this->unit;
    this->unit = u;
  }
}

void Codingfield::UI::Button::SetTitle(const std::string& t) {
  if(title != t) {
    if(title.empty())
      oldTitle = t;
    else
      oldTitle = title;

    title = t;

    // If there is no Code ID, get SetTitle
    if (GetCodeId()=="") SetCodeId(t.c_str());
  }
}

void Codingfield::UI::Button::Draw() {

  bool reDrawGraph=false;

  if(IsHidden()){
    return;
  }

  // Save values to graph later

  if (ptrTime!=nullptr) {

    unsigned long newTime = *ptrTime;

    if (oldTime != newTime) {
      circBuffer.push(*ptrVar);
      //circBuffer.println();
      oldTime = newTime;
      reDrawGraph=true;
    }
  }

  if(!IsInZoom()) { 

    DrawNormal();

  } else if (IsInZoom() && ptrTime!=nullptr) {

    if (reDrawGraph) DrawZoomGraph();

  } else DrawNormal();

  isInvalidated = false;
}


void Codingfield::UI::Button::DrawNormal(std::string strText) {

  // Update with registered var if text is empty

  //println("Draw Normal");

  if(ptrVar != nullptr && strText.empty()) {
    SetText(stringFromVarUnit());
  } else if (!strText.empty()) {
    SetText(strText);
  }

  bool forceUpdate = isInvalidated;

  if(forceUpdate) {
    Serial.println("FORCEUPDATE " + String(GetCodeId()));
  }

  int x = position.x + (size.width/2);
  int yText;
  int yTitle;
  if(title.empty()) {
    yText = position.y + (size.height/2);
  } else {
    yText = position.y + (size.height/3);
    yTitle = position.y + ((size.height/3)*2);
  }

  if(backgroundColorUpdated || forceUpdate) {
    M5.Lcd.fillRect(position.x, position.y, size.width, size.height, backgroundColor);
    backgroundColorUpdated = false;
    forceUpdate = true;
  }

  if(forceUpdate || (oldText != text)) {
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(backgroundColor);
    M5.Lcd.setTextSize(textSize);

    M5.Lcd.setFreeFont(FF22);
    M5.Lcd.drawString(oldText.c_str(), x, yText);

    M5.Lcd.setTextColor(textColor);
    M5.Lcd.drawString(text.c_str(), x, yText);
    oldText = text;
  }

  if(forceUpdate || (oldTitle != title)) {
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(backgroundColor);

    M5.Lcd.setFreeFont(FF21);
    M5.Lcd.drawString(oldTitle.c_str(), x, yTitle);

    M5.Lcd.setTextColor(textColor);
    M5.Lcd.drawString(title.c_str(), x, yTitle);
    oldTitle = title;
  }

  if(forceUpdate || (wasSelected != isSelected)) {
    if(isSelected) {
      M5.Lcd.drawRect(position.x, position.y, size.width, size.height, RED);
      M5.Lcd.drawRect(position.x+1, position.y+1, size.width-2, size.height-2, RED);
      M5.Lcd.drawRect(position.x+2, position.y+2, size.width-4, size.height-4, RED);
    } else {
      M5.Lcd.drawRect(position.x, position.y, size.width, size.height, backgroundColor);
      M5.Lcd.drawRect(position.x + 1, position.y + 1, size.width - 2, size.height - 2, backgroundColor);
      M5.Lcd.drawRect(position.x + 2, position.y + 2, size.width - 4, size.height - 4, backgroundColor);
    }
  }

}

void Codingfield::UI::Button::DrawZoom() {

  //println("Draw Zoom");

  int spaceInterval = 10;

  int value = *ptrVar;
  value = constrain(value, 0, size.height);

  int x = position.x + (*ptrTime * spaceInterval) % size.width;
  int y = size.height + position.y - value;

  if (x == position.x || isInvalidated || coldStart) {
    if (coldStart) coldStart=false;
    M5.Lcd.fillRect(position.x-1, position.y-1, size.width+2, size.height+2, BLACK);
    lastX = x;
    lastY = y;
  }

  M5.Lcd.drawLine(lastX, lastY, x, y, RED);

  lastX = x;
  lastY = y;
  
}


void Codingfield::UI::Button::DrawZoomGraph() {

  println("Draw Zoom Graph");
  
  if (isInvalidated || coldStart) {
    if (coldStart) coldStart=false;
    M5.Lcd.fillRect(position.x-1, position.y-1, size.width+2, size.height+2, BLACK);
  }

  int spaceInterval = GRAPHSPACE;

  // Draw the graph
  M5.Lcd.fillRect(position.x-1, position.y-1, size.width+2, size.height+2, BLACK);

  valueMin = circBuffer.minValue();
  valueMax = circBuffer.maxValue();

  if (abs(scaleMin - valueMin)<20 || abs(scaleMin - valueMin)>40) scaleMin=valueMin - 30;
  if (abs(scaleMax - valueMax)<20 || abs(scaleMax - valueMax)>40) scaleMax=valueMax + 30;

  M5.Lcd.setFreeFont(TT1);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.drawString(String(scaleMax), position.x+10, position.y+10);
  M5.Lcd.drawString(String(scaleMin), position.x+10, size.height + position.y-10);

  int zeroScaled = size.height + position.y - map(0, scaleMin, scaleMax, 0, size.height);
  M5.Lcd.drawLine(position.x+5, zeroScaled, position.x + size.width -5, zeroScaled, WHITE);

  for (int i=0;i<GRAPHRESOLUTION;i++) {

    int value = circBuffer.at(i);
    value = constrain(value, 0, size.height);

    int valueScale = map(value, scaleMin, scaleMax, 0, size.height);

    //M5.Lcd.drawString(String(value), position.x+10, size.height + position.y-10);

    //int x = position.x + ((GRAPHRESOLUTION-1-i) * spaceInterval) % size.width;
    int x = position.x + (i* spaceInterval) % size.width;
    int y = size.height + position.y - valueScale;

    if (i>0) M5.Lcd.drawLine(lastX, lastY, x, y, RED);

    lastX = x;
    lastY = y;
  }

  
}


std::string Codingfield::UI::Button::stringFromVarUnit(int32_t val, std::string unitStr) {
  return(String(val).c_str() + unitStr);
}

std::string Codingfield::UI::Button::stringFromVarUnit() {
  return(stringFromVarUnit(*ptrVar, unit));
}