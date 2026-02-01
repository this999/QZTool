#!/usr/bin/env bash
set -euo pipefail

# clean-coverage.sh
# Usage:
#   scripts/clean-coverage.sh [--keep-main|--full|--archive]
# Runs relative to the repository root and operates on ./build by default.

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$REPO_ROOT/build"

usage() {
  cat <<EOF
Usage: $0 [option]
Options:
  --keep-main   Remove per-file HTML and css, keep build/coverage.html (default)
  --full        Remove all coverage artifacts (coverage*, .gcda, .gcno, .info) in build/
  --archive     Archive coverage HTML files into ~/coverage-report-YYYY-MM-DD.tgz (keeps files)
  --help        Show this help

Run this script from repository; it targets the ${BUILD_DIR} directory.
EOF
  exit 1
}

if [ "$#" -eq 0 ]; then
  MODE=keep-main
else
  case "$1" in
    --keep-main) MODE=keep-main ;;
    --full) MODE=full ;;
    --archive) MODE=archive ;;
    --help|-h) usage ;;
    *) echo "Unknown option: $1" >&2; usage ;;
  esac
fi

if [ ! -d "$BUILD_DIR" ]; then
  echo "Build directory not found: $BUILD_DIR" >&2
  exit 1
fi

cd "$BUILD_DIR"

case "$MODE" in
  keep-main)
    echo "Removing per-file HTML reports and coverage.css, keeping coverage.html if present..."
    # remove coverage.<file>.html but keep coverage.html
    find . -maxdepth 1 -type f -name 'coverage.*.html' ! -name 'coverage.html' -print -delete || true
    [ -f coverage.css ] && rm -v coverage.css || true
    echo "Done."
    ;;

  full)
    echo "Removing all coverage artifacts from $BUILD_DIR (coverage*, .gcda, .gcno, .info)..."
    find . -type f \( -name 'coverage*' -o -name '*.gcda' -o -name '*.gcno' -o -name '*.info' \) -print -delete || true
    echo "Done."
    ;;

  archive)
    DST="${HOME}/coverage-report-$(date +%F).tgz"
    echo "Archiving coverage HTML files to $DST (keeps originals)..."
    tar czf "$DST" coverage.html coverage.*.html coverage.css 2>/dev/null || {
      echo "No HTML coverage files found to archive." >&2
      exit 1
    }
    echo "Archived to $DST"
    ;;

  *)
    usage ;;
esac

exit 0
