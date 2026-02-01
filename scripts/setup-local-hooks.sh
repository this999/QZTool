#!/usr/bin/env bash
set -euo pipefail

# scripts/setup-local-hooks.sh
# Installs a simple pre-commit hook that runs clang-format on staged files.
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
HOOK_DIR="$REPO_ROOT/.git/hooks"
HOOK_FILE="$HOOK_DIR/pre-commit"

mkdir -p "$HOOK_DIR"
cat > "$HOOK_FILE" <<'HOOK'
#!/usr/bin/env bash
# Pre-commit hook: format staged C/C++ files
STAGED=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(cpp|h)$' || true)
if [ -n "$STAGED" ]; then
  echo "Running clang-format on staged files..."
  echo "$STAGED" | xargs -r clang-format -style=file -i
  # add modified files back to staging
  echo "$STAGED" | xargs -r git add
fi
HOOK

chmod +x "$HOOK_FILE"
echo "Pre-commit hook installed at $HOOK_FILE"
