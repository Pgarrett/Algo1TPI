project(tests)

add_subdirectory(lib/googletest-release-1.7.0)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(correrTests
        factories.h factories.cpp auxiliares.h
        test_campo.cpp
        test_drone_lo_basico.cpp
        test_drone_vuelo_escalerado.cpp
        test_drone_vuelos_cruzados.cpp
        test_drone_cargar_y_guardar.cpp
        test_sistema_lo_basico.cpp
        test_sistema_se_vino_la_maleza.cpp
        test_sistema_se_expande_plaga.cpp
        test_sistema_despegar.cpp
        test_sistema_listo_para_cosechar.cpp
        test_sistema_volar_y_sensar.cpp
        test_sistema_cargar_y_guardar.cpp)

target_link_libraries(correrTests gtest gtest_main)
target_link_libraries(correrTests src)
