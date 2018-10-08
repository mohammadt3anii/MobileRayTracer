#!/bin/bash

pdflatex -shell-escape dissertation
bibtex dissertation
makeglossaries dissertation
pdflatex -shell-escape dissertation
pdflatex -shell-escape dissertation