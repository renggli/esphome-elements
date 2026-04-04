#!/bin/bash

# Variables
BASE="assets/weather"
YAML_FILE="$BASE/weather.yaml"

# Prepare the header of the yaml package.
echo "image:" > $YAML_FILE
echo "  rgb:" >> $YAML_FILE

# Process all SVG files.
for FILE in $BASE/*.svg; do
  SVG_FILE="${FILE}"
  PNG_FILE="${FILE%.svg}.png"
  PNG_ID=$(basename "${FILE%.svg}" | tr '-' '_')

  # Convert to PNG.
  inkscape \
    --export-width=32 \
    --export-height=32 \
    --export-background=black \
    --export-filename="${PNG_FILE}" \
    "${SVG_FILE}"

  # Darken image.
  magick \
    "${PNG_FILE}" \
    -fill black \
    -trim +repage \
    -colorize 25% \
    "${PNG_FILE}"

  # Write config.
  echo "  - id: ${PNG_ID}" >> $YAML_FILE
  echo "    file: ${PNG_FILE}" >> $YAML_FILE

done
