EXECS=mpi_floodmax mpi_floodmax_opt
MPICC?=mpicc.mpich

all: ${EXECS}

mpi_floodmax: FloodMax.c
	${MPICC} -o mpi_floodmax FloodMax.c
mpi_floodmax_opt: FloodMaxOpt.c
	${MPICC} -o mpi_floodmax_opt FloodMaxOpt.c

clean:
	rm -f ${EXECS}