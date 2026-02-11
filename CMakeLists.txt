cmake_minimum_required(VERSION 3.14)
project(nBLManalysis)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ROOT CONFIG REQUIRED RIO ROOTDataFrame ROOTNTuple ROOTNTupleUtil)

add_library(nBLManalysis SHARED)
target_link_libraries(nBLManalysis PUBLIC ROOT::RIO ROOT::ROOTDataFrame ROOT::ROOTNTuple  ROOT::ROOTNTupleUtil)
target_sources(nBLManalysis PRIVATE 
    "src/DSignal.cpp"
    "src/Event.cpp"
    "src/IOLecroy.cpp"
    "src/nBLMUtils.cpp"
    "src/Processing.cpp"
    "src/RawConvertor.cpp"
    "src/Run.cpp"
    "src/ScanFolder.cpp"
    "src/TimeStamp.cpp"
    "src/Trigger.cpp")

target_include_directories(nBLManalysis PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:include/>
)
root_generate_dictionary(G__nBLManalysis include/nBLM/DSignal.hpp include/nBLM/Event.hpp include/nBLM/TimeStamp.hpp MODULE nBLManalysis LINKDEF include/nBLM/LinkDef.hpp)

add_subdirectory(use)