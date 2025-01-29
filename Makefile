SRCS	= src/Knot_funcs.cpp src/Polynomial_funcs.cpp
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

main.exe							: header.hpp src/main.cpp ${OBJS}
	g++ src/main.cpp ${OBJS} -o main.exe

src/Knot_funcs.o					: header.hpp src/Knot_funcs.cpp
	g++ -c src/Knot_funcs.cpp -o src/Knot_funcs.o

src/Polynomial_funcs.o				: header.hpp src/Polynomial_funcs.cpp
	g++ -c src/Polynomial_funcs.cpp -o src/Polynomial_funcs.o

rb_in_gen.exe						: header.hpp src/rubberband_input_generator.cpp
	g++ src/rubberband_input_generator.cpp -o rb_in_gen.exe

clean		:
	rm -f src/*.o

fclean		: clean
	rm -f *.out

.PHONY		: clean fclean
