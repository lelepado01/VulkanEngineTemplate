CXX = g++ 
SHADER_CXX = glslc

CFLAGS = -g -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread 

ONLY_HEADERS = $(wildcard *.h)

SRC_FILES = $(subst ./main.cpp,,$(shell find . -type f -name "*.cpp"))
OBJ_FILES = $(subst .cpp,.o,$(SRC_FILES))

SRC_SHADER = 	$(shell find . -type f -name "*.vert") \
				$(shell find . -type f -name "*.frag") \
				$(shell find . -type f -name "*.tese") \
				$(shell find . -type f -name "*.tesc") \
				$(shell find . -type f -name "*.comp")
				
OBJ_SHADER = $(addsuffix .spv,$(SRC_SHADER))

NAME = VulkanEngineTemplate

.cpp.o: %.cpp %.h $(ONLY_HEADERS)
	$(CXX) $(CFLAGS) -o $@ -c $<

%.vert.spv: %.vert
	$(SHADER_CXX) $^ -o $@ 

%.frag.spv: %.frag
	$(SHADER_CXX) $^ -o $@ 

%.tese.spv: %.tese
	$(SHADER_CXX) $^ -o $@ 

%.tesc.spv: %.tesc
	$(SHADER_CXX) $^ -o $@ 

%.comp.spv: %.comp
	$(SHADER_CXX) $^ -o $@ 

$(NAME): main.cpp $(OBJ_FILES) $(OBJ_SHADER)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJ_FILES) main.cpp

.PHONY: run clean

run: $(NAME)
	./$(NAME)

clean:  
	rm -f $(OBJ_SHADER)
	rm -f $(OBJ_FILES)