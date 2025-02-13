# defaultVideoProject

A good place to start developing video synthesis/processing projects on VSERPI

This sample project has code block examples for midi integration with mappings for the KORG nanoKontrol

![Control Scheme](https://github.com/PeteHaughie/defaultVideoProject/blob/main/vserpi-midi-ccs.png?raw=true)

This project has examples for the rotary encoders 16 – 23, and the sliders 120 – 127.

Because desktop and RPi report their input devices differently there is a built-in variable to switch them. `0` will cycle through available video input devices and `9` will cycle through available midi devices.

## Control Scheme

Refer to the image above for details about the mapping of #cc numbers to the physical controls.

- Rotary encoder 1 `#cc 16` - variable `sx` controls the horizontal position of the video
- Slider 9 `#cc 120` - variable `dc` controls the vertical position of the video
- Button `#cc 61` - variable `mode` cycles through modes
- Button `#cc 62` - variable `hdAspectRatioSwitch` changes the input aspect ratio

