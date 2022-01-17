all = server.o ./modul/socket.o ./modul/sqlite.o ./modul/op.o

Ser : $(all)
	cc $(all) -o Ser -lsqlite3 -pthread

.PYTHON:clean
clean:
	rm -rf $(all) core Ser 
