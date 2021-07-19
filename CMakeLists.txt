
set(V ${CMAKE_CURRENT_LIST_DIR})
set(VERILATEST_DIR ${V} PARENT_SCOPE)
set(VERILATEST_SOURCES
    ${V}/SignalObserver.cpp
    ${V}/TestBench.cpp
    PARENT_SCOPE
)

function(verilatest_setup TARGET_NAME)
    target_include_directories(${TARGET_NAME} PUBLIC ${VERILATEST_DIR})
    target_sources(${TARGET_NAME} PRIVATE ${VERILATEST_SOURCES})
endfunction()
