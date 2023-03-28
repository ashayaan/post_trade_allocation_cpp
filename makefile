CC = g++
LD = g++
CFLAGS = -c -O -g
LIBS = -ldl

PROG_OBJS = maintrytry.o readAttributes.o ordersStruct.o parSet.o
//PROG_OBJS = readAttributes.o setAttributes.o readSetMain.o  maintrytry.o postTradeAllocation.o

TGTS = readData.out

$(TGTS):$(PROG_OBJS)
	$(CC) $(LFLAGS) $(PROG_OBJS) $(LIBS) -o $(TGTS)

.SUFFIXES:.cpp
 
.cpp.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f $(TGTS)
	rm -f *.o

