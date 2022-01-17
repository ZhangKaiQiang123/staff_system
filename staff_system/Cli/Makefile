all = client.o ./modul/socket.o ./modul/menu.o ./modul/op.o

Cli: $(all)
	cc $(all) -o Cli

.PYTHON:clean
clean:
	rm -rf $(all) core Cli
