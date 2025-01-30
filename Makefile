SRCS	= src/functions.cpp src/TreeNode_funcs.cpp src/Knot_funcs.cpp src/Polynomial_funcs.cpp
OBJS	= ${SRCS:.cpp=.o}
# PROGRAM	= a${ID}.out
# 
# all		: ${PROGRAM}
# 
# ${PROGRAM}	: ${OBJS}
# 	g++ ${OBJS} -o ${PROGRAM}
# 
#${OBJS}		: ${SRCS}
# 	g++ -c ${SRCS}

calc_jones_poly.exe					: header.hpp src/calc_jones_poly.cpp ${OBJS}
	g++ src/calc_jones_poly.cpp ${OBJS} -o calc_jones_poly.exe

src/functions.o						: header.hpp src/functions.cpp
	g++ -c src/functions.cpp -o src/functions.o

src/TreeNode_funcs.o				: header.hpp src/TreeNode_funcs.cpp
	g++ -c src/TreeNode_funcs.cpp -o src/TreeNode_funcs.o

src/Knot_funcs.o					: header.hpp src/Knot_funcs.cpp
	g++ -c src/Knot_funcs.cpp -o src/Knot_funcs.o

src/Polynomial_funcs.o				: header.hpp src/Polynomial_funcs.cpp
	g++ -c src/Polynomial_funcs.cpp -o src/Polynomial_funcs.o

rb_in_gen.exe						: header.hpp src/rubberband_input_generator.cpp
	g++ src/rubberband_input_generator.cpp -o rb_in_gen.exe

clean		:
	rm -f src/*.o

fclean		: clean
	rm -f *.exe

.PHONY		: clean fclean
