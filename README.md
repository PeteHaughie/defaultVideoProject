# defaultVideoProject

A good place to start developing video synthesis/processing projects on VSERPI

This sample project has code block examples for midi integration with mappings for the KORG nanoKontrol

![Control Scheme](vserpi-midi-css.png)

This project has examples for the rotary encoders 16 – 23, and the sliders 120 – 127.

## Control Scheme

Refer to the image above for details about the mapping of #cc numbers to the physical controls.

- Rotary encoder 1 (`#cc 16`) - variable `sx` controls the horizontal position of the video
- Slider 9 (`#cc 120`) - variable `dc` controls the vertical position of the video
- Button `#cc 61` - variable `mode` cycles through modes
- Button `#cc 62` - variable `hdAspectRatioSwitch` changes the input aspect ratio

## Extras

These are mapped wierdly but it's just to show the way to map an oscillator to a parameter

- Rotary encoder 2 (`#cc 17`) - variable `osc1` controls the `sx` variance
- Slider 10 (`#cc 121`) - variable `osc2` controls the `sy` variance