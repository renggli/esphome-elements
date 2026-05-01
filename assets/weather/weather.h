#pragma once
#include "esphome.h"

extern image::Image* const clear_night;
extern image::Image* const cloudy;
extern image::Image* const fog;
extern image::Image* const hail;
extern image::Image* const lightning_rainy;
extern image::Image* const lightning;
extern image::Image* const partlycloudy_night;
extern image::Image* const partlycloudy;
extern image::Image* const pouring;
extern image::Image* const rainy;
extern image::Image* const snowy_rainy;
extern image::Image* const snowy;
extern image::Image* const sunny;
extern image::Image* const windy_variant;
extern image::Image* const windy;

inline void set_weather_image(
    esphome::elements::ImageElement* const image_element, std::string& state) {
  if (state == "clear-night") {
    image_element->set_image(clear_night);
  } else if (state == "cloudy") {
    image_element->set_image(cloudy);
  } else if (state == "fog") {
    image_element->set_image(fog);
  } else if (state == "hail") {
    image_element->set_image(hail);
  } else if (state == "lightning-rainy") {
    image_element->set_image(lightning_rainy);
  } else if (state == "lightning") {
    image_element->set_image(lightning);
  } else if (state == "partlycloudy-night") {
    image_element->set_image(partlycloudy_night);
  } else if (state == "partlycloudy") {
    image_element->set_image(partlycloudy);
  } else if (state == "pouring") {
    image_element->set_image(pouring);
  } else if (state == "rainy") {
    image_element->set_image(rainy);
  } else if (state == "snowy") {
    image_element->set_image(snowy);
  } else if (state == "snowy-rainy") {
    image_element->set_image(snowy_rainy);
  } else if (state == "sunny") {
    image_element->set_image(sunny);
  } else if (state == "windy-variant") {
    image_element->set_image(windy_variant);
  } else if (state == "windy") {
    image_element->set_image(windy);
  } else {
    image_element->clear_image();
  }
}
