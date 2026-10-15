#!/usr/bin/env bash
set -euo pipefail
thinkgame_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
if [[ -x "$thinkgame_dir/build/thinkgame" ]]; then
  exec "$thinkgame_dir/build/thinkgame" "$@"
elif [[ -x "$thinkgame_dir/bin/thinkgame" ]]; then
  exec "$thinkgame_dir/bin/thinkgame" "$@"
else
  printf '%s\n' 'Build first: cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j 4' >&2
  exit 1
fi
