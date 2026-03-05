#pragma once

#include <deque>

#include "container_element.h"

namespace esphome::elements {

// ---------------------------------------------------------------------------
// SelectElement
// ---------------------------------------------------------------------------

/// Base class for containers that display exactly ONE child at a time.
/// Subclasses implement next() / prev() to advance the selection.
/// visit_children() marks only the selected child as visible, so on_show /
/// on_hide events are dispatched correctly by the ElementComponent machinery.
class SelectElement : public ContainerElement {
 public:
  SelectElement(ElementComponent *component, Element *parent, ActiveMode active_mode)
      : ContainerElement(component, parent, active_mode) {}

  /// Check if a specific child is currently visible.
  bool has_visible_child(const Element *child) const { return index_ != -1 && elements_[index_] == child; }

  /// Only the selected child is visible.
  void visit_children(const std::function<void(Element *, bool)> &fn) override;

  /// Go to an element with a specific index.
  void go_to(int index);

  /// Advance to the previous active child.
  virtual void prev() = 0;

  /// Advance to the next active child.
  virtual void next() = 0;

  // ---------------------------------------------------------------------------
  // on_change event
  // ---------------------------------------------------------------------------

  /// Register a callback for when the selection changes.
  /// Receives (from_index, from_element, to_index, to_element).
  /// Not fired during initial setup (when there is no previous selection).
  void add_on_change_callback(std::function<void(SelectElement *, int, int)> &&callback) {
    on_change_callbacks_.add(std::move(callback));
  }

  /// Called when the selection transitions from one child to another.
  void on_change(int from_index, int to_index);

 protected:
  int index_ = -1;
  LazyCallbackManager<void(SelectElement *, int, int)> on_change_callbacks_;
};

class SelectElementChangeTrigger : public Trigger<int, int> {
 public:
  explicit SelectElementChangeTrigger(SelectElement *parent) {
    parent->add_on_change_callback(
        [this](SelectElement * /*element*/, int from_index, int to_index) { trigger(from_index, to_index); });
  }
};

template<typename... Ts> class SelectNextAction : public Action<Ts...> {
 public:
  SelectNextAction(SelectElement *element) : element_(element) {}
  void play(const Ts &...x) override { element_->next(); }

 protected:
  SelectElement *element_;
};

template<typename... Ts> class SelectPrevAction : public Action<Ts...> {
 public:
  SelectPrevAction(SelectElement *element) : element_(element) {}
  void play(const Ts &...x) override { element_->prev(); }

 protected:
  SelectElement *element_;
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
  void update_state() override;

  void prev() override {}
  void next() override {}

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

  void prev() override;
  void next() override;

 protected:
  std::deque<int> history_;

  void update_history_();
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

  void prev() override;
  void next() override;
};

}  // namespace esphome::elements
