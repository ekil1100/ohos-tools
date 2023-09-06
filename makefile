FILE_NAME = $(basename $(file_path))
OHOS_PATH = /home/like/repo/openharmony
ES2ABC = $(OHOS_PATH)/out/rk3568/clang_x64/arkcompiler/ets_frontend/es2abc
VM = $(OHOS_PATH)/out/rk3568/clang_x64/arkcompiler/ets_runtime/ark_js_vm
AOT = $(OHOS_PATH)/out/rk3568/clang_x64/exe.unstripped/clang_x64/arkcompiler/ets_runtime/ark_aot_compiler
DISASM = $(OHOS_PATH)/out/rk3568/clang_x64/arkcompiler/runtime_core/ark_disasm

# all: Compile and execute all files
.PHONY: all
all: abc pgo aot exe disasm

.PHONY: sasm
sasm: abc disasm

# Compile hello-world.js to hello-world.abc
.PHONY: abc
abc: $(file_path)
	$(ES2ABC) $(file_path) --module --merge-abc --output $(FILE_NAME).abc

# PGO sampling for hello-world.abc
.PHONY: pgo
pgo: $(FILE_NAME).abc
	$(VM) --enable-pgo-profiler=true --compiler-pgo-profiler-path=$(FILE_NAME).ap --entry-point=$(FILE_NAME) $(FILE_NAME).abc

# AOT compile hello-world.abc
.PHONY: aot aot-log
aot: $(FILE_NAME).abc $(FILE_NAME).ap
	$(AOT) --compiler-fast-compile=false --compiler-trace-bc=true --enable-pgo-profiler=true --compiler-pgo-profiler-path=$(FILE_NAME).ap --aot-file=$(FILE_NAME) $(FILE_NAME).abc

aot-log: $(FILE_NAME).abc $(FILE_NAME).ap
	$(AOT) --compiler-fast-compile=false --compiler-trace-bc=true --enable-pgo-profiler=true --compiler-pgo-profiler-path=$(FILE_NAME).ap --aot-file=$(FILE_NAME) --log-level=info --compiler-log=all0123 $(FILE_NAME).abc > $(FILE_NAME).log 2>&1

# Execute hello-world.abc
.PHONY: exe
exe: $(FILE_NAME).abc
	$(VM) --entry-point=$(FILE_NAME) --aot-file=$(FILE_NAME) --log-level=info $(FILE_NAME).abc

# Disassemble hello-world.abc
.PHONY: disasm
disasm: $(FILE_NAME).abc
	$(DISASM) $(FILE_NAME).abc $(FILE_NAME).pa

# Clean generated files
.PHONY: clean
clean:
	rm *.abc *.ap *.ai *.an *.pa
