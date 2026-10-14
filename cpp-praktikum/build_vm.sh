#!/usr/bin/env bash
# Dieses Skript erst NACH dem Kopieren in der Linux-VM ausfuehren:
#   bash build_vm.sh
# Es baut nur die Aufgaben von Tag 4/5 und startet kein Roboterprogramm.
set -euo pipefail

if [[ "$(uname -s)" != "Linux" ]]; then
    echo "Bitte dieses Build-Skript in der Linux-VM ausfuehren." >&2
    exit 1
fi

# Der Speicherort des Skripts bestimmt den Projektpfad, nicht das aktuelle
# Arbeitsverzeichnis. Anfuehrungszeichen erhalten Leerzeichen in Ordnernamen.
project_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
build_dir="$project_dir/build-vm"

# Separates Buildverzeichnis: der vorhandene macOS-CMake-Cache passt nicht zur
# VM. Ein mitkopierter alter Ordner "build" wird hier nicht verwendet.
cmake -S "$project_dir" -B "$build_dir" \
    -DSEPR_BUILD_SIMULATION=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build "$build_dir" --parallel --target \
    tag04_keyboard_control \
    tag04_communicate_methode \
    tag04_interfaces_and_step_methode \
    tag04_pid_controller \
    tag04_keyboard_and_controler \
    tag05_positionsschaetzung \
    tag05_koordinatenliste \
    tag05_robot_control

echo "Fertig. Programme liegen in $build_dir"
echo "Die weiteren Schritte stehen in README_VM.md."
