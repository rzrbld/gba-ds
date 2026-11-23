# Duo Lite Linux builds

This fork keeps GBA DS as an independently installed app, outside BananaOS's
immutable firmware. The upstream project is <https://github.com/kyannon/gba-ds>.

## Install and update

In Slopetanium, add `rzrbld/gba-ds` as an **App** source and choose the release's
`gba-ds-v…-linux-aarch64.zip`. Keep the launcher selection automatic: the archive
contains one `GBA DS Duo.sh` entry. Ports continue to use PortMaster;
this app appears as **GBA DS Duo** in BananaOS's main menu. It uses a separate
package directory and tile from `kyannon/gba-ds`, so installing this fork does not
replace or modify that app. ROMs and saves still use BananaOS's shared folders.

The in-app updater defaults to this fork's releases. A user-supplied `update_url`
still overrides it. Do not switch back to the upstream Linux package until it
supports the device's CPU and panel orientation.

## Changes from upstream v1.6.1

- Linux builds use `-march=armv8-a -mtune=cortex-a55` for both the app and mGBA.
  Tuning does not require ARMv8.2/LSE instructions. The original v1.6 binary faults
  at `0x463960` (`ldaddal`); the Duo Lite reports no `atomics` CPU feature.
- `GBADS_DISPLAY_ROTATIONS=270,0` draws upright logical coordinates and rotates
  each requested panel at GPU presentation. BananaOS's native KMS launcher derives
  this from `BANANA_TOP_ROTATION` and `BANANA_BOTTOM_ROTATION`. Unset/zero retains
  the original rendering path. Android and compositor-managed displays do not get
  an automatic extra rotation.
- Rendering into intermediate textures restores the previous target, preserving
  the rotated panel when pixel shaders/scaling are enabled. Screenshots and
  window-local pointer coordinates use logical panel coordinates.

## Reproduce

Install the dependencies listed in `.github/workflows/linux.yml`, then run:

```sh
AARCH64_TOOLCHAIN_BIN=/usr/bin BUILD_JOBS=4 scripts/build-rocknix.sh
```

Actions builds PR/manual artifacts. A version tag such as `v1.6.1.1` publishes
that exact archive and its checksum as a release. Release assets are produced by
Actions, never uploaded from a local build. Match `VERSION` to the tag. The default
CPU flags can be overridden with `GBADS_CPU_FLAGS` for development; keep releases
on the portable default. The emulator dependency is pinned in
`scripts/mgba-options.sh`.

Fork revisions use the upstream four-number version format (`1.6.1.1`,
`1.6.1.2`, …), so the existing in-app update comparison orders them correctly.
