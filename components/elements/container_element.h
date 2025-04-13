#pragma once

#include "element.h"
#include "timer.h"

namespace esphome::elements {

// The modes to check conditions on.
enum class ConditionMode {
  ANY,
  ALL,
};

/// Element that delgates to a list of other elements.
class ContainerElement : public Element {
public:
  ContainerElement(ElementType type, ElementComponent *component,
                   Element *parent, ConditionMode active_mode)
      : Element(type, component, parent), active_mode_(active_mode) {}

  // The strategy to infer the activeness of the container based on its
  // children.
  void set_active_mode(ConditionMode active_mode) {
    active_mode_ = active_mode;
  };
  ConditionMode get_active_mode() { return active_mode_; }

  void add_element(Element *element);

  void on_show() override;
  void on_hide() override;
  bool is_active() override;

protected:
  std::vector<Element *> elements_ = {};
  ConditionMode active_mode_;
};

/// Draws multiple elements on top of each other.
class OverlayElement : public ContainerElement {
public:
  OverlayElement(ElementComponent *component, Element *parent)
      : ContainerElement(ElementType::OVERLAY, component, parent,
                         ConditionMode::ANY) {}

  void draw(display::Display &display) override;
};

/// Draws the first element that is active.
class PriorityElement : public ContainerElement {
public:
  PriorityElement(ElementComponent *component, Element *parent)
      : ContainerElement(ElementType::PRIORITY, component, parent,
                         ConditionMode::ANY) {}

  void draw(display::Display &display) override;

  void on_show() override;
  void on_hide() override;

protected:
  int index_ = -1;
  int find_active_index_();
};

/// Draws multiple elements evenly horizontally spaced next to each other.
class HorizontalElement : public ContainerElement {
public:
  HorizontalElement(ElementComponent *component, Element *parent)
      : ContainerElement(ElementType::HORIZONTAL, component, parent,
                         ConditionMode::ALL) {}

  void draw(display::Display &display) override;
};

/// Draws multiple elements evently vertically spaced above each other.
class VerticalElement : public ContainerElement {
public:
  VerticalElement(ElementComponent *component, Element *parent)
      : ContainerElement(ElementType::VERTICAL, component, parent,
                         ConditionMode::ALL) {}

  void draw(display::Display &display) override;
};

/// Draws multiple elements in sequence.
class SequenceElement : public ContainerElement {
public:
  SequenceElement(ElementComponent *component, Element *parent)
      : ContainerElement(ElementType::SEQUENCE, component, parent,
                         ConditionMode::ANY) {}

  void set_duration(uint32_t duration_ms) {
    timer_.set_duration(duration_ms);
    timer_.reset(get_component().get_current_ms());
  }

  void draw(display::Display &display) override;
  void go_to(int index);

  void on_show() override;
  void on_hide() override;
  void on_next() override;

protected:
  Timer timer_;
  int index_ = 0;
};

} // namespace esphome::elements
