BINS = ray1 ray2 ray3 ray4 ray5
HEADERS = $(wildcard *.h)

CXXFLAGS += -g -Wall
LDFLAGS += -lm

all: $(BINS)

$(BINS): %: %.cc $(HEADERS)
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(BINS)
