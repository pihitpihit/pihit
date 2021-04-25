
all: list

platform:
	rm -rf ../src/CMakefiles ../src/CMakeCache.txt
	mkdir -p cmakes
	cd cmakes; cmake ../../src
	$(MAKE) -s -C cmakes -j

list:
	@echo Usage:
	@echo "\tmake list"
	@echo "\tmake platform"

