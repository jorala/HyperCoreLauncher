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

## Credits

HyperCore Launcher is built on the work of the Prism Launcher, PolyMC and MultiMC contributors, whose copyright notices are preserved in the source files. Version metadata is currently served by the upstream Prism Launcher meta server.

## Forking/Redistributing/Custom builds policy

You are free to fork, redistribute and provide custom builds as long as you follow the terms of the [license](LICENSE) (this is a legal responsibility), and if you made code changes rather than just packaging a custom build, please do the following as a basic courtesy:

- Make it clear that your fork is not Prism Launcher and is not endorsed by or affiliated with the Prism Launcher project (<https://prismlauncher.org>).
- Go through [CMakeLists.txt](CMakeLists.txt) and change Prism Launcher's API keys to your own or set them to empty strings (`""`) to disable them (this way the program will still compile but the functionality requiring those keys will be disabled).

If you have any questions or want any clarification on the above conditions please make an issue and ask us.

If you are just building Prism Launcher for your distribution, please make sure to set the `Launcher_BUILD_PLATFORM` to a slug representing your distribution. Examples are `archlinux`, `fedora` and `nixpkgs`.

Note that if you build this software without removing the provided API keys in [CMakeLists.txt](CMakeLists.txt) you are accepting the following terms and conditions:

- [Microsoft Identity Platform Terms of Use](https://docs.microsoft.com/en-us/legal/microsoft-identity-platform/terms-of-use)
- [CurseForge 3rd Party API Terms and Conditions](https://support.curseforge.com/en/support/solutions/articles/9000207405-curse-forge-3rd-party-api-terms-and-conditions)

If you do not agree with these terms and conditions, then remove the associated API keys from the [CMakeLists.txt](CMakeLists.txt) file by setting them to an empty string (`""`).

## License [![https://github.com/PrismLauncher/PrismLauncher/blob/develop/LICENSE](https://img.shields.io/github/license/PrismLauncher/PrismLauncher?label=License&logo=gnu&color=C4282D)](LICENSE)

All launcher code is available under the GPL-3.0-only license.

The logo and related assets are under the CC BY-SA 4.0 license.
