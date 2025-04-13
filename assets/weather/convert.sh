#!/bin/bash

# Convert all SVGs to PNGs using Inkscape.
for file in *.svg; do
  inkscape \
    --export-width=32 \
    --export-height=32 \
    --export-background=black \
    --export-filename="${file%.svg}.png" \
    "$file"
done
