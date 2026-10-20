# Verification — Creative Mic 1.0.1

## Local development checks (2026-10-20)

13 unit tests passed on the development host, with a compiled RNNoise filter:
PCM frame counts and silence; exact 480-sample dry delay; aligned half-wet mix;
saturation at -32768/32767; invalid/nonfinite arguments and truncated input;
deterministic seeded white-noise processing; PulseAudio 13 text-format parsing;
startup module rollback; module ownership and child cleanup; child-failure and
server-disconnection detection; instance lock; Python 3.8 syntax parsing.

Seeded white-noise output/input power ratio after warmup: 0.214007 (about 6.7 dB
reduction). This is a synthetic regression test, not a real-world speech-quality
benchmark, intelligibility measurement, or comparison with Agora.

## Target verification

Ubuntu 20.04 container installation, Python 3.8 execution, PulseAudio 13 routing,
and GUI startup are required before release. See the CI run associated with the
released commit for the authoritative logs. These checks are pending until a
successful run is recorded here. A local build on another distribution is not
proof of Ubuntu 20.04 compatibility.

## Material limitations

No physical microphone, desktop call, Discord/Zoom/OBS/browser interaction,
speech intelligibility, or real-room quality has been verified in a container.
PipeWire compatibility uses standard pipewire-pulse protocol and has not been
verified on an actual PipeWire desktop. No background-speaker isolation, echo
cancellation, or dereverberation is implemented. End-to-end latency is not
measured. SIGKILL cannot trigger cleanup. Device hotplug requires Refresh and
restart; live settings changes are intentionally disabled while running.
