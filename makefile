FILE_NAME = $(basename $(file_path))
OHOS_PATH = /home/like/repo/openharmony
ES2ABC = $(OHOS_PATH)/out/rk3568/clang_x64/arkcompiler/ets_frontend/es2abc
VM = $(OHOS_PATH)/out/rk3568/clang_x64/arkcompiler/ets_runtime/ark_js_vm
AOT = $(OHOS_PATH)/out/rk3568/clang_x64/exe.unstripped/clang_x64/arkcompiler/ets_runtime/ark_aot_compiler
DISASM = $(OHOS_PATH)/out/rk3568/clang_x64/arkcompiler/runtime_core/ark_disasm

# all: Compile and execute all files
all: abc pgo aot an disasm

# Compile hello-world.js to hello-world.abc
abc: $(file_path)
	$(ES2ABC) $(file_path) --module --merge-abc --output $(FILE_NAME).abc

# PGO sampling for hello-world.abc
pgo: $(FILE_NAME).abc
	$(VM) --enable-pgo-profiler=true --compiler-pgo-profiler-path=$(FILE_NAME).ap --entry-point=$(FILE_NAME) $(FILE_NAME).abc

# AOT compile hello-world.abc
aot: $(FILE_NAME).abc $(FILE_NAME).ap
	$(AOT) --compiler-fast-compile=false --compiler-trace-bc=true --enable-pgo-profiler=true --compiler-pgo-profiler-path=$(FILE_NAME).ap --aot-file=hello-world $(FILE_NAME).abc

# Execute hello-world.abc
an: $(FILE_NAME).abc
	$(VM) --entry-point=$(FILE_NAME) --aot-file=$(FILE_NAME) --log-level=info $(FILE_NAME).abc

# Disassemble hello-world.abc
disasm: $(FILE_NAME).abc
	$(DISASM) $(FILE_NAME).abc $(FILE_NAME).pa

# Clean generated files
clean:
	rm *.abc *.ap *.ai *.an *.pa

.PHONY: all abc pgo aot an disasm clean
