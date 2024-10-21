cmake_minimum_required(VERSION 3.25)

find_package(cmake-bare REQUIRED PATHS node_modules/cmake-bare)

project(bare_realm C)

add_bare_module(bare_realm)

target_sources(
  ${bare_realm}
  PRIVATE
    binding.c
)
