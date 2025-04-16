#pragma once

#include "element.h"

namespace esphome::elements {

// The strategy to infer the activeness of the container based on the state
// of its children.
enum class ActiveMode {
  // The container is always active.
  ALWAYS,
  // The container is active, if any of its children are.
  ANY,
  // The container is active, if all of its children are.
  ALL,
  // The container is never active.
  NEVER,
};

/// Element that delgates to a list of other elements.
class ContainerElement : public Element {
public:
  ContainerElement(ElementType type, ElementComponent *component,
                   Element *parent, ActiveMode active_mode)
      : Element(type, component, parent), active_mode_(active_mode) {}

  void set_active_mode(ActiveMode active_mode) { active_mode_ = active_mode; };
  ActiveMode get_active_mode() { return active_mode_; }

  void add_element(Element *element);

  void dump_config(int level) override;

  void on_show() override;
  void on_hide() override;
  bool is_active() override;

protected:
  std::vector<Element *> elements_ = {};
  ActiveMode active_mode_;
};

/// Draws multiple elements on top of each other.
class OverlayElement : public ContainerElement {
public:
  OverlayElement(ElementComponent *component, Element *parent)
      : ContainerElement(ElementType::OVERLAY, component, parent,
                         ActiveMode::ANY) {}

  void draw(display::Display &display) override;
};

/// Draws the first element that is active.
class PriorityElement : public ContainerElement {
public:
  PriorityElement(ElementComponent *component, Element *parent)
      : ContainerElement(ElementType::PRIORITY, component, parent,
                         ActiveMode::ANY) {}

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
                         ActiveMode::ALL) {}

  void draw(display::Display &display) override;
};

/// Draws multiple elements evently vertically spaced above each other.
class VerticalElement : public ContainerElement {
public:
  VerticalElement(ElementComponent *component, Element *parent)
      : ContainerElement(ElementType::VERTICAL, component, parent,
                         ActiveMode::ALL) {}

  void draw(display::Display &display) override;
};

/// Draws multiple elements in sequence.
class SequenceElement : public ContainerElement {
public:
  SequenceElement(ElementComponent *component, Element *parent)
      : ContainerElement(ElementType::SEQUENCE, component, parent,
                         ActiveMode::ANY) {}

  void draw(display::Display &display) override;
  void go_to(int index);

  void on_show() override;
  void on_hide() override;
  void on_next() override;

protected:
  int index_ = -1;
};

} // namespace esphome::elements
