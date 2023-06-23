#!/bin/sh

umask 022

myflags="-flto -O2 -s TOTAL_STACK=8MB"
#myflags="-flto -O2 -s TOTAL_STACK=128MB -s INITIAL_MEMORY=256MB"
#myflags="-g4 -fsanitize=address -fsanitize=undefined"

emsdk-env em++  \
	-std=c++17 $myflags -Wall -Wno-unused-function -pedantic \
	-s EXPORTED_FUNCTIONS=_parse  -s EXPORTED_RUNTIME_METHODS=ccall,cwrap \
	-s ALLOW_MEMORY_GROWTH=1 \
	-I../src -DLALR_NO_THREADS \
	../src/lalr/ErrorPolicy.cpp \
	../src/lalr/Grammar.cpp \
	../src/lalr/GrammarAction.cpp \
	../src/lalr/GrammarCompiler.cpp \
	../src/lalr/GrammarGenerator.cpp \
	../src/lalr/GrammarParser.cpp \
	../src/lalr/GrammarState.cpp \
	../src/lalr/GrammarSymbol.cpp \
	../src/lalr/GrammarSymbolSet.cpp \
	../src/lalr/GrammarTransition.cpp \
	../src/lalr/RegexAction.cpp \
	../src/lalr/RegexCharacter.cpp \
	../src/lalr/RegexCompiler.cpp \
	../src/lalr/RegexGenerator.cpp \
	../src/lalr/RegexItem.cpp \
	../src/lalr/RegexNode.cpp \
	../src/lalr/RegexNodeLess.cpp \
	../src/lalr/RegexParser.cpp \
	../src/lalr/RegexState.cpp \
	../src/lalr/RegexStateLess.cpp \
	../src/lalr/RegexSyntaxTree.cpp \
	../src/lalr/RegexToken.cpp \
	../src/lalr/RegexTransition.cpp \
	lalr_playground.cpp \
	-o lalr_playground.js

# --pre-js chpeg-pre.js \
# -g2 -gsource-map --source-map-base='./'
