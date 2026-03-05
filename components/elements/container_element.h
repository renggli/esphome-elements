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

// ---------------------------------------------------------------------------
// ContainerElement
// ---------------------------------------------------------------------------

/// Element that holds a list of child elements. Subclasses decide how many
/// are drawn and, via visit_children(), which ones are considered visible.
class ContainerElement : public Element {
 public:
  ContainerElement(ElementComponent *component, Element *parent, ActiveMode active_mode)
      : Element(component, parent), active_mode_(active_mode) {}

  void set_active_mode(ActiveMode active_mode) { active_mode_ = active_mode; };
  ActiveMode get_active_mode() { return active_mode_; }

  void add_element(Element *element);

  void dump_config(int level) override;

  bool is_active() const override;

  /// All children are visible — correct for Overlay, Horizontal, Vertical.
  void visit_children(const std::function<void(Element *, bool)> &fn) override;

  /// Propagates update_state() to all children before the container's own update.
  void update_state() override;

 protected:
  std::vector<Element *> elements_;
  ActiveMode active_mode_;
};

// ---------------------------------------------------------------------------
// SelectElement
// ---------------------------------------------------------------------------

/// Base class for containers that display exactly ONE child at a time.
/// Subclasses call go_to() / next() / prev() to change the selection.
/// visit_children() marks only the selected child as visible, so events
/// are dispatched correctly by the centralised ElementComponent machinery.
class SelectElement : public ContainerElement {
 public:
  SelectElement(ElementComponent *component, Element *parent, ActiveMode active_mode)
      : ContainerElement(component, parent, active_mode) {}

  bool has_visible_child(const Element *child) const { return index_ != -1 && elements_[index_] == child; }

  /// Only the selected child is visible.
  void visit_children(const std::function<void(Element *, bool)> &fn) override;

  void go_to(int index);
  void prev();
  void next();

 protected:
  int index_ = -1;
};

// ---------------------------------------------------------------------------
// OverlayElement
// ---------------------------------------------------------------------------

/// Draws multiple elements on top of each other (all visible simultaneously).
class OverlayElement : public ContainerElement {
 public:
  OverlayElement(ElementComponent *component, Element *parent) : ContainerElement(component, parent, ActiveMode::ANY) {}

  const char *get_type_name() const override { return "overlay"; }

  void draw(display::Display &display) override;
};

// ---------------------------------------------------------------------------
// PriorityElement
// ---------------------------------------------------------------------------

/// Draws the first active child element. Automatically updates on each frame.
class PriorityElement : public SelectElement {
 public:
  PriorityElement(ElementComponent *component, Element *parent) : SelectElement(component, parent, ActiveMode::ANY) {}

  const char *get_type_name() const override { return "priority"; }

  void draw(display::Display &display) override;

  /// Called each frame to re-evaluate which child has priority.
  void update_state() override;

 protected:
  int find_active_index_() const;
};

// ---------------------------------------------------------------------------
// HorizontalElement / VerticalElement
// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------
// RandomElement
// ---------------------------------------------------------------------------

/// Displays one child at a time, chosen randomly. Advances via next() / prev().
class RandomElement : public SelectElement {
 public:
  RandomElement(ElementComponent *component, Element *parent) : SelectElement(component, parent, ActiveMode::ANY) {}

  const char *get_type_name() const override { return "random"; }

  void draw(display::Display &display) override;
  void update_state() override;

  void prev();
  void next();

 protected:
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

// ---------------------------------------------------------------------------
// SequenceElement
// ---------------------------------------------------------------------------

/// Displays one child at a time in sequence. Advances via next() / prev().
class SequenceElement : public SelectElement {
 public:
  SequenceElement(ElementComponent *component, Element *parent) : SelectElement(component, parent, ActiveMode::ANY) {}

  const char *get_type_name() const override { return "sequence"; }

  void draw(display::Display &display) override;
  void update_state() override;

  void prev();
  void next();
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
