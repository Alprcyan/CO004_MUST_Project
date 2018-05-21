CPPC=g++
CPPFLAGS=-Wpedantic -std=c++11 -Wall -Wno-unused-private-field

OBJS=filereader.o job.o logger.o main.o \
	parser.o process.o scheduler.o policy.o

all : $(OBJS)
	$(CPPC) $(CPPFLAGS) $(OBJS) -o scheduler

filereader.o : filereader.cc filereader.h schedulerutils.h
	$(CPPC) $(CPPFLAGS) -c filereader.cc

job.o : job.cc job.h schedulerutils.h scheduler.h
	$(CPPC) $(CPPFLAGS) -c job.cc

logger.o : logger.cc logger.h schedulerutils.h
	$(CPPC) $(CPPFLAGS) -c logger.cc

main.o : main.cc filereader.h job.h logger.h parser.h \
		scheduler.h schedulerutils.h
	$(CPPC) $(CPPFLAGS) -c main.cc

parser.o : parser.cc parser.h schedulerutils.h
	$(CPPC) $(CPPFLAGS) -c parser.cc

process.o : process.cc process.h schedulerutils.h job.h
	$(CPPC) $(CPPFLAGS) -c process.cc

scheduler.o : scheduler.cc scheduler.h schedulerutils.h
	$(CPPC) $(CPPFLAGS) -c scheduler.cc

policy.o : policy.cc policy.h schedulerutils.h
	$(CPPC) $(CPPFLAGS) -c policy.cc

clean :
	rm scheduler *.o

