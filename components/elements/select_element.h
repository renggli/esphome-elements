#pragma once

#include <deque>
#include <vector>

#include "element.h"

namespace esphome::elements {

/// Base class for containers that display exactly one child at a time.
class SelectElement : public Element {
 public:
  SelectElement(ElementComponent* component, Element* parent)
      : Element(component, parent) {}

  void dump_config(int level) override;

  void add_element(Element* element);

  bool is_active() override;
  void update_state() override;
  void update_visibility(bool now_visible) override;

  /// Return the index of an element, or -1 if not found.
  int index_of(const Element* element) const;

  /// Go to an element with a specific index.
  void go_to(int index);

  /// Go to a specific element.
  void go_to(const Element* element);

  /// Advance to the previous active child.
  virtual void prev() = 0;

  /// Advance to the next active child.
  virtual void next() = 0;

  /// Register a callback for when the selection changes.
  /// Receives (from_index, from_element, to_index, to_element).
  /// Not fired during initial setup (when there is no previous selection).
  void add_on_change_callback(
      std::function<void(SelectElement*, int, int)>&& callback) {
    on_change_callbacks_.add(std::move(callback));
  }

  /// Called when the selection transitions from one child to another.
  void on_change(int from_index, int to_index);

 protected:
  std::vector<Element*> elements_;
  int index_ = -1;
  LazyCallbackManager<void(SelectElement*, int, int)> on_change_callbacks_;
};

class SelectElementChangeTrigger : public Trigger<SelectElement&, int, int> {
 public:
  explicit SelectElementChangeTrigger(SelectElement* parent) {
    parent->add_on_change_callback(
        [this](SelectElement* element, int from_index, int to_index) {
          trigger(*element, from_index, to_index);
        });
  }
};

template <typename... Ts>
class SelectNextAction : public Action<Ts...> {
 public:
  SelectNextAction(SelectElement* element) : element_(element) {}
  void play(const Ts&... x) override { element_->next(); }

 protected:
  SelectElement* element_;
};

template <typename... Ts>
class SelectPrevAction : public Action<Ts...> {
 public:
  SelectPrevAction(SelectElement* element) : element_(element) {}
  void play(const Ts&... x) override { element_->prev(); }

 protected:
  SelectElement* element_;
};

template <typename... Ts>
class SelectGoToAction : public Action<Ts...> {
 public:
  SelectGoToAction(SelectElement* element) : element_(element) {}
  TEMPLATABLE_VALUE(int, index)
  void set_element(Element* element) { target_element_ = element; }

  void play(const Ts&... x) override {
    if (target_element_ != nullptr) {
      element_->go_to(target_element_);
    } else if (this->index_.has_value()) {
      element_->go_to(this->index_.value(x...));
    }
  }

 protected:
  SelectElement* element_;
  Element* target_element_{nullptr};
};

// ---------------------------------------------------------------------------
// PriorityElement
// ---------------------------------------------------------------------------

/// Draws the first active child element. Automatically updates on each frame.
class PriorityElement : public SelectElement {
 public:
  PriorityElement(ElementComponent* component, Element* parent)
      : SelectElement(component, parent) {}

  const char* get_type_name() const override { return "priority"; }

  void draw(display::Display& display) override;
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
  RandomElement(ElementComponent* component, Element* parent)
      : SelectElement(component, parent) {}

  const char* get_type_name() const override { return "random"; }

  void draw(display::Display& display) override;
  void update_state() override;

  void prev() override;
  void next() override;

  void push(int index);
  void push(Element* element);
  void clear_queue();
  bool has_queue() const { return !queue_.empty(); }
  size_t queue_size() const { return queue_.size(); }

 protected:
  std::deque<int> history_;
  std::deque<int> queue_;

  void update_history_();
};

template <typename... Ts>
class RandomPushAction : public Action<Ts...> {
 public:
  RandomPushAction(RandomElement* element) : element_(element) {}
  void set_clear(bool clear) { clear_ = clear; }
  void set_advance(bool advance) { advance_ = advance; }
  void set_elements(const std::vector<Element*>& elements) {
    elements_ = elements;
  }
  void add_element(Element* element) { elements_.push_back(element); }

  void play(const Ts&... x) override {
    if (clear_) {
      element_->clear_queue();
    }
    for (Element* elem : elements_) {
      element_->push(elem);
    }
    if (advance_) {
      element_->next();
    }
  }

 protected:
  RandomElement* element_;
  bool clear_{false};
  bool advance_{false};
  std::vector<Element*> elements_;
};

template <typename... Ts>
class RandomClearQueueAction : public Action<Ts...> {
 public:
  RandomClearQueueAction(RandomElement* element) : element_(element) {}

  void play(const Ts&... x) override { element_->clear_queue(); }

 protected:
  RandomElement* element_;
};

// ---------------------------------------------------------------------------
// SequenceElement
// ---------------------------------------------------------------------------

/// Displays one child at a time in sequence. Advances via next() / prev().
class SequenceElement : public SelectElement {
 public:
  SequenceElement(ElementComponent* component, Element* parent)
      : SelectElement(component, parent) {}

  const char* get_type_name() const override { return "sequence"; }

  void draw(display::Display& display) override;
  void update_state() override;

  void prev() override;
  void next() override;
};

}  // namespace esphome::elements
