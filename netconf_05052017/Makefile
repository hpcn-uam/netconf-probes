#
# Makefile for libnetconf example applications
#
# Author: Radek Krejci <rkrejci@cesnet.cz>
# Copyright (c) 2012-2014 CESNET, z.s.p.o.
#

CC      = gcc
CFLAGS  = -Wall -g
#INCLUDE = -I../../src/ 
LIB     =  -levent -lnetconf
#LIBPATH	= -L../../.libs/
TARGETS = cliente server descarga_fichero pares_paq tren_paq listen iperf fprobe_start fprobe_result stop_iperf3 information ifconfig ping stop_fprobe

all: $(TARGETS)

cliente: client.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
descarga_fichero: descarga_fichero.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
server: server.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
pares_paq: pares_paq.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
tren_paq: tren_paq.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
listen: listen.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
iperf: iperf.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
fprobe_start: fprobe_start.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
fprobe_result: fprobe_result.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
stop_iperf3: stop_iperf3.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
information: information.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
ifconfig: ifconfig.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
ping: ping.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
stop_fprobe: stop_fprobe.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBPATH) $(LIB)
clean:
	rm -f *.o
	rm -f $(TARGETS)


