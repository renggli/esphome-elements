#pragma once

#include <vector>

#include "element.h"

namespace esphome::elements {

/// Strategy to infer the activeness of a container from its children.
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

  void set_active_mode(ActiveMode active_mode) { active_mode_ = active_mode; }
  ActiveMode get_active_mode() { return active_mode_; }

  void add_element(Element *element);

  void dump_config(int level) override;

  bool is_active() const override;

  /// All children are visible — correct for Overlay, Horizontal, Vertical.
  void update_visibility(bool now_visible) override;

  /// Propagates update_state() to all children before the container's own update.
  void update_state() override;

 protected:
  std::vector<Element *> elements_;
  ActiveMode active_mode_;
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

}  // namespace esphome::elements
