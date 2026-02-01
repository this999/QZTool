Scripts directory — quick reference

This folder contains small helper scripts for coverage reporting and cleanup used by the QZTool project.
Run the scripts from the repository root (`/home/dawid/Code/qt/QZTool`).

Prerequisites
- gcovr (recommended) for HTML reports: `sudo apt install gcovr`
- lcov/genhtml (optional) if you prefer lcov tools: `sudo apt install lcov`
- build directory must exist and tests must have been executed to produce `.gcda` / `.gcno` files.
 - If you don't have a `build/` yet, `check-coverage.sh` supports `--build` and `--rebuild` to configure/build/run tests automatically.

Available scripts

1) check-coverage.sh
Purpose: print a coverage summary (lines/functions/branches). Optionally generate HTML and/or fail if coverage is below a threshold.
Usage:
   ./scripts/check-coverage.sh                       # print summary (reads build/)
   ./scripts/check-coverage.sh --threshold 80        # exit 1 if line coverage < 80%
   ./scripts/check-coverage.sh --html                # generate and open build/coverage.html
   ./scripts/check-coverage.sh --threshold 80 --html
   ./scripts/check-coverage.sh --build --html        # configure (with coverage flags), build, run tests, generate HTML
   ./scripts/check-coverage.sh --rebuild --html      # same as --build but removes existing build/ first

Notes:
- The `--build` and `--rebuild` modes will attempt to configure CMake with `-DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug`.
- If your Qt installation is in a nonstandard location, set `QT_CMAKE_PREFIX` to the directory containing `Qt6Config.cmake` or `Qt5Config.cmake` (for example `/usr/lib/x86_64-linux-gnu/cmake/Qt6`). Example:
   export QT_CMAKE_PREFIX=/usr/lib/x86_64-linux-gnu/cmake/Qt6
   ./scripts/check-coverage.sh --rebuild --html

The script uses `gcovr` to generate summaries and HTML. Install it with your package manager if missing.

2) clean-coverage.sh
Purpose: remove coverage artefacts, or archive HTML reports.
Usage:
   ./scripts/clean-coverage.sh               # default: keep main coverage.html, remove per-file HTML and css
   ./scripts/clean-coverage.sh --keep-main   # same as default
   ./scripts/clean-coverage.sh --full        # remove all coverage artifacts (.gcda, .gcno, coverage*)
   ./scripts/clean-coverage.sh --archive     # tar/gzip HTML reports to ~/coverage-report-YYYY-MM-DD.tgz

CMake targets
The top-level `CMakeLists.txt` provides convenience targets to run the scripts or gcovr via CMake (if present):
   cmake --build build --target check-coverage     # runs scripts/check-coverage.sh (if the target is defined in CMake)
   cmake --build build --target clean-coverage     # runs scripts/clean-coverage.sh --full (if defined)
   cmake --build build --target coverage-report    # runs gcovr in build to produce coverage.html

Note: if a particular target is not defined in your CMakeLists, use the scripts directly.

Recommended workflow
1. Configure build with coverage (optional):
   cmake -S . -B build -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
2. Build and run tests:
   cmake --build build -j$(nproc)
   (cd build && ctest --output-on-failure)
3. Check coverage:
   ./scripts/check-coverage.sh --html
4. Clean or archive reports as needed:
   ./scripts/clean-coverage.sh --keep-main

If you want a different behavior, open an issue or edit the script to match your CI requirements.

Aliases / convenience
If you want a short local alias for the project, add one of these to your `~/.bashrc`:

Simple alias (project-local):
```bash
echo "alias pcmake='./scripts/cmake-wrapper.sh'" >> ~/.bashrc
source ~/.bashrc
# then: pcmake check-coverage
```

Safe function (calls wrapper only when inside the repo):
```bash
cat >> ~/.bashrc <<'EOF'
cm() {
   if [ -x "./scripts/cmake-wrapper.sh" ]; then
      ./scripts/cmake-wrapper.sh "$@"
   else
      command cmake "$@"
   fi
}
EOF
source ~/.bashrc
# then: cm check-coverage
```
