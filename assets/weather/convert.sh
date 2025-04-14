#!/bin/bash

# Process all SVG files.
for file in *.svg; do

  # Convert to PNG.
  inkscape \
    --export-width=32 \
    --export-height=32 \
    --export-background=black \
    --export-filename="${file%.svg}.png" \
    "$file"

  # Darken image.
  magick \
    "${file%.svg}.png" \
    -fill black \
    -colorize 30% \
    "${file%.svg}.png"

done
