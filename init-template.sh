#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 4 ]]; then
  echo "Uso: $0 <applicationId> <namespace> <libName> <appLabel> [rootProjectName]"
  echo "Ejemplo: $0 com.acme.revkit com.acme.revkit revkit \"RevKit\" RevKit"
  exit 1
fi

APP_ID="$1"
NAMESPACE="$2"
LIB_NAME="$3"
APP_LABEL="$4"
ROOT_NAME="${5:-}"

# Rutas esperadas
MANIFEST="app/src/main/AndroidManifest.xml"
CMAKE="app/src/main/cpp/CMakeLists.txt"
APP_GRADLE="app/build.gradle"
SETTINGS="settings.gradle"

# Validaciones rápidas
for f in "$MANIFEST" "$CMAKE" "$APP_GRADLE" "$SETTINGS"; do
  if [[ ! -f "$f" ]]; then
    echo "ERROR: No existe el fichero esperado: $f"
    exit 2
  fi
done

backup() { cp -f "$1" "$1.bak"; }

echo "==> Backups .bak"
backup "$MANIFEST"
backup "$CMAKE"
backup "$APP_GRADLE"
backup "$SETTINGS"

echo "==> AndroidManifest.xml: package + label + lib_name"
# package="..."
perl -pi -e "s/package=\"[^\"]+\"/package=\"$APP_ID\"/g" "$MANIFEST"
# android:label="..."
perl -pi -e "s/android:label=\"[^\"]+\"/android:label=\"$APP_LABEL\"/g" "$MANIFEST"
# android:value="hello"
perl -pi -e "s/android:value=\"[^\"]+\"/android:value=\"$LIB_NAME\"/g" "$MANIFEST"

echo "==> CMakeLists.txt: project/lib target name -> $LIB_NAME"
# project(hello)
perl -pi -e "s/\\bproject\\([^)]+\\)/project($LIB_NAME)/g" "$CMAKE"

# Cambiar el target de la lib: add_library(hello SHARED ...)
perl -pi -e "s/\\badd_library\\(\\s*([A-Za-z0-9_\\-]+)\\s+SHARED\\b/add_library($LIB_NAME SHARED/g" "$CMAKE"

# Cambiar referencias target_* (hello ...)
perl -pi -e "s/\\btarget_(include_directories|compile_definitions|link_libraries|link_options)\\(\\s*([A-Za-z0-9_\\-]+)\\b/target_\\1($LIB_NAME/g" "$CMAKE"

echo "==> app/build.gradle: namespace + applicationId"
# namespace "..."
perl -pi -e "s/\\bnamespace\\s+\"[^\"]+\"/namespace \"$NAMESPACE\"/g" "$APP_GRADLE"
# applicationId "..."
perl -pi -e "s/\\bapplicationId\\s+\"[^\"]+\"/applicationId \"$APP_ID\"/g" "$APP_GRADLE"

echo "==> settings.gradle: rootProject.name (opcional)"
if [[ -n "$ROOT_NAME" ]]; then
  perl -pi -e "s/\\brootProject\\.name\\s*=\\s*\"[^\"]+\"/rootProject.name = \"$ROOT_NAME\"/g" "$SETTINGS"
fi

echo "==> Limpieza de builds generados"
rm -rf .idea .gradle app/.cxx app/build build 2>/dev/null || true

echo "OK. Cambios aplicados:"
echo "  applicationId: $APP_ID"
echo "  namespace:     $NAMESPACE"
echo "  libName:       $LIB_NAME"
echo "  appLabel:      $APP_LABEL"
[[ -n "$ROOT_NAME" ]] && echo "  projectName:   $ROOT_NAME"
echo
echo "Siguiente:"
echo "  1) Abre Android Studio o ejecuta: ./gradlew clean assembleDebug"
echo "  2) Sync Gradle"
