# KuyoSynthe

KuyoSynthe is toy synthesizer for Godot using GDSYNTHESIZER.

 ![kuyosynthe_image.png](kuyosynthe_image.png "KuyoSynthe")

## What is GDSYNTHESIZER?

KuyoSynthe is an implementation sample of GDSYNTHESIZSE using Godot script.
KuyoSynthe is the way to know how to use GDSYNTHESIZER.

KuyoSynthe is:
 - an implementation sample of GDSYNTHESIZSE using Godot script
 - the way to know how to use GDSYNTHESIZER
 - tone editor for each musical instruments of midi.
 - sequence player for SMF files

KuyoSynthe has been confirmed to work as a desktop app on Windows.
Also, KuyoSynthe has been confirmed to work as a WEB (HTML) application for Windows PCs.
Operation on other platforms has not been confirmed.
Target Godot Version is ver.4.2.1.

This project is just a hobby of the author and may not necessarily work the way you want it to.

## How to install

KuyoSynthe requires the GDSYNTHESIZSER runtime library.
GDSYNTHESIZSER is published on the following github.
https://github.com/soyokuyo/gdsynthesizer
 Build this and place the files generated in "project/bin" in the res:// directory of the godot project.
A precompiled runtime library for GDSYNTHESIZER may be made available in the near future.

## How to use

Each parameter can be changed using the dial knob. Change the dial knob by hovering over the mouse cursor and using the wheel.
Put your favorite SMF file in the "res://" directory. Then press the "select SMF" button to select the file and play it.



