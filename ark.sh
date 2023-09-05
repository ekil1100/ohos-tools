#!/bin/bash

# Check the number of input arguments
if [ $# -lt 1 ]; then
  echo "Please provide the file path as an argument."
  exit 1
fi

# Get the provided file path
file_path="$1"

# Extract the file name from the file path
file_name=$(basename "$file_path" | cut -d. -f1)

# Check if --aot flag is provided
if [[ "$@" =~ "--aot" ]]; then
  # Build the AOT compilation command
  aot_cmd="/home/like/repo/openharmony/out/rk3568/clang_x64/exe.unstripped/clang_x64/arkcompiler/ets_runtime/ark_aot_compiler"
  aot_cmd_args=(
    "--compiler-fast-compile=false"
    "--compiler-trace-bc=true"
    "--enable-pgo-profiler=true"
    "--compiler-pgo-profiler-path=./$file_name.ap"
    "--aot-file=$file_name"
    "$file_path"
  )

  # Print and execute the AOT compilation command
  echo "$aot_cmd ${aot_cmd_args[@]}"
  $aot_cmd ${aot_cmd_args[@]}
else
  # Build the ES2ABC compilation command
  compiler_cmd="/home/like/repo/openharmony/out/rk3568/clang_x64/arkcompiler/ets_frontend/es2abc"
  compiler_cmd_args=(
    "$file_path"
    "--module"
    "--merge-abc"
    "--output ./$file_name.abc"
  )

  # Print and execute the ES2ABC compilation command
  echo "$compiler_cmd ${compiler_cmd_args[@]}"
  $compiler_cmd ${compiler_cmd_args[@]}
fi

# Check if --disassembly flag is provided
if [[ "$@" =~ "--disassembly" ]]; then
  # Build the disassembly command
  disasm_cmd="/home/like/repo/openharmony/out/rk3568/clang_x64/arkcompiler/runtime_core/ark_disasm"
  disasm_cmd_args=(
    "$file_name.abc"
    "$file_name.pa"
  )

  # Print and execute the disassembly command
  echo "$disasm_cmd ${disasm_cmd_args[@]}"
  $disasm_cmd ${disasm_cmd_args[@]}
fi
