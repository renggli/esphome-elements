#pragma once

#include "element.h"

namespace esphome::elements {

/// Element that delgates to a list of other elements.
class ContainerElement : public Element {
 public:
  explicit ContainerElement(ElementType type) : Element(type) {}

  void add_element(Element* element);

  void on_show(const Context& context) override;
  void on_hide(const Context& context) override;

 protected:
  std::vector<Element*> elements_ = {};
};

/// Draws multiple elements on top of each other.
class OverlayElement : public ContainerElement {
 public:
  explicit OverlayElement() : ContainerElement(ElementType::OVERLAY) {}

  void draw(const Context& context, display::Display& display) override;
};

/// Draws multiple elements evenly horizontally spaced next to each other.
class HorizontalElement : public ContainerElement {
 public:
  explicit HorizontalElement() : ContainerElement(ElementType::HORIZONTAL) {}

  void draw(const Context& context, display::Display& display) override;
};

/// Draws multiple elements evently vertically spaced above each other.
class VerticalElement : public ContainerElement {
 public:
  explicit VerticalElement() : ContainerElement(ElementType::VERTICAL) {}

  void draw(const Context& context, display::Display& display) override;
};

/// Draws multiple elements in sequence.
class SequenceElement : public ContainerElement {
 public:
  explicit SequenceElement() : ContainerElement(ElementType::SEQUENCE) {}

  void set_duration(optional<uint32_t> duration_ms) {
    duration_ms_ = duration_ms;
  }

  void draw(const Context& context, display::Display& display) override;

  void on_show(const Context& context) override;
  void on_hide(const Context& context) override;
  void on_next(const Context& context) override;

 protected:
  optional<uint32_t> duration_ms_ = nullopt;
  uint32_t start_ms_ = 0;
  uint32_t index_ = 0;
};

}  // namespace esphome::elements
