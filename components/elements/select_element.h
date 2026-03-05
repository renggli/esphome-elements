#pragma once

#include <deque>

#include "container_element.h"

namespace esphome::elements {

// ---------------------------------------------------------------------------
// SelectElement
// ---------------------------------------------------------------------------

/// Base class for containers that display exactly ONE child at a time.
/// Subclasses call go_to() to change the selection.
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

 protected:
  int index_ = -1;
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
