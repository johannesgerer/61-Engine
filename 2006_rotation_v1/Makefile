LDLIBS=-lstdc++ -lboost_serialization -lm
CXX ?= g++
IDIR = 
CXXFLAGS = -Wall -std=c++11 $(IDIR) \
           # -H
					 # -Winvalid-pch\
					 -Winline \
	         -O0 \
	          # -g -DDEBUG  -D_GLIBCXX_DEBUG


prog = 
sources = $(addsuffix .cpp, $(prog) tests)


run: $(prog)
	./$(prog)

filt: $(prog)
	./$(prog) | c++filt -t

debug: $(prog)
	./$(prog) -s

$(prog): $(sources:.cpp=.o)



clean:
	rm -f $(prog) *.o *.d


#derive the dependencies of every compilation unit
# (for automatic recompilation on changes)

%.d: %
	$(COMPILE.cpp) -MG -MM '$<' \
	 -MT '$(patsubst %.h, %.h.gch, $(<:.cpp=.o)) $@' -MF '$@'

include $(addsuffix .d, $(sources) )

MARKDOWN = pandoc --from markdown_github --to html --standalone

markdowns = $(patsubst %.md,%.html,$(wildcard *.md))

md: $(markdowns)

%.html: %.md
		$(MARKDOWN) $< --output $@
