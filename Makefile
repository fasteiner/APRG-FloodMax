EXECS=mpi_floodmax mpi_floodmax_opt
MPICC?=mpicc.mpich

all: ${EXECS}

mpi_floodmax: src/FloodMax.c
	${MPICC} -o out/mpi_floodmax src/FloodMax.c
mpi_floodmax_opt: src/FloodMaxOpt.c
	${MPICC} -o out/mpi_floodmax_opt src/FloodMaxOpt.c

clean:
	rm -f out/mpi_floodmax
	rm -f out/mpi_floodmax_opt
