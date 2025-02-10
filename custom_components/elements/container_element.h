#pragma once

#include "element.h"
#include "utils.h"

namespace esphome::elements {

/// Element that delgates to a list of other elements.
class ContainerElement : public Element {
 public:
  explicit ContainerElement(ElementType type, ElementComponent* component,
                            Element* parent)
      : Element(type, component, parent) {}

  void add_element(Element* element);

  void on_show() override;
  void on_hide() override;

 protected:
  std::vector<Element*> elements_ = {};
};

/// Draws multiple elements on top of each other.
class OverlayElement : public ContainerElement {
 public:
  explicit OverlayElement(ElementComponent* component, Element* parent)
      : ContainerElement(ElementType::OVERLAY, component, parent) {}

  void draw(display::Display& display) override;
};

/// Draws multiple elements evenly horizontally spaced next to each other.
class HorizontalElement : public ContainerElement {
 public:
  explicit HorizontalElement(ElementComponent* component, Element* parent)
      : ContainerElement(ElementType::HORIZONTAL, component, parent) {}

  void draw(display::Display& display) override;
};

/// Draws multiple elements evently vertically spaced above each other.
class VerticalElement : public ContainerElement {
 public:
  explicit VerticalElement(ElementComponent* component, Element* parent)
      : ContainerElement(ElementType::VERTICAL, component, parent) {}

  void draw(display::Display& display) override;
};

/// Draws multiple elements in sequence.
class SequenceElement : public ContainerElement {
 public:
  explicit SequenceElement(ElementComponent* component, Element* parent)
      : ContainerElement(ElementType::SEQUENCE, component, parent) {}

  void set_duration(uint32_t duration_ms) {
    timer_.set_duration(duration_ms);
    timer_.reset(get_context().current_ms);
  }

  void draw(display::Display& display) override;
  void go_to(int index);

  void on_show() override;
  void on_hide() override;
  void on_next() override;

 protected:
  Timer timer_;
  uint32_t index_ = 0;
};

}  // namespace esphome::elements
