#!/usr/bin/env bash
set -euo pipefail

# scripts/check-coverage.sh
# Usage:
#   scripts/check-coverage.sh [--threshold N] [--html] [--build|--rebuild]
# Examples:
#   ./scripts/check-coverage.sh              # print summary
#   ./scripts/check-coverage.sh --threshold 80  # fail if lines < 80
#   ./scripts/check-coverage.sh --html       # generate/open HTML report

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$REPO_ROOT/build"
THRESHOLD=0
OPEN_HTML=0
DO_BUILD=0
DO_REBUILD=0

usage() {
  cat <<EOF
Usage: $0 [options]
Options:
  --threshold N, -t N   fail with exit code 1 if line coverage < N (percent)
  --html                generate HTML report (requires gcovr) and open it
  --help                show this help
EOF
  exit 1
}

# parse args
while [ "$#" -gt 0 ]; do
  case "$1" in
    --threshold|-t)
      THRESHOLD="$2"; shift 2;;
    --html)
      OPEN_HTML=1; shift;;
    --build)
      DO_BUILD=1; shift;;
    --rebuild)
      DO_REBUILD=1; shift;;
    --help|-h)
      usage;;
    *)
      echo "Unknown arg: $1" >&2; usage;;
  esac
done

if [ "$DO_BUILD" -eq 0 ] && [ ! -d "$BUILD_DIR" ]; then
  echo "Build directory not found: $BUILD_DIR (use --build or --rebuild to create it)" >&2
  exit 2
fi
# prefer gcovr
if ! command -v gcovr >/dev/null 2>&1; then
  echo "gcovr not found in PATH. Install gcovr (recommended) or use lcov/genhtml manually." >&2
  exit 2
fi

# optionally configure/build/run tests
if [ "$DO_REBUILD" -eq 1 ]; then
  echo "Rebuilding: removing ${BUILD_DIR} and reconfiguring with coverage flags"
  rm -rf "$BUILD_DIR"
  DO_BUILD=1
fi

if [ "$DO_BUILD" -eq 1 ]; then
  echo "Configuring with coverage flags and building in ${BUILD_DIR}"
  # allow user to override Qt CMake prefix via QT_CMAKE_PREFIX environment variable
  CMAKE_EXTRA_ARGS=( -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug )
  # try to detect Qt CMake location if not provided
  if [ -z "${QT_CMAKE_PREFIX-}" ]; then
    DETECTED=""
    if command -v qmake >/dev/null 2>&1; then
      QPREFIX=$(qmake -query QT_INSTALL_PREFIX 2>/dev/null || true)
      if [ -n "$QPREFIX" ]; then
        # look for cmake dir under the qmake prefix
        if [ -d "$QPREFIX/lib/cmake" ]; then
          DETECTED="$QPREFIX/lib/cmake"
        elif [ -d "$QPREFIX/lib64/cmake" ]; then
          DETECTED="$QPREFIX/lib64/cmake"
        fi
      fi
    fi
    if [ -z "$DETECTED" ]; then
      # search common system locations quickly
      for p in /usr/lib /usr/lib64 /usr/lib/x86_64-linux-gnu /opt/qt*; do
        # find Qt6/Qt5 config files
        found=$(find "$p" -maxdepth 4 -type f \( -name "Qt6Config.cmake" -o -name "qt6-config.cmake" -o -name "Qt5Config.cmake" -o -name "qt5-config.cmake" \) 2>/dev/null | head -n1 || true)
        if [ -n "$found" ]; then
          DETECTED=$(dirname "$found")
          break
        fi
      done
    fi
    if [ -n "$DETECTED" ]; then
      QT_CMAKE_PREFIX="$DETECTED"
      echo "Auto-detected Qt CMake directory: $QT_CMAKE_PREFIX"
    fi
  fi
  if [ -n "${QT_CMAKE_PREFIX-}" ]; then
    echo "Using QT_CMAKE_PREFIX=${QT_CMAKE_PREFIX}"
    CMAKE_EXTRA_ARGS+=( -DCMAKE_PREFIX_PATH="${QT_CMAKE_PREFIX}" )
  else
    echo "Could not detect Qt CMake files. Set QT_CMAKE_PREFIX to the directory containing Qt6Config.cmake or Qt5Config.cmake (e.g. /usr/lib/x86_64-linux-gnu/cmake/Qt6)" >&2
    exit 3
  fi
  cmake -S "$REPO_ROOT" -B "$BUILD_DIR" "${CMAKE_EXTRA_ARGS[@]}"
  cmake --build "$BUILD_DIR" -j"$(nproc)"
  echo "Running tests (ctest)..."
  (cd "$BUILD_DIR" && ctest --output-on-failure) || true
fi

# Print summary (gcovr will scan the build dir)
echo "== Coverage summary =="
OUT="$(cd "$BUILD_DIR" && gcovr -r "$REPO_ROOT" --print-summary --branches 2>&1)" || true
printf "%s\n" "$OUT"

# extract lines percentage
LINES_PCT="$(printf "%s\n" "$OUT" | awk -F"[:%]" '/^lines:/{gsub(/ /,"",$2); print $2; exit}')"
if [ -z "$LINES_PCT" ]; then
  echo "Could not detect lines coverage percentage" >&2
else
  echo "Lines coverage: ${LINES_PCT}%"
fi

# threshold check
if [ "$THRESHOLD" -ne 0 ]; then
  if [ -z "$LINES_PCT" ]; then
    echo "Threshold check requested but lines coverage unknown." >&2
    exit 2
  fi
  # numeric compare using awk for floats
  PASS=$(awk -v a="$LINES_PCT" -v b="$THRESHOLD" 'BEGIN{print (a+0 >= b+0) ? 1 : 0}')
  if [ "$PASS" -eq 1 ]; then
    echo "Coverage OK: ${LINES_PCT}% >= ${THRESHOLD}%"
  else
    echo "Coverage check failed: ${LINES_PCT}% < ${THRESHOLD}%" >&2
    exit 1
  fi
fi

# optionally generate/open HTML
if [ "$OPEN_HTML" -eq 1 ]; then
  echo "Generating HTML report (this may take a moment)..."
  (cd "$BUILD_DIR" && gcovr -r "$REPO_ROOT" --html --html-details -o coverage.html)
  echo "Wrote: $BUILD_DIR/coverage.html"
  # open if available
  if command -v xdg-open >/dev/null 2>&1; then
    xdg-open "$BUILD_DIR/coverage.html" || true
  fi
fi

exit 0
