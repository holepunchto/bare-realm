cmake_minimum_required(VERSION 3.25)

project(bare_realm C)

include(bare)

add_bare_module(bare_realm)

target_sources(
  ${bare_realm}
  PRIVATE
    binding.c
)
