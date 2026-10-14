cmake_minimum_required(VERSION 3.16)
project(CppPraktikum LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(tag01_taschenrechner
    tag01/taschenrechner/main.cpp
    tag01/taschenrechner/taschenrechner.cpp
)

add_executable(tag01_maya_numbers
    tag01/maya_numbers/main.cpp
    tag01/maya_numbers/maya_numbers.cpp
)

add_executable(tag01_verfahren_von_heron
    tag01/verfahren_von_heron/main.cpp 
    tag01/verfahren_von_heron/verfahren_von_heron.cpp
)

add_executable(tag01_palindrome
    tag01/palindrome/main.cpp 
    tag01/palindrome/palindrome.cpp 
)

add_executable(tag01_buchstaben_und_woerter
    tag01/buchstaben_und_woerter/buchstaben_und_woerter.cpp
)

add_executable(tag01_quadratische_funktionen
    tag01/quadratische_funktionen/quadratische_funktionen.cpp 
)

add_executable(tag01_referenzen
    tag01/referenzen/referenzen.cpp
)

add_executable(tag01_speicher_und_arrays
    tag01/speicher_und_arrays/speicher_und_arrays.cpp
)

add_executable(tag01_bubble_sort
    tag01/bubble_sort/bubble_sort.cpp
)

add_executable(tag02_flaschen
    tag02/flaschen/flaschen.cpp 
    tag02/flaschen/main.cpp
)

add_executable(tag02_datum
    tag02/datum/datum.cpp 
    tag02/datum/main.cpp
)

add_executable(tag02_fahrzeuge
    tag02/fahrzeuge/main.cpp 
    tag02/fahrzeuge/vehicle.cpp
)

add_executable(tag02_quadrate
    tag02/quadrate/main.cpp 
    tag02/quadrate/squares.cpp
)

add_executable(tag02_vererbung
    tag02/vererbung_und_ueberschreibung/main.cpp 
    tag02/vererbung_und_ueberschreibung/vererbung_und_ueberschreiben.cpp
)

add_executable(tag02_virtuelle_methoden
    tag02/virtuelle_methode/vererbung_und_ueberschreiben.cpp 
    tag02/virtuelle_methode/main.cpp 
)   

add_executable(tag02_polymorphie
    tag02/polymorphie/main.cpp
)

add_executable(tag03_rekursion
    tag03/Rekursion/main.cpp
)

add_executable(tag03_fibonacci
    tag03/fibonacci/main.cpp
)

add_executable(tag03_read_and_write
    tag03/read_and_write/main.cpp
)

add_executable(tag03_exceptions
    tag03/Ausnahmen/main.cpp
)

add_executable(tag03_list
    tag03/list/main.cpp
)

add_executable(tag03_ncurses
    tag03/ncurses/main.cpp 
)

target_link_libraries(tag03_ncurses PRIVATE ncurses)


add_executable(tag04_keyboard_control
    tag04/keyboard_control/main.cpp
    tag04/keyboard_control/keyboard_control.cpp
)
add_executable(tag04_communicate_methode
    tag04/communicate_methode/main.cpp
    tag04/communicate_methode/keyboard_control.cpp
)
target_link_libraries(tag04_communicate_methode PRIVATE ncurses)
target_include_directories(tag04_communicate_methode PRIVATE common)

# Tag 4/5: Rechenklassen und dateibasierte Aufgaben. Diese benoetigen weder
# die Simulation noch Linux-Echtzeitsignale und koennen separat gebaut werden.
add_library(sepr_pid_controller tag04/controler/PIDController.cpp)
target_include_directories(sepr_pid_controller PUBLIC tag04/controler)

add_executable(tag04_pid_controller tag04/controler/main.cpp)
target_link_libraries(tag04_pid_controller PRIVATE sepr_pid_controller)

add_library(sepr_pos_estimation tag05/positionsschaetzung/pos_estimation.cpp)
target_include_directories(sepr_pos_estimation PUBLIC tag05/positionsschaetzung)

add_executable(tag05_positionsschaetzung tag05/positionsschaetzung/main.cpp)
target_link_libraries(tag05_positionsschaetzung PRIVATE sepr_pos_estimation)

add_library(sepr_maneuver tag05/maneuver/maneuver.cpp)
target_include_directories(sepr_maneuver PUBLIC tag05/maneuver)

add_executable(tag05_koordinatenliste tag05/koordinatenliste/main.cpp)
target_link_libraries(tag05_koordinatenliste PRIVATE sepr_maneuver)

# InterfaceSIM verwendet SIGRTMIN und POSIX-Timer. Die fertigen Steuerungen
# sind deshalb fuer die Linux-VM vorgesehen, nicht fuer den macOS-Host.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(SEPR_SIMULATION_DEFAULT ON)
else()
    set(SEPR_SIMULATION_DEFAULT OFF)
endif()
option(SEPR_BUILD_SIMULATION "Steuerungen fuer die Linux-VM bauen"
       ${SEPR_SIMULATION_DEFAULT})

if(SEPR_BUILD_SIMULATION)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
        message(FATAL_ERROR "Die Simulationsprogramme bitte in der Linux-VM bauen.")
    endif()

    find_package(Curses REQUIRED)
    find_library(SEPR_JSONCPP_LIBRARY NAMES jsoncpp)
    find_path(SEPR_JSONCPP_INCLUDE_BASE NAMES jsoncpp/json/json.h)
    find_library(SEPR_RT_LIBRARY NAMES rt)
    if(NOT SEPR_JSONCPP_LIBRARY OR NOT SEPR_JSONCPP_INCLUDE_BASE OR NOT SEPR_RT_LIBRARY)
        message(FATAL_ERROR "VM-Bibliotheken jsoncpp (mit Headern) und rt werden benoetigt.")
    endif()

    # Das schon vorhandene Lehrstuhlmodul wird unveraendert genau einmal
    # uebersetzt und von allen drei Simulationsprogrammen verwendet.
    add_library(sepr_sim_interface
        tag04/interfaces_and_step_methode/InterfaceSIM.cpp
    )
    target_include_directories(sepr_sim_interface PUBLIC
        tag04/interfaces_and_step_methode
        common
        ${CURSES_INCLUDE_DIRS}
        ${SEPR_JSONCPP_INCLUDE_BASE}
    )
    target_link_libraries(sepr_sim_interface PUBLIC
        ${CURSES_LIBRARIES} ${SEPR_RT_LIBRARY} ${SEPR_JSONCPP_LIBRARY}
    )

    add_executable(tag04_interfaces_and_step_methode
        tag04/interfaces_and_step_methode/main.cpp
        tag04/interfaces_and_step_methode/keyboard_control.cpp
    )
    target_link_libraries(tag04_interfaces_and_step_methode PRIVATE sepr_sim_interface)

    add_executable(tag04_keyboard_and_controler
        tag04/keyboard_and_controler/main.cpp
        tag04/keyboard_and_controler/keyboard_control.cpp
    )
    target_link_libraries(tag04_keyboard_and_controler PRIVATE
        sepr_sim_interface sepr_pid_controller
    )

    add_executable(tag05_robot_control
        tag05/robot_control/main.cpp
        tag05/robot_control/robot_control.cpp
    )
    target_link_libraries(tag05_robot_control PRIVATE
        sepr_sim_interface sepr_pid_controller sepr_pos_estimation sepr_maneuver
    )
endif()
