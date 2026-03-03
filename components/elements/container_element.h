#pragma once

#include <deque>
#include <vector>

#include "element.h"

namespace esphome::elements {

// The strategy to infer the activeness of the container based on the state
// of its children.
enum class ActiveMode : std::uint8_t {
  /// The container is always active.
  ALWAYS,
  /// The container is active if any of its children are.
  ANY,
  /// The container is active if all of its children are.
  ALL,
  /// The container is never active.
  NEVER,
};

/// Element that delegates to a list of other elements.
class ContainerElement : public Element {
 public:
  ContainerElement(ElementComponent *component, Element *parent, ActiveMode active_mode)
      : Element(component, parent), active_mode_(active_mode) {}

  void set_active_mode(ActiveMode active_mode) { active_mode_ = active_mode; };
  ActiveMode get_active_mode() { return active_mode_; }

  void add_element(Element *element);

  void dump_config(int level) override;

  bool is_active() override;

  void on_show() override;
  void on_hide() override;

 protected:
  std::vector<Element *> elements_;
  ActiveMode active_mode_;
};

/// Draws multiple elements on top of each other.
class OverlayElement : public ContainerElement {
 public:
  OverlayElement(ElementComponent *component, Element *parent) : ContainerElement(component, parent, ActiveMode::ANY) {}

  const char *get_type_name() const override { return "overlay"; }

  void draw(display::Display &display) override;
};

/// Draws the first element that is active.
class PriorityElement : public ContainerElement {
 public:
  PriorityElement(ElementComponent *component, Element *parent)
      : ContainerElement(component, parent, ActiveMode::ANY) {}

  const char *get_type_name() const override { return "priority"; }

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
      : ContainerElement(component, parent, ActiveMode::ALL) {}

  const char *get_type_name() const override { return "horizontal"; }

  void draw(display::Display &display) override;
};

/// Draws multiple elements evenly vertically spaced from top to bottom.
class VerticalElement : public ContainerElement {
 public:
  VerticalElement(ElementComponent *component, Element *parent)
      : ContainerElement(component, parent, ActiveMode::ALL) {}

  const char *get_type_name() const override { return "vertical"; }

  void draw(display::Display &display) override;
};

/// Draws multiple elements in random sequence.
class RandomElement : public ContainerElement {
 public:
  RandomElement(ElementComponent *component, Element *parent) : ContainerElement(component, parent, ActiveMode::ANY) {}

  const char *get_type_name() const override { return "random"; }

  void draw(display::Display &display) override;
  void go_to(int index);

  void prev();
  void next();

  void on_show() override;
  void on_hide() override;

 protected:
  int index_ = -1;
  std::deque<int> history_;

  void update_history_();
};

template<typename... Ts> class RandomNextAction : public Action<Ts...> {
 public:
  RandomNextAction(RandomElement *element) : element_(element) {}
  void play(const Ts &...x) override { element_->next(); }

 protected:
  RandomElement *element_;
};

template<typename... Ts> class RandomPrevAction : public Action<Ts...> {
 public:
  RandomPrevAction(RandomElement *element) : element_(element) {}
  void play(const Ts &...x) override { element_->prev(); }

 protected:
  RandomElement *element_;
};

/// Draws multiple elements in sequence.
class SequenceElement : public ContainerElement {
 public:
  SequenceElement(ElementComponent *component, Element *parent)
      : ContainerElement(component, parent, ActiveMode::ANY) {}

  const char *get_type_name() const override { return "sequence"; }

  void draw(display::Display &display) override;
  void go_to(int index);

  void prev();
  void next();

  void on_show() override;
  void on_hide() override;

 protected:
  int index_ = -1;
};

template<typename... Ts> class SequenceNextAction : public Action<Ts...> {
 public:
  SequenceNextAction(SequenceElement *element) : element_(element) {}
  void play(const Ts &...x) override { element_->next(); }

 protected:
  SequenceElement *element_;
};

template<typename... Ts> class SequencePrevAction : public Action<Ts...> {
 public:
  SequencePrevAction(SequenceElement *element) : element_(element) {}
  void play(const Ts &...x) override { element_->prev(); }

 protected:
  SequenceElement *element_;
};

}  // namespace esphome::elements
