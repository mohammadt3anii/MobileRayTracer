#!/bin/bash

latex -interaction=nonstopmode -shell-escape dissertation
bibtex dissertation
makeglossaries dissertation
pdflatex -interaction=nonstopmode -shell-escape dissertation
pdflatex -interaction=nonstopmode -shell-escape dissertation