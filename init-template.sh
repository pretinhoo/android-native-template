#!/usr/bin/env bash
set -euo pipefail

# -----------------------------------------------------------------------------
# Android Native Template Initializer
#
# This script customizes the template by updating:
# - applicationId
# - namespace
# - native library name
# - app label
# - optional root project name
#
# It also cleans generated build artifacts so the project can be opened fresh
# in Android Studio.
# -----------------------------------------------------------------------------

if [[ $# -lt 4 ]]; then
  echo "Usage:"
  echo "  $0 <applicationId> <namespace> <libName> <appLabel> [rootProjectName]"
  echo
  echo "Example:"
  echo "  $0 com.acme.revkit com.acme.revkit revkit \"RevKit\" RevKit"
  exit 1
fi

APP_ID="$1"
NAMESPACE="$2"
LIB_NAME="$3"
APP_LABEL="$4"
ROOT_NAME="${5:-}"

# Expected project paths
MANIFEST="app/src/main/AndroidManifest.xml"
CMAKE="app/src/main/cpp/CMakeLists.txt"
APP_GRADLE="app/build.gradle"
SETTINGS="settings.gradle"

# Quick validation
for f in "$MANIFEST" "$CMAKE" "$APP_GRADLE" "$SETTINGS"; do
  if [[ ! -f "$f" ]]; then
    echo "ERROR: Expected file not found: $f"
    exit 2
  fi
done

backup() {
  cp -f "$1" "$1.bak"
}

echo "==> Creating .bak backups"
backup "$MANIFEST"
backup "$CMAKE"
backup "$APP_GRADLE"
backup "$SETTINGS"

echo "==> AndroidManifest.xml: package + label + native lib name"

# package="..."
perl -pi -e "s/package=\"[^\"]+\"/package=\"$APP_ID\"/g" "$MANIFEST"

# android:label="..."
perl -pi -e "s/android:label=\"[^\"]+\"/android:label=\"$APP_LABEL\"/g" "$MANIFEST"

# android:value="revkit" (android.app.lib_name)
perl -pi -e "s/android:value=\"[^\"]+\"/android:value=\"$LIB_NAME\"/g" "$MANIFEST"

echo "==> CMakeLists.txt: project and library target name -> $LIB_NAME"

# project(revkit)
perl -pi -e "s/\\bproject\\([^)]+\\)/project($LIB_NAME)/g" "$CMAKE"

# add_library(revkit SHARED ...)
perl -pi -e "s/\\badd_library\\(\\s*([A-Za-z0-9_\\-]+)\\s+SHARED\\b/add_library($LIB_NAME SHARED/g" "$CMAKE"

# target_* references (include_directories, compile_definitions, link_libraries, link_options)
perl -pi -e "s/\\btarget_(include_directories|compile_definitions|link_libraries|link_options)\\(\\s*([A-Za-z0-9_\\-]+)\\b/target_\\1($LIB_NAME/g" "$CMAKE"

echo "==> app/build.gradle: namespace + applicationId"

# namespace "..."
perl -pi -e "s/\\bnamespace\\s+\"[^\"]+\"/namespace \"$NAMESPACE\"/g" "$APP_GRADLE"

# applicationId "..."
perl -pi -e "s/\\bapplicationId\\s+\"[^\"]+\"/applicationId \"$APP_ID\"/g" "$APP_GRADLE"

echo "==> settings.gradle: rootProject.name (optional)"

if [[ -n "$ROOT_NAME" ]]; then
  perl -pi -e "s/\\brootProject\\.name\\s*=\\s*\"[^\"]+\"/rootProject.name = \"$ROOT_NAME\"/g" "$SETTINGS"
fi

echo "==> Cleaning generated build artifacts"

rm -rf .idea .gradle app/.cxx app/build build 2>/dev/null || true

echo
echo "OK. Changes applied:"
echo "  applicationId : $APP_ID"
echo "  namespace     : $NAMESPACE"
echo "  libName       : $LIB_NAME"
echo "  appLabel      : $APP_LABEL"
[[ -n "$ROOT_NAME" ]] && echo "  projectName   : $ROOT_NAME"

echo
echo "Next steps:"
echo "  1) Open Android Studio or run: ./gradlew clean assembleDebug"
echo "  2) Sync Gradle"