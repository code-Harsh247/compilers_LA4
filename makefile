LEX_FILE = poly.l
C_FILE = polyutils.c
BISON_FILE = poly.y
BISON_OUTPUT = poly.tab.c
BISON_HEADER = poly.tab.h
LEX_OUTPUT = lex.yy.c
EXE = ./a.out
INPUT = sample.txt
OUTPUT_FILE = output.txt

all: 
	rm -f $(LEX_OUTPUT) $(EXE) $(BISON_OUTPUT) $(OUTPUT_FILE) $(BISON_HEADER)
	bison -d $(BISON_FILE)
	flex $(LEX_FILE)
	gcc $(C_FILE) $(LEX_OUTPUT) $(BISON_OUTPUT)
	$(EXE) < $(INPUT) > $(OUTPUT_FILE)

post_clean:
	rm -f $(LEX_OUTPUT) $(EXE) $(BISON_OUTPUT) $(OUTPUT_FILE) $(BISON_HEADER)