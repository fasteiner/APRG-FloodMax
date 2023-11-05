EXECS=mpi_floodmax mpi_floodmax_opt
MPICC?=mpicc.mpich

# Define the output directory
OUTDIR=out

# Create the output directory if it doesn't exist
$(OUTDIR):
	mkdir -p $(OUTDIR)

all: ${EXECS}

mpi_floodmax: src/FloodMax.c $(OUTDIR)
	${MPICC} -o $(OUTDIR)/mpi_floodmax src/FloodMax.c

mpi_floodmax_opt: src/FloodMaxOpt.c $(OUTDIR)
	${MPICC} -o $(OUTDIR)/mpi_floodmax_opt src/FloodMaxOpt.c

clean:
	rm -f $(OUTDIR)/mpi_floodmax
	rm -f $(OUTDIR)/mpi_floodmax_opt
