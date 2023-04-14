integrator:
	gcc -o integrator.exe the_grand_integrator.c

worker:
	gcc -o the_little_worker the_little_worker.c 

runtest:
	make integrator
	make worker
	./integrator.exe 0.01 10

report:
	make integrator
	make worker
	echo "" > report.txt
	./integrator.exe 0.000000001 1  >> report.txt
	./integrator.exe 0.000000001 10  >> report.txt
	./integrator.exe 0.000000001 50 >> report.txt
	./integrator.exe 0.000000001 100 >> report.txt
	./integrator.exe 0.000000001 1000 >> report.txt
	./integrator.exe 0.000000001 10000 >> report.txt
	./integrator.exe 0.000000001 100000 >> report.txt
	./integrator.exe 0.000000001 1000000 >> report.txt