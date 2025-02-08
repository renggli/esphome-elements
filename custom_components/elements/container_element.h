#pragma once

#include "element.h"
#include "esphome/components/display/display.h"

namespace esphome::elements {

class ContainerElement : public Element {
 public:
  explicit ContainerElement(ElementType type) : Element(type) {}

  void add_element(Element* element) { elements_.push_back(element); }

 protected:
  std::vector<Element*> elements_ = {};
};

/// Draws multiple elements on top of each other.
class OverlayElement : public ContainerElement {
 public:
  explicit OverlayElement() : ContainerElement(OVERLAY_ELEMENT) {}

  void draw(const Context& context, display::Display& display) override;
};

/// Draws multiple elements horizontally next to each other.
class HorizontalElement : public ContainerElement {
 public:
  explicit HorizontalElement() : ContainerElement(HORIZONTAL_ELEMENT) {}

  void draw(const Context& context, display::Display& display) override;
};

/// Draws multiple elements vertically next to each other.
class VerticalElement : public ContainerElement {
 public:
  explicit VerticalElement() : ContainerElement(VERTICAL_ELEMENT) {}

  void draw(const Context& context, display::Display& display) override;
};

// Draws multiple elements in sequence.
class SequenceElement : public ContainerElement {
 public:
  explicit SequenceElement() : ContainerElement(SEQUENCE_ELEMENT) {}

  void set_duration(uint32_t duration_ms) { duration_ms_ = duration_ms; }

  void draw(const Context& context, display::Display& display) override;
 protected:
  uint32_t duration_ms_ = 0;
  uint32_t start_ms_ = 0;
  uint32_t index_ = 0;
};

}  // namespace esphome::elements
