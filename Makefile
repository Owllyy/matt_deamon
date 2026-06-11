NAME        =       Matt_daemon

SRCS        =       main.cpp Deamon.cpp Matt.cpp file.cpp Tintin_reporter.cpp					
OBJS        =       ${SRCS:.cpp=.o}

CXXFLAGS    =       -Wall -Wextra -Werror --std=c++17

CXX			=		clang++

DEP			=		Deamon.hpp Tintin_reporter.hpp file.hpp

%.o : %.cpp $(DEP)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all :       $(NAME)

clean :
	$(RM) $(OBJS)

fclean :    clean
	$(RM) $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all

$(NAME) :   $(OBJS) $(DEP)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

.PHONY :    all fclean clean re