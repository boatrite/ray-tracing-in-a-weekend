all:
	mkdir -p build
	g++ src/main.cpp -o build/my_program
	build/my_program > build/output.ppm && feh build/output.ppm
