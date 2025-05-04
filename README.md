# ESPHome Elements

[![Build Status](https://github.com/renggli/esphome-elements/actions/workflows/ci.yml/badge.svg)](https://github.com/renggli/esphome-elements/actions/workflows/ci.yml)
[![GitHub Issues](https://img.shields.io/github/issues/renggli/esphome-elements.svg)](https://github.com/renggli/esphome-elements/issues)
[![GitHub Forks](https://img.shields.io/github/forks/renggli/esphome-elements.svg)](https://github.com/renggli/esphome-elements/network)
[![GitHub Stars](https://img.shields.io/github/stars/renggli/esphome-elements.svg)](https://github.com/renggli/esphome-elements/stargazers)
[![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/renggli/esphome-elements/main/LICENSE)

Personalize your LED matrix with [ESPHome](https://esphome.io) Elements, a custom component to easily combine text, images, and custom visualizations.

## Basic Setup

To integrate _ESPHome Elements_ into your project, simply add the following to your ESPHome configuration file:

```yaml
external_components:
  - source: github://renggli/esphome-elements
```

Next, define a [Addressable Light Component](https://esphome.io/components/light/index.html) that is connected to an [Addressable Light Display](https://esphome.io/components/display/addressable_light.html). Configure both parts according to your display hardware. You do not need to define a `lambda` for rendering, as _ESPHome Elements_ will manage the display updates.

```yaml
light:
  # configuration of addressable light component
  - id: led_light
    ...

display:
  # configuration of addressable light display
  - platform: addressable_light
    id: led_display
    addressable_light_id: led_light
    ...
```

Finally, define the `elements` configuration block. This is where you'll bring your display to life. Here's an example showing a default animation of the [custom element](#custom):

```yaml
elements:
  id: elements_display
  display: led_display
  element:
    - type: custom
```

[![ESPHome Elements Lissajous Animation](assets/lissajous.png)](https://www.youtube.com/shorts/Ih7tS33tRmk)

For a complete configuration example demonstrating various features, refer to [example.yaml](example.yaml).

## Elements

_ESPHome Elements_ provides a variety of configurable elements to dynamically compose and sequence content on your LED matrix, allowing for sophisticated displays with text, images, and advanced visualizations.

### Text Elements

_Displays textual information, labels, or dynamic messages._

There are three text elements with the following specific configuration variables:

1. `static_text` displays a fixed string:
   - **text** (Required, string): The string to be displayed. This string remains constant and does not change dynamically.
2. `dynamic_text` uses a lambda function to generate the string
   - **lambda** (Required, lambda): A lambda function that returns the string to be displayed. This allows the text to update dynamically based on sensor values, variables, or other changing conditions. The lambda function should return a value of type string, possibly an empty string.
3. `time_text` uses a format string to display date and time
   - **time** (Required, ID): The ID of a [time component](https://esphome.io/components/time/index.html). This specifies the source of the time for the clock.
   - **format** (Required, string): A format string that defines how the date and time should be displayed. This string uses format specifiers (e.g., `%Y` for year, `%m` for month, `%H` for hour) as defined by the C `strftime` function.

All text elements support the following configuration variables:

- **font** (Required, ID): The ID of the font to use for rendering the text. This links to a font definition within your ESPHome configuration, allowing you to customize the text's appearance.
- **color** (Optional, ID or #rrggbb): The color of the text specified either as an ID reference or as a hexadecimal color code. Defaults to white if not specified.
- **background_color** (Optional, ID or #rrggbb): The background color of the text. Similar to color, this can be an ID reference or a hexadecimal color code. If not specified, the background is transparent.
- **anchor** (Optional, Anchor): Specifies the reference point of the text box with an absolute `offset` and/or a relative `fraction`. This is used in conjunction with align to position the text.
- **align** (Optional, enum): Specifies how the text is aligned at the point defined by the anchor. Possible values are
  `TOP`, `CENTER_VERTICAL`, `BASELINE`, `BOTTOM`, `LEFT`, `CENTER_HORIZONTAL`, `RIGHT`, `TOP_LEFT`, `TOP_CENTER`, `TOP_RIGHT`, `CENTER_LEFT`, `CENTER` (default), `CENTER_RIGHT`, `BASELINE_LEFT`, `BASELINE_CENTER`, `BASELINE_RIGHT`, `BOTTOM_LEFT`, `BOTTOM_CENTER`, and `BOTTOM_RIGHT`.
- **scroll_mode** (Optional, enum): Specifies the scrolling behavior of the text, if any. Useful for long texts that exceed the display area. Possible values are `NONE` (default), `LEFT_TO_RIGHT`, `RIGHT_TO_LEFT`, `BOTTOM_TO_TOP`, and `TOP_TO_BOTTOM`. If scrolling is enabled, an `on_next` event is sent to the parent element on completion of the animation.
- **scroll_speed** (Optional, float): Specifies the speed of the scrolling animation in pixel per second.

The following example scrolls the string "Hello World" over the display:

```yaml
type: static_text
font: font_chunky_8
scroll_mode: LEFT_TO_RIGHT
text: "Hello World"
```

The following example displays a sensor value:

```yaml
type: dynamic_text
font: font_chunky_8
lambda: |-
  if (id(outside_temperature).has_state()) {
    float temp = id(outside_temperature).state;
    return str_snprintf("%.1f °C", 12, temp);
  }
  return "";
```

The following example displays the current time:

```yaml
type: time_text
time: current_time
font: font_chunky_8
format: "%H:%M:%S"
```

### Image Element

Shows static or animated images on the display.

The following configuration variables are supported:

- **image** (Required, ID): The ID of the image to display. This references an image definition in your configuration.
- **anchor** (Optional, Anchor): Specifies the reference point of the image with an absolute `offset` and/or a relative `fraction`. This is used in conjunction with align to position the image.
- **align** (Optional, enum): Specifies how the image is aligned relative to its anchor point. This determines where the rest of the image is positioned, given the anchor's location. Possible values are `TOP`, `CENTER_VERTICAL`, `BOTTOM`, `LEFT`, `CENTER_HORIZONTAL`, `RIGHT`, `TOP_LEFT`, `TOP_CENTER`, `TOP_RIGHT`, `CENTER_LEFT`, `CENTER` (default), `CENTER_RIGHT`, `BOTTOM_LEFT`, `BOTTOM_CENTER`, `BOTTOM_RIGHT`, `HORIZONTAL_ALIGNMENT`, and `VERTICAL_ALIGNMENT`.

The following example displays the image of a cat:

```yaml
type: image
image: cat
```

### Clock Element

Shows a configurable analog clock on the display.

The following configuration variables are supported:

- **time** (Required, ID): The ID of a [time component](https://esphome.io/components/time/index.html). This specifies the source of the time for the clock.
- **minute_markers** (Optional, AnalogClockOptions): Configuration options for the minute markers. This allows you to customize the appearance of the small lines or dots that indicate the minutes on the clock face.
- **hour_markers** (Optional, AnalogClockOptions): Configuration options for the hour markers. This allows you to customize the appearance of the lines or numbers that indicate the hours on the clock face.
- **quarter_markers** (Optional, AnalogClockOptions): Configuration options for the quarter-hour markers. This allows you to customize the appearance of the markers that indicate the 15-minute intervals.
- **second_hand** (Optional, AnalogClockOptions): Configuration options for the second hand.
- **minute_hand** (Optional, AnalogClockOptions): Configuration options for the minute hand.
- **hour_hand** (Optional, AnalogClockOptions): Configuration options for the hour hand.

For each of the `AnalogClockOptions` the following fields can be defined:

- **start** (Optional, float): The starting position of the marker or hand as a fraction of the clock radius. A value of 0 indicates the center of the clock, and 1 indicates the edge.
- **end** (Optional, float): The ending position of the marker or hand, expressed as a fraction of the clock radius, using the same scale as start.
- **color** (Optional, ID or #rrggbb): The color of the marker or hand. This can be specified as an ID reference to a color defined elsewhere or as a hexadecimal color code.
- **visible** (Optional, boolean): Whether the marker or hand is visible. If set to `false`, the marker or hand will not be displayed.
- **smooth** (Optional, boolean): Whether the hand moves smoothly. If set to `false`, the hand steps to the next increment.

The following example configures the default clock settings. Use this as a template to create your own unique clock face:

```yaml
type: clock
time: current_time
minute_markers:
  start: 0.95
  end: 1.00
  color: "#0000ff"
  visible: false
hour_markers:
  start: 0.90
  end: 1.00
  color: "#0000ff"
  visible: true
quarter_markers:
  start: 0.75
  end: 1.00
  color: "#0000ff"
  visible: true
second_hand:
  start: 0.00
  end: 0.75
  color: "#ff0000"
  visible: true
  smooth: true
minute_hand:
  start: 0.00
  end: 0.95
  color: "#ffffff"
  visible: true
  smooth: false
hour_hand:
  start: 0.00
  end: 0.66
  color: "#ffffff"
  visible: true
  smooth: true
```

### Custom Element

Allows for user-defined, highly specific visual elements or behaviors beyond the built-in options.

The following configuration variables are supported:

- **draw** (Optional, lambda): A lambda function that defines how the element is rendered on the display. This is the core of a custom element, allowing you to draw anything you want using the [Display](https://esphome.io/components/display/index.html#display-rendering-engine) drawing primitives. The variable `element` refers to the custom element, `display` to the render display. If the lambda is not set, the [default lissajous animation](#basic-setup) is display.
- **on_show** (Optional, lambda): A lambda function that is executed when the element becomes visible on the display. This is useful for performing setup tasks, starting animations, or loading data. The variable `element` refers to the custom element.
- **on_hide** (Optional, lambda): A lambda function that is executed when the element is no longer visible on the display. This can be used to clean up resources, stop animations, or save data. The variable `element` refers to the custom element.
- **on_next** (Optional, lambda): A lambda function that is executed when the element should transition to its next state or frame in a sequence. The variable `element` refers to the custom element.
- **is_active** (Optional, lambda): A lambda that returns a boolean which determines whether the element is currently active and should be displayed. The variable `element` refers to the custom element.

The following example displays a yellow circle when the weather is sunny, and otherwise a blue rectangle:

```yaml
type: custom
is_active: |-
  return id(outside_weather).has_state();
draw: |-
  auto state = id(outside_weather).state;
  if (state == "sunny") {
    display.filled_circle(16, 16, 8, Color(0xffff00));
  } else {
    display.filled_rectangle(8, 8, 16, 16, Color(0x0000ff));
  }
```

### Composition Elements

Arranges multiple elements within the display area to create a cohesive visual layout.

1. `horizontal` displays its children evenly spaced horizontally from left to right.
2. `vertical` displays its children evenly spaced horizontally from top to bottom.
3. `overlay` displays its children on top of each other.

The following configuration variables are supported:

- **active_mode** (Optional, enum): Specifies how the activity state of the children should propagate to the container. Possible values are:
  - `ALWAYS`: the container is always active;
  - `ANY`: the container is active, if any of its children are (default for `overlay`, `priority`, and `sequence`);
  - `ALL`: the container is active, if all of its children are (default for `horizontal` and `vertical`); and
  - `NEVER`: the container is never active.
- **elements** (Required, Array&lt;Element&gt;): A list of child elements in the desired drawing order.

The following example displays the string "Time" above the current time:

```yaml
type: vertical
elements:
  - type: static_text
    font: font_chunky_8
    text: "Time"
  - type: time_text
    time: current_time
    font: font_chunky_8
    format: "%H:%M:%S"
```

### Sequence Elements

This category of elements governs how child elements are displayed over time, allowing for dynamic and interactive displays.

- `priority`: This element displays the first child element within its list that is currently determined to be active based on its individual `is_active` state.
- `sequence`: This element presents its child elements in a defined order. It progresses to the next active child element upon receiving an `on_next` call. Additionally, it can jump to a specific child element when a `go_to` action with the target index is invoked.

The same configuration variables as [Composition Elements](#composition-elements) are supported.

### Delegate Elements

Changes the default behavior of elements by wrapping them.

- `delay`: This element generates a _next_ event after observing a specified count of _next_ events from its child element. Resets the counter when being shown.
  - **count** (Optional, int): The number of events before the _next_ event should be triggered. If unset, swallow all _next_ events from its child element.
- `timeout`: This element generates a _next_ event after a specified timeout. It passes through _next_ events from its child element. Resets the timer when being shown.
  - **duration** (Optional, duration): The time after which a _next_ event should be triggered.

The following configuration variable is supported:

- **element** (Required, Element): A single child element.
