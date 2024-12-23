all: ClientCompiled ServerCompiled
ClientCompiled: Client.c Parser.c Main.c
	gcc -Wall -o ClientCompiled Client.c Parser.c Main.c
ServerCompiled: Student.c Server.c Course.c Writer.c
	gcc -Wall -o ServerCompiled Student.c Server.c Course.c Writer.c
clean:
	-rm ClientCompiled ServerCompiled