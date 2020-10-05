all:
	mkdir -p build
	g++ src/main.cpp -o build/my_program
	bash -c "display <(build/my_program)"

debug:
	mkdir -p build
	g++ -g3 -O0 src/main.cpp -o build/my_program
	gdb ./build/my_program

compare:
	mkdir -p build
	g++ src/main.cpp -o build/my_program
	# Combine AA and non-AA image vertically.
	# bash -c "convert -append <(build/my_program) <(build/my_program --no-aa) >(display)"
	# Same command but using `montage` instead of `convert`.
	# bash -c "montage -mode concatenate -tile 1x <(build/my_program) <(build/my_program --no-aa) >(display)"
	# Lay out horizontally instead of vertically.
	# bash -c "montage -mode concatenate -tile x1 <(build/my_program) <(build/my_program --no-aa) >(display)"
	# Adding more images and annotate them!
	bash -c "convert -append \
	  <(montage -mode concatenate -tile x1 \
		  <(convert <(build/my_program) -fill black -pointsize 17 -gravity Northwest -draw \"text 0,0 'Original (True Lambertian)'\" >(cat)) \
		  <(convert <(build/my_program --hacky_lambertian) -fill black -pointsize 17 -gravity Northwest -draw \"text 0,0 'Approx Lambertian'\" >(cat)) \
		  <(convert <(build/my_program --true_lambertian) -fill black -pointsize 17 -gravity Northwest -draw \"text 0,0 'True Lambertian'\" >(cat)) \
		  <(convert <(build/my_program --alt_approx_lambertian) -fill black -pointsize 17 -gravity Northwest -draw \"text 0,0 'Alt Approx Lambertian'\" >(cat)) \
		  >(cat)) \
		<(montage -mode concatenate -tile x1 \
		  <(convert <(build/my_program --normals) -fill black -pointsize 17 -gravity Northwest -draw \"text 0,0 'Normals'\" >(cat)) \
		  <(convert <(build/my_program --no-aa) -fill black -pointsize 17 -gravity Northwest -draw \"text 0,0 'No AA'\" >(cat)) \
		  <(convert <(build/my_program --no-gamma-correct) -fill black -pointsize 17 -gravity Northwest -draw \"text 0,0 'No Gamma Correction'\" >(cat)) \
			>(cat)) \
		>(display)"
