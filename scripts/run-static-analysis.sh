#!/usr/bin/env bash
set -euo pipefail

# scripts/run-static-analysis.sh
#  - clang-format check (fails if formatting differs)
#  - generate compile_commands.json and run clang-tidy (reports warnings)

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$REPO_ROOT"

# 1) clang-format check
echo "==> clang-format check"
FILES=$(git ls-files '*.cpp' '*.h' || true)
if [ -n "$FILES" ]; then
  # run clang-format in a temp copy to detect changes
  TMP_DIR=$(mktemp -d)
  cp -a $FILES "$TMP_DIR/" || true
  pushd "$TMP_DIR" >/dev/null
  echo "$FILES" | xargs -r clang-format -style=file -i
  if ! git --no-pager diff --exit-code >/dev/null 2>&1; then
    echo "clang-format found formatting issues. Run the following in repo root to fix:"
    echo
    echo "  clang-format -style=file -i \\$(git ls-files '*.cpp' '*.h')"
    popd >/dev/null
    rm -rf "$TMP_DIR"
    exit 1
  fi
  popd >/dev/null
  rm -rf "$TMP_DIR"
  echo "clang-format: OK"
else
  echo "No C/C++ files found for clang-format"
fi

# 2) clang-tidy
echo "==> clang-tidy (reports only; does not modify files)"
BUILD_DIR="$REPO_ROOT/build-clang-tidy"
mkdir -p "$BUILD_DIR"
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON >/dev/null

FILES=$(git ls-files 'QZToolCore/*.cpp' 'tests/*.cpp' || true)
if [ -n "$FILES" ]; then
  echo "Running clang-tidy on: $FILES"
  # run clang-tidy; collect warnings but do not fail the script (change to `|| exit 1` to fail)
  echo "$FILES" | xargs -r -n1 -P4 clang-tidy -p "$BUILD_DIR" --quiet || true
  echo "clang-tidy: completed (check output for warnings)"
else
  echo "No target files for clang-tidy"
fi

exit 0
