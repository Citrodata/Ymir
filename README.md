# Ymir

[![Release](https://github.com/StrikerX3/Ymir/actions/workflows/release.yaml/badge.svg)](https://github.com/StrikerX3/Ymir/actions/workflows/release.yaml) <a href="https://discord.gg/NN3A7n5dzn">![Discord Shield](https://discord.com/api/guilds/1368676375627694341/widget.png?style=shield)</a> <a href="https://patreon.com/StrikerX3">![Patreon Shield](https://img.shields.io/badge/Patreon-F96854?style=flat&logo=patreon&logoColor=white)</a>

A work-in-progress Sega Saturn emulator.

Join the [Discord community](https://discord.gg/NN3A7n5dzn).

Find the official compatibility list [here](https://docs.google.com/spreadsheets/d/1SLZzL9LelSlpEmTKy8cjaQnE7mew2uW1rfCgcekO58Q/edit?usp=sharing).

<div class="grid" markdown>
  <img width="49.5%" src="https://github.com/StrikerX3/Ymir/blob/main/docs/images/cd-player.png"/>
  <img width="49.5%" src="https://github.com/StrikerX3/Ymir/blob/main/docs/images/sonic-r.png"/>
  <img width="49.5%" src="https://github.com/StrikerX3/Ymir/blob/main/docs/images/virtua-fighter-2.png"/>
  <img width="49.5%" src="https://github.com/StrikerX3/Ymir/blob/main/docs/images/radiant-silvergun.png"/>
  <img width="49.5%" src="https://github.com/StrikerX3/Ymir/blob/main/docs/images/panzer-dragoon-saga.png"/>
  <img width="49.5%" src="https://github.com/StrikerX3/Ymir/blob/main/docs/images/nights-into-dreams.png"/>
  <img width="100%" src="https://github.com/StrikerX3/Ymir/blob/main/docs/images/debugger.png"/>
</div>


## Features

- Load games from MAME CHD, BIN+CUE, IMG+CCD, MDF+MDS or ISO files
- Automatic IPL (BIOS) ROM detection
- Automatic region switching
- Up to two players with standard Control Pads or 3D Control Pads on both ports (more to come)
- Fully customizable keybindings
- Backup RAM, DRAM and ROM cartridges (more to come)
- Integrated backup memory manager to import and export saves, and transfer between internal and cartridge RAM
- Save states
- Rewinding (up to one minute at 60 fps), turbo speed, frame step (forwards and backwards)
- Full screen mode with VRR support and low input lag
- Optional deinterlaced/progressive rendering of high resolution modes
- A work-in-progress feature-rich debugger


## Usage

Grab the latest release [here](https://github.com/StrikerX3/Ymir/releases/latest).
Check the [Releases](https://github.com/StrikerX3/Ymir/releases) page for previous versions.

Ymir does not require installation. Simply download it to any directory and run the executable.
On Windows you might also need to install the latest [Microsoft Visual C++ Redistributable package](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist) ([x86_64 installer](https://aka.ms/vs/17/release/vc_redist.x64.exe)).

The program accepts command-line arguments. Invoke `ymir-sdl3 --help` to list the options:

```
Ymir - Sega Saturn emulator
Usage:
  Ymir [OPTION...] positional parameters

  -p, --profile arg  Path to profile directory
  -h, --help         Display help text
  -f, --fullscreen   Start in fullscreen mode
  -P, --paused       Start paused
```

Use `-p <profile-path>` to point to a separate set of configuration and state files, useful if you wish to have different user profiles (hence the name).

The options are case-sensitive -- lowercase `-p` sets the profile path, uppercase `-P` makes the emulator start paused.

Note that the Windows version does not output anything to the console, but it does honor the command line parameters.

Ymir requires an IPL (BIOS) ROM to work. You can place the ROMs under the `roms` directory created alongside the executable on the first run.
The emulator will scan and automatically select the IPL ROM matching the loaded disc. If no disc is loaded, it will use a ROM matching the first preferred region. Failing that, it will pick whatever is available.
You can override the selection on Settings > IPL.

Ymir can load game disc images from MAME CHD, BIN+CUE, IMG+CCD, MDF+MDS or ISO files. It does not support injecting .elf files directly at the moment.


## Compiling

See [COMPILING.md](COMPILING.md).


## Support my work

If you enjoy my projects and want to help me keep developing them, consider supporting me:
- [Patreon](https://www.patreon.com/StrikerX3) for ongoing support
- PIX for one-time donations in Brazil: ask me on Discord.

Your support is completely optional but genuinely appreciated. It helps me dedicate more time and energy to these passion projects while keeping everything open-source and free for everyone. Thank you!
