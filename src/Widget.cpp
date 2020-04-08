#include "Widget.h"


using namespace Codingfield::UI;

Widget::Widget(Widget* parent) : Widget(parent, Point(0,0), Size(0,0)) {
}

Widget::Widget(Widget* parent, Point position, Size size) :  parent{parent}, position{position}, size {size} {
  if(parent != nullptr)
    parent->AddChild(this);
}

Widget::Widget(Widget* parentW, int32_t* ptrVariable, unsigned long* ptrMillis) {  // NEW

  this->parent = parentW;
  this->position = Point(0,0);
  this->size = Size(0,0);

  if(parent != nullptr)
    parent->AddChild(this);

  this->ptrVar = ptrVariable;

  this->ptrTime = ptrMillis;
}



void Widget::Invalidate() {
  for(auto c : children) {
    c->Invalidate();
  }
  isInvalidated = true;
}

void Widget::SetSize(const Size& s) {
  size = s;
  OnSizeUpdated();
}

void Widget::SetPosition(const Point& p) {
  position = p;
  OnPositionUpdated();
}

void Widget::SetTextSize(uint8_t textSize) {
  this->textSize = textSize;
}


uint8_t Widget::GetTextSize() {
  return(textSize);
}