all:
	gcc -o zad02_integral.exe zad02_integral.c 

report:
	make all
	echo "" > report.txt
	./zad02_integral.exe 0.000000001 1  >> report.txt
	./zad02_integral.exe 0.000000001 10  >> report.txt
	./zad02_integral.exe 0.000000001 50 >> report.txt
	./zad02_integral.exe 0.000000001 100 >> report.txt
	./zad02_integral.exe 0.000000001 1000 >> report.txt
	./zad02_integral.exe 0.000000001 10000 >> report.txt
	./zad02_integral.exe 0.000000001 100000 >> report.txt
	./zad02_integral.exe 0.000000001 1000000 >> report.txt

clear:
	rm -f *.o *.gch *.a *.so *.exe
