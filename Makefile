all:
	mkdir -p build
	g++ src/main.cpp -o build/my_program
	bash -c "display <(build/my_program)"

compare:
	mkdir -p build
	g++ src/main.cpp -o build/my_program
	# Combine AA and non-AA image vertically.
	# bash -c "convert -append <(build/my_program) <(build/my_program --no-aa) >(display)"
	# Same command but using `montage` instead of `convert`.
	# bash -c "montage -mode concatenate -tile 1x <(build/my_program) <(build/my_program --no-aa) >(display)"
	# Lay out horizontally instead of vertically.
	bash -c "montage -mode concatenate -tile x1 <(build/my_program) <(build/my_program --no-aa) >(display)"
