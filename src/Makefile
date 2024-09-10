assembler : assembler.o data_structures.o error_handling.o first_pass.o machine_code.o parser.o preprocessor.o second_pass.o utilities.o
		gcc -std=c90 -ansi -Wall -pedantic misc_Functions.o assembler.o set.o -o assembler
assembler.o : assembler.c
		gcc -c -ansi -std=c90 -Wall -pedantic assembler.c -o assembler.o
data_structures.o : data_structures.c
		gcc -c -ansi -std=c90 -Wall -pedantic data_structures.c -o data_structures.o
error_handling.o : error_handling.c
		gcc -c -ansi -std=c90 -Wall -pedantic error_handling.c -o error_handling.o
first_pass.o : first_pass.c
		gcc -c -ansi -std=c90 -Wall -pedantic first_pass.c -o first_pass.o
machine_code.o : machine_code.c
		gcc -c -ansi -std=c90 -Wall -pedantic machine_code.c -o machine_code.o
parser.o : parser.c
		gcc -c -ansi -std=c90 -Wall -pedantic parser.c -o parser.o
preprocessor.o : preprocessor.c
		gcc -c -ansi -std=c90 -Wall -pedantic preprocessor.c -o preprocessor.o
second_pass.o : second_pass.c
		gcc -c -ansi -std=c90 -Wall -pedantic second_pass.c -o second_pass.o
utilities.o : utilities.c
		gcc -c -ansi -std=c90 -Wall -pedantic utilities.c -o utilities.o

