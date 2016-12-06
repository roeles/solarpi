DEPS=Makefile

all: code 

code: bin/scdplus

imgclean:
	rm -f img/*.pdf

outputclean:
	rm -f *.pdf

intermediateclean:
	rm -f *.aux
	rm -f *.bbl
	rm -f *.log
	rm -f *.out
	rm -f *.blg
	rm -f *.nlo
	rm -f *.ist
	rm -f *.nls

clean: imgclean outputclean intermediateclean

%.pdf-view: %.pdf
		xpdf $<

%.pdf-install: %.pdf
	cp -v $< ~/public_html/


#Image generation
img/%.pdf: img/%.svg $(DEPS)
	 rsvg-convert -f pdf -o $@ $<

img/%.svg: plots/%.gnuplot data/%.csv $(DEPS)
	gnuplot -e 'set datafile separator ","; set grid; set terminal svg; set output "$@"; input="$(word 2, $^)"' $<

img/%.svg: plots/%.gnuplot data/%.polar $(DEPS)
	gnuplot -e 'set grid; set terminal svg enhanced; set output "$@"' $<

img/%.svg: img/%.dot $(DEPS)
	dot -Tsvg -Kdot -o $@ $<

#Allow compression
data/%.csv: data/%.csv.xz
	xzcat $< > $@

bin/%: code/%.cpp
	c++ -o $@ $< -lm -lrt -Wall -O3
