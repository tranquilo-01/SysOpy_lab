integrator:
	gcc -o integrator.exe the_grand_integrator.c

worker:
	gcc -o the_little_worker the_little_worker.c 

runtest:
	make integrator
	make worker
	./integrator.exe 0.01 10