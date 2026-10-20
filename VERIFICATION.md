# Verification — Creative Mic 1.0.1

## Local development checks (2026-10-20)

16 unit tests passed on the development host, with a compiled RNNoise filter:
PCM frame counts and silence; exact 480-sample dry delay; aligned half-wet mix;
wet-path correlation confirming the 480-sample delay; saturation at -32768/32767; invalid/nonfinite arguments and truncated input;
deterministic seeded white-noise processing; PulseAudio 13 text-format parsing;
startup module rollback, rollback at each process spawn, interrupted module load; module ownership and child cleanup; child-failure and
server-disconnection detection; instance lock; Python 3.8 syntax parsing.

Seeded white-noise output/input power ratio after warmup: 0.214007 (about 6.7 dB
reduction). This is a synthetic regression test, not a real-world speech-quality
benchmark, intelligibility measurement, or comparison with Agora.

## Target verification

Passed in an actual Ubuntu 20.04.6 LTS amd64 GitHub Actions container:
Python 3.8.10, PulseAudio 13.99.1, and glibc 2.31. Successful verification run:
https://github.com/PSNarayananmc/creative-mic/actions/runs/39378708980

- All 16 unit tests passed against the target-built native filter.
- A real isolated PulseAudio server delivered 51,920 PCM samples through the
  virtual microphone (peak 10,000). The source display name is exactly
  `Creative Mic`; the source and monitors are excluded from input selection.
- Real headless CLI SIGINT and SIGTERM exited cleanly and restored the module
  list. Stop retained unrelated modules. Native child termination and server
  shutdown were detected and cleaned up.
- Tkinter GUI startup/normal close passed under Xvfb as a non-root desktop user,
  both from source and from the installed package.
- The .deb built on Ubuntu 20.04 installed successfully with apt/dpkg. Installed
  CLI version/help and native silence processing passed. Dynamic dependencies
  resolved to the target's libc and libm, without bundled host libraries.
- Debian archives are `control.tar.gz` and `data.tar.gz`. Package files use root
  ownership, 0755 executables/directories, and 0644 source/docs/assets.

The release workflow repeats these checks on the release-triggering commit,
including the final stricter module-ownership checks, before publication. The
release installer is the artifact from that run, not the development-host build.

## Material limitations

No physical microphone, desktop call, Discord/Zoom/OBS/browser interaction,
speech intelligibility, or real-room quality has been verified in a container.
PipeWire compatibility uses standard pipewire-pulse protocol and has not been
verified on an actual PipeWire desktop. No background-speaker isolation, echo
cancellation, or dereverberation is implemented. End-to-end latency is not
measured. SIGKILL cannot trigger cleanup. Device hotplug requires Refresh and
restart; live settings changes are intentionally disabled while running.
