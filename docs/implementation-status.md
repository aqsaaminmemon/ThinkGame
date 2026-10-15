# ThinkGame implementation status

Reviewed and rebuilt on 2026-10-15. The project is a working offline C++17 / SFML 3 desktop game, with source and a runnable Linux build.

| Requirement | Status |
| --- | --- |
| Illustrated home screen with Play / Continue, Levels and Score | Implemented; new mascot, simpler wording, mouse and keyboard navigation checked |
| Score of 100 points per solved level, derived from existing saves | Implemented; reload and duplicate-award checks pass |
| Larger puzzles, scaling at large sizes, 1120 x 800 default | Implemented; 720 x 600 and 1920 x 1080 rendering and mouse targets checked |
| 30 authored levels, one missing result per level | Implemented and checked |
| Five shapes, six levels per shape | Implemented and checked |
| Different mathematical relationships | Catalogue reviewed; same-arity equivalence screening passes |
| Sequential unlocking and clickable locked-level messages | Implemented; model and graphical tests pass |
| Connected Levels map with yellow solved nodes | Implemented and rendered |
| Supplied logo, mostly white UI, pastel cells, yellow controls | Implemented and visually reviewed |
| Keyboard entry, Backspace, Enter and Submit | Implemented; graphical interaction tests pass |
| Answer feedback and worked explanations | Implemented across all 30 levels |
| Persistent pattern help in words, with no countdown | Implemented; stays open beyond the old timer, closes through × or Escape |
| One clue at levels 1–10, two at 11–20, three at 21–30 | Implemented; all tier boundaries, saved allowances, progressive clues and legacy saves checked |
| Six cute sound cues for clicks, hints, answers, starting and completion | Implemented with bundled original WAV files; playback and immediate mute checked |
| Saved progress, backups and corruption handling | Implemented; tests pass |
| Smaller desktop windows and keyboard access to Next | Implemented; regression test passes at 720 x 760 |
| Retained CMake build and updated launcher | Implemented; run.sh prefers the local build |
| Installed asset discovery through PATH and symbolic links | Implemented; isolated discovery tests pass |
| Readable hint cards with wrapped progressive clues | Implemented; all 30 levels rendered at large and 720 x 600 sizes |

## Latest verification

Both CTest suites pass in the retained project build:
- Core suite: all 90 examples, 30 independent final answers, rule screening, progression, input, tiered clue limits for every level, persistent viewing, legacy quotas, saves, recovery, invalid saves, save failures, and installed asset discovery through PATH and symbolic links, including incomplete asset folders.
- Graphical suite: home-screen entry, Play / Continue, Home / Levels / Score navigation, puzzle input isolation on menus, saved score, scaled mouse targets, real SFML event handling and rendering, locked popups, both submission methods, every level, all five shapes, small-window screens, keyboard progression, explanations, word-based hints for all 30 levels at both sizes, and final completion.

The complete output is in `verification.log`. The screenshots in `screenshots/` are renders of the C++ game.

## Finishing changes from the review

- Added a home screen with progress cards and Play, Levels and Score choices, plus persistent navigation on each screen. The new cute brain mascot is decorative; actual puzzles appear only inside the game.
- Added persistent pattern help, tiered allowances, progressively more specific word clues, and a close control. Previously unlocked clues can be reread without increasing the allowance.
- Replaced formulas in hint cards with separate authored clues in plain words for every level. Formulas remain in the worked solution after solving.
- Enabled SFML Audio and bundled six short, original synthesized sound effects. The top-bar sound control stops all playback when muted.
- Added 100 points per completed level without changing the save format. Replaying preserves completion and score.
- Changed the default window to 1120 x 800 and minimum to 720 x 600; enlarged the puzzle card and shapes, removed the puzzle sidebar to give the centre more room, centred the heading, and scaled the interface on large windows with matching mouse coordinates.
- Used a five-column map so all 30 level nodes fit in shorter windows.
- Replaced the cube-only puzzle with a remainder rule, avoiding a level that differed from squaring only in its exponent.
- Made run.sh prefer the newly compiled game instead of the older packaged executable.
- Improved keyboard focus scrolling in both directions in shorter windows.
- Made caret timing independent of the platform clock's tick unit.
- Cleared stale recovery notices after a healthy save load.
- Retained the build dependencies under build/ so later rebuilding does not require temporary directories.
- Separated the Hint label from its bulb icon and wrapped formula text with a hint card that grows to fit its contents.
- Resolved the executable location through PATH and symbolic links before finding assets; incomplete asset folders no longer hide a valid installed bundle.
- Corrected the README's launcher instructions and documented local installation.

## Practical limits

Linux compilation, rendering and behavior are verified. Windows and macOS builds remain unverified. Difficulty is authored and should be playtested; these tests do not establish a strictly increasing difficulty for every player or mathematical uniqueness among every possible rule.

## Run and rebuild

```bash
./run.sh
cmake --build build -j 4
ctest --test-dir build --output-on-failure
```

The graphical test requires an available desktop display and uses isolated test saves. The player's save is not used by the tests. Restart an already-open game to load the updated executable.
