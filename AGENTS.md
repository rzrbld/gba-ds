# GBA DS Duo Linux fork

This is `rzrbld/gba-ds`, an explicitly authorized compatibility fork of
`kyannon/gba-ds`. Read [docs/duolite-linux.md](docs/duolite-linux.md) and retain
upstream attribution, licenses and the original app design.

- Keep CPU/display fixes here, visibly in source. Do not patch another installed
  upstream app's executable, launch scripts or config to test the fork.
- Build both the app and mGBA for ARMv8-A. Tuning for Cortex-A55 is allowed;
  `-mcpu=cortex-a55` enables instructions the Duo Lite lacks.
- Honor explicit panel rotations. The Duo top is logically 1920×1080 over a
  1280×960 bottom; native top scanout is physically portrait. Compositor/Android
  output must not be rotated twice. Test render targets and inverse pointer maps.
- Keep the fork independently installable as **GBA DS Duo** with its own updater
  feed. Slopetanium installs its release ZIP as an App; do not embed its source
  or patch an installed PortMaster package to supply it.
- Shared OS library/driver fixes belong in `rzrbld/bananaos_duo`. Preserve the
  user's saves and configuration; test with temporary save paths.
- Linux PR/manual builds create artifacts; matching `v*` tags publish ZIPs and
  checksums through Actions. Never manually upload a local public release asset
  or rewrite a published tag. Use four-number versions supported by the updater.
- Every authored commit includes
  `Co-authored-by: Codex <noreply@openai.com>`. Use `[skip ci]` for documentation
  commits when no build is needed. Preserve the existing author identity.
- Run the rotation test and portable cross-build for affected code changes.
  Upstream's full regression suite uses private fixtures: do not claim it ran
  without those fixtures or fetch commercial ROMs as a substitute.
- Inherited upstream web/Android publication is outside this fork's Linux
  release scope. Do not publish a website as a side effect of merging a fix.
