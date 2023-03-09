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

create_test_in:
	echo "init 100" > test.in

	(ls -p | grep -v / | sed 's/^/count /') >> test.in

	echo "show 0" >> test.in
	echo "show 1" >> test.in
	echo "show 2" >> test.in

	echo "delete 1" >> test.in
	echo "show 1" >> test.in
	echo "destroy" >> test.in 
	echo "exit" >> test.in

test:
	sudo mv libwc.so /usr/lib
	make create_test_in
	./wc_repl_static.exe < test.in > test_results/results_static_$(O_SUFFIX).txt 2>/dev/null
	./wc_repl_shared.exe < test.in > test_results/results_shared_$(O_SUFFIX).txt 2>/dev/null
	./wc_repl_dlopen.exe < test.in > test_results/results_dynamic_$(O_SUFFIX).txt 2>/dev/null

report:
	make all
	mkdir test_results
	sudo ls > /dev/null
	make test
	rm -f test.in

