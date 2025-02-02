SRCS_DIR=src
OBJ_DIR =obj
SRCS    = ${SRCS_DIR}/functions.cpp ${SRCS_DIR}/TreeNode_funcs.cpp ${SRCS_DIR}/Knot_funcs.cpp ${SRCS_DIR}/Polynomial_funcs.cpp
OBJS    = ${SRCS:${SRCS_DIR}/%.cpp=${OBJ_DIR}/%.o}
CXXFLAGS = -std=c++11
CXX = g++

calc_jones_poly.exe					: header.hpp src/calc_jones_poly.cpp ${OBJS}
	${CXX} ${CXXFLAGS} src/calc_jones_poly.cpp ${OBJS} -o calc_jones_poly.exe

obj/functions.o						: header.hpp src/functions.cpp
	${CXX} ${CXXFLAGS} -c src/functions.cpp -o obj/functions.o

obj/TreeNode_funcs.o				: header.hpp src/TreeNode_funcs.cpp
	${CXX} ${CXXFLAGS} -c src/TreeNode_funcs.cpp -o obj/TreeNode_funcs.o

obj/Knot_funcs.o					: header.hpp src/Knot_funcs.cpp
	${CXX} ${CXXFLAGS} -c src/Knot_funcs.cpp -o obj/Knot_funcs.o

obj/Polynomial_funcs.o				: header.hpp src/Polynomial_funcs.cpp
	${CXX} ${CXXFLAGS} -c src/Polynomial_funcs.cpp -o obj/Polynomial_funcs.o

rb_in_gen.exe						: header.hpp src/rubberband_input_generator.cpp
	${CXX} ${CXXFLAGS} src/rubberband_input_generator.cpp -o rb_in_gen.exe

clean		:
	rm -f obj/*.o

fclean		: clean
	rm -f *.exe

.PHONY		: clean fclean