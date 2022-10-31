CXX = c++

TARGET = benchmark
OBJDIR = obj

SOURCES = main.cpp
OBJECTS = $(addprefix obj/,$(SOURCES:.cpp=.o))
DEPENDENCIES = $(OBJECTS:.o=.d)

CXXFLAGS = -Wall -Wextra -Werror -O3 -std=c++98

all: $(TARGET)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(TARGET)

re:
	$(MAKE) fclean
	$(MAKE) all

run: all
	@./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $@

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.cpp | obj
	$(CXX) $(CXXFLAGS) -MMD -Iinclude -Ift_containers-terminator/tests -c $< -o $@
	
test:
	cd test && sh test_all.sh

.PHONY: all clean fclean re test

-include $(DEPENDENCIES)
