COMPILATION_OPTIONS=-Wall

all:
	make lib_static
	make lib_shared
	make exe_static
	make exe_shared
	make exe_dlopen

lib_static: libwc.c
	gcc $(COMPILATION_OPTIONS) $(OPTIMALIZATION) -c libwc.c
	ar cr libwc$(SUFFIX).a libwc.o

lib_shared: libwc.c
	gcc $(COMPILATION_OPTIONS) $(OPTIMALIZATION) -c -fPIC libwc.c
	gcc $(COMPILATION_OPTIONS) $(OPTIMALIZATION) -shared -o libwc.so libwc.o

exe_static: libwc.c wc_repl.c dllmanager.h
	gcc $(COMPILATION_OPTIONS) $(OPTIMALIZATION) wc_repl.c -o wc_repl_static.exe -L . libwc.a

exe_shared: libwc.c wc_repl.c dllmanager.h
	gcc $(COMPILATION_OPTIONS) $(OPTIMALIZATION) wc_repl.c -o wc_repl_shared.exe -L . libwc.so

exe_dlopen: libwc.c wc_repl.c dllmanager.h
	gcc $(COMPILATION_OPTIONS) $(OPTIMALIZATION) wc_repl.c -o wc_repl_dlopen.exe -ldl -D USE_DLL

clean:
	rm -f *.o *.gch *.a *.so *.exe test.in


