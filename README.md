# ESPHome Elements

[![Build Status](https://github.com/renggli/esphome-elements/actions/workflows/ci.yml/badge.svg)](https://github.com/renggli/esphome-elements/actions/workflows/ci.yml)
[![GitHub Issues](https://img.shields.io/github/issues/renggli/esphome-elements.svg)](https://github.com/renggli/esphome-elements/issues)
[![GitHub Forks](https://img.shields.io/github/forks/renggli/esphome-elements.svg)](https://github.com/renggli/esphome-elements/network)
[![GitHub Stars](https://img.shields.io/github/stars/renggli/esphome-elements.svg)](https://github.com/renggli/esphome-elements/stargazers)
[![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/renggli/esphome-elements/main/LICENSE)

Control and personalize your LED matrix display with [ESPHome](https://esphome.io) Elements, a custom component for configuring dynamic text, images, animations, and more.

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

Finally, define the `elements` configuration block. This is where you'll bring your display to life. Here's an example showing the default animation:

```yaml
elements:
  id: elements_display
  display: led_display
  element:
    - type: custom
```

[![ESPHome Elements Lissajous Animation](assets/lissajous.png)](https://www.youtube.com/shorts/Ih7tS33tRmk)

For a complete configuration example demonstrating various features, refer to the [example.yaml](example.yaml) file.

## Elements Documentation

_ESPHome Elements_ provides a variety of configurable elements to dynamically compose and sequence content on your LED matrix, allowing for sophisticated displays with text, images, and advanced visualizations.

### Text

_Displays textual information, labels, or dynamic messages._

There are three text elements with the following specific configuration variables:

1. `static_text` displays a fixed string:
   - **text** (Required, string): The string to be displayed. This string remains constant and does not change dynamically.
2. `dynamic_text` uses a lambda function to generate the string
   - **lambda** (Required, lambda): A lambda function that returns the string to be displayed. This allows the text to update dynamically based on sensor values, variables, or other changing conditions. The lambda function should return a value of type string, possibly an empty string.
3. `time_text` uses a format string to display date and time
   - **time** (Required, ID): The ID of a time provider component. This specifies the source of the date/time information. Typically, this would be a time component like SNTP.
   - **format** (Required, string): A format string that defines how the date and time should be displayed. This string uses format specifiers (e.g., `%Y` for year, `%m` for month, `%H` for hour) as defined by the C `strftime` function.

All text elements support the following configuration variables:

- **font** (Required, ID): The ID of the font to use for rendering the text. This links to a font definition within your ESPHome configuration, allowing you to customize the text's appearance.
- **color** (Optional, ID or #rrggbb): The color of the text specified either as an ID reference or as a hexadecimal color code. Defaults to white if not specified.
- **background_color** (Optional, ID or #rrggbb): The background color of the text. Similar to color, this can be an ID reference or a hexadecimal color code. If not specified, the background is transparent.
- **anchor** (Optional, Anchor): Specifies the reference point of the text box with an absolute `offset` and/or a relative `fraction`. This is used in conjunction with align to position the text.
- **align** (Optional, enum): Specifies how the text is aligned at the point defined by the anchor. Possible values are
  `TOP`, `CENTER_VERTICAL`, `BASELINE`, `BOTTOM`, `LEFT`, `CENTER_HORIZONTAL`, `RIGHT`, `TOP_LEFT`, `TOP_CENTER`, `TOP_RIGHT`, `CENTER_LEFT`, `CENTER` (default), `CENTER_RIGHT`, `BASELINE_LEFT`, `BASELINE_CENTER`, `BASELINE_RIGHT`, `BOTTOM_LEFT`, `BOTTOM_CENTER`, and `BOTTOM_RIGHT`.
- **scroll_mode** (Optional, enum): Specifies the scrolling behavior of the text, if any. Useful for long texts that exceed the display area. Possible values are `NONE` (default), `LEFT_TO_RIGHT`, `RIGHT_TO_LEFT`, `BOTTOM_TO_TOP`, and `TOP_TO_BOTTOM`.
- **scroll_speed** (Optional, float): Specifies the speed of the scrolling animation in pixel per second.

The following example scrolls the string "Hello World" over the display:

```yaml
- type: static_text
  font: font_chunky_8
  scroll_mode: LEFT_TO_RIGHT
  text: "Hello World"
```

The following example displays a sensor value:

```yaml
- type: dynamic_text
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
- type: time_text
  time: current_time
  font: font_chunky_8
  format: "%H:%M:%S"
```

### Image

Shows static or animated images on the display.

The following configuration variables are supported:

- **image** (Required, ID): The ID of the image to display. This references an image definition in your configuration.
- **anchor** (Optional, Anchor): Specifies the reference point of the image with an absolute `offset` and/or a relative `fraction`. This is used in conjunction with align to position the image.
- **align** (Optional, enum): Specifies how the image is aligned relative to its anchor point. This determines where the rest of the image is positioned, given the anchor's location. Possible values are `TOP`, `CENTER_VERTICAL`, `BOTTOM`, `LEFT`, `CENTER_HORIZONTAL`, `RIGHT`, `TOP_LEFT`, `TOP_CENTER`, `TOP_RIGHT`, `CENTER_LEFT`, `CENTER` (default), `CENTER_RIGHT`, `BOTTOM_LEFT`, `BOTTOM_CENTER`, `BOTTOM_RIGHT`, `HORIZONTAL_ALIGNMENT`, and `VERTICAL_ALIGNMENT`.

The following example displays the image of a cat:

```yaml
- type: image
  image: cat
```

### Custom

Allows for user-defined, highly specific visual elements or behaviors beyond the built-in options.

### Composition

Arranges multiple elements within the display area to create a cohesive visual layout.

### Sequencing

Controls the order and timing of how elements appear or change on the display over time.

### Decorating

Changes the default behavior of elements by wrapping them.
