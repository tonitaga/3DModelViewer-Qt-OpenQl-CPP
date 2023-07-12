CC=g++
CFLAGS=-Wall -Werror -Wextra -std=c++17
TESTFLAGS=-lgtest

CC_FILES=\
./model/helper/FileReader/filereader.cc\
./model/helper/FileReader/filechecker.cc\
./model/helper/Normalize/normalizer.cc\
./model/helper/TransformModel/transformmodel.cc\
./model/helper/VertexData/vertexdata.cc\

CC_TEST_FILE=tests/gtests.cc


.PHONY: all

all: install


install:
	@make clean
	@mkdir ./build
	@cd view && qmake && make -j6 && make clean && rm Makefile && cd ../ && mv view/3DViewerView.app build
	@echo ==Install finished==
	@echo ==To open application write 'make open'==

uninstall:
	@rm -rf build/
	@echo ==Uninstall finished==

unzipp:
	@cd ../misc/; unzip -q models.zip -d ./; unzip -q texture.zip -d ./
	@cd ../src


test:
	@cp ../misc/models/cube.obj ./tests
	@cp ../misc/models/cube2.obj ./tests
	@cp ../misc/models/FinalBaseMesh.obj ./tests
	@cp ../misc/models/awp.obj ./tests
	@$(CC) $(CFLAGS) $(TESTFLAGS) --coverage $(CC_TEST_FILE) $(CC_FILES) -o test
	@./test
	@rm -rf ./tests/cube.obj ./tests/cube2.obj ./tests/FinalBaseMesh.obj ./tests/awp.obj
	@make clean

gcov_report:
	@cp ../misc/models/cube.obj ./tests
	@cp ../misc/models/cube2.obj ./tests
	@cp ../misc/models/FinalBaseMesh.obj ./tests
	@cp ../misc/models/awp.obj ./tests
	@$(CC) $(CFLAGS) $(TESTFLAGS) --coverage $(CC_TEST_FILE) $(CC_FILES) -o gcov_report
	@chmod +x *
	@./gcov_report
	@lcov -t "gcov_report" -o gcov_report.info --no-external -c -d .
	@genhtml -o report/ gcov_report.info
	@open ./report/index.html
	@rm -rf ./tests/cube.obj ./tests/cube2.obj ./tests/FinalBaseMesh.obj ./tests/awp.obj
	@make clean


dvi:
	@open info.html

dist:
	@rm -rf Archive_3DViewer_v2.1/
	@mkdir Archive_3DViewer_v2.1/
	@mkdir Archive_3DViewer_v2.1/src
	@mv ./build/3DViewerView.app Archive_3DViewer_v2.1/src/
	@tar cvzf Archive_3DViewer_v2.1.tgz Archive_3DViewer_v2.1/
	@rm -rf Archive_3DViewer_v2.1/
	@echo ==Archive was created==

bist:
	@rm -rf Archive_3DViewer_v2.1.tgz
	@echo ==Archive was removed==

open:
	@open ./build/3DViewerView.app

check:
	@cp ../materials/linters/.clang-format .clang-format
	@clang-format -i controller/*.cc controller/*.h

	@clang-format -i model/facade/*.h model/facade/*.cc
	@clang-format -i model/helper/FileReader/*.h model/helper/FileReader/*.cc
	@clang-format -i model/helper/Normalize/*.h model/helper/Normalize/*.cc
	@clang-format -i model/helper/ObjectRecorder/*.h model/helper/ObjectRecorder/*.cc
	@clang-format -i model/helper/TransformModel/*.h model/helper/TransformModel/*.cc
	@clang-format -i model/helper/VertexData/*.h model/helper/VertexData/*.cc

	@clang-format -i view/*.cc view/*.h
	@clang-format -i tests/*.cc
	@clang-format -i model/helper/StopWatch/*.h

	@clang-format -n controller/*.cc controller/*.h

	@clang-format -n model/facade/*.h model/facade/*.cc
	@clang-format -n model/helper/FileReader/*.h model/helper/FileReader/*.cc
	@clang-format -n model/helper/Normalize/*.h model/helper/Normalize/*.cc
	@clang-format -n model/helper/ObjectRecorder/*.h model/helper/ObjectRecorder/*.cc
	@clang-format -n model/helper/TransformModel/*.h model/helper/TransformModel/*.cc
	@clang-format -n model/helper/VertexData/*.h model/helper/VertexData/*.cc

	@clang-format -n view/*.cc view/*.h
	@clang-format -n tests/*.cc
	@clang-format -n model/helper/StopWatch/*.h

	@rm .clang-format
	@echo ==No style errors==


clean:
	@rm -rf *.{a,o,dSYM,out}
	@rm -rf $(EXECUTABLE)
	@rm -rf gcov_report
	@rm -rf report
	@rm -rf CPPLINT.cppfg
	@rm -rf *.gcda
	@rm -rf *.gcno
	@rm -rf *.info
	@rm -rf test
	@rm -rf Archive_3DViewer_v2.1 
	@rm -rf *tgz
	@rm -rf build
	@rm -rf gcov_test
	@rm -rf ../misc/__MACOSX
	@echo ==Clean was finished==

leaks:
	@echo -------------------MEMORY_LEAKS-------------------
	@cp ../misc/models/cube.obj ./tests
	@cp ../misc/models/cube2.obj ./tests
	@cp ../misc/models/FinalBaseMesh.obj ./tests
	@cp ../misc/models/awp.obj ./tests
	@$(CC) $(CFLAGS) $(TESTFLAGS) $(CC_TEST_FILE) $(CC_FILES) -lcheck -o test
	@CK_FORK=no leaks --atExit -- ./test
	@rm -rf ./tests/cube.obj ./tests/cube2.obj ./tests/FinalBaseMesh.obj ./tests/awp.obj
	@make clean
