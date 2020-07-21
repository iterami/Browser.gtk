all: setup

setup: c/main.c
	mkdir -p ~/.iterami
	mkdir -p ~/.iterami/css
	cp ../common/css/gtk.css ~/.iterami/css
	gcc c/main.c `pkg-config --cflags --libs gtk+-3.0` -o3 ~/.iterami/browser

clean:
	rm ~/.iterami/browser
