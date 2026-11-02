<p align="center">
<picture>
  <source media="(prefers-color-scheme: dark)" srcset="/program_info/io.github.jorala.HyperCoreLauncher.logo-darkmode.svg">
  <source media="(prefers-color-scheme: light)" srcset="/program_info/io.github.jorala.HyperCoreLauncher.logo.svg">
  <img alt="HyperCore Launcher" src="/program_info/io.github.jorala.HyperCoreLauncher.logo.svg" width="40%">
</picture>
</p>

<p align="center">
  HyperCore Launcher is a custom launcher for Minecraft that lets you easily manage multiple installations of Minecraft at once, with a modernized interface and a deeply customizable system.<br />
  <br />HyperCore Launcher is a <b>fork</b> of <a href="https://github.com/PrismLauncher/PrismLauncher">Prism Launcher</a> (itself a fork of PolyMC, which was forked from MultiMC) and is <b>not</b> endorsed by those projects.
</p>

## Status

HyperCore Launcher is in early development. Test builds are produced for **Windows** and **macOS** (Linux is not supported yet).

## Installation

- Test builds are available from the [GitHub Actions](https://github.com/jorala/HyperCoreLauncher/actions) tab and, once released, from the [Releases](https://github.com/jorala/HyperCoreLauncher/releases) page.
- Development builds include debug information, so their file sizes are relatively large. Expect bugs and instability.

## Feedback & Support

Found a bug or have an idea? Please [open an issue](https://github.com/jorala/HyperCoreLauncher/issues).

## Building

HyperCore Launcher is written in C++ with Qt 6 and built with CMake. The build setup is inherited from Prism Launcher; see `CMakePresets.json` and the workflows in `.github/` for how Windows and macOS builds are produced.

## Credits and License

HyperCore Launcher is licensed under the GPL-3.0 (see `COPYING.md`). It is built on the work of the Prism Launcher, PolyMC and MultiMC contributors, whose copyright notices are preserved in the source files. Version metadata is currently served by the upstream Prism Launcher meta server.
