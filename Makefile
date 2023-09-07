all:
	emcc -v
	CC=emcc $(MAKE) -C zstd/lib libzstd.a MOREFLAGS="-Werror"
	emcc -O3 --bind -std=c++17 -I ./zstd/lib/ src/zstd-wasm-compress.cpp -o ./bin/zstd.js zstd/lib/libzstd.a

clean:
	$(MAKE) -C zstd clean
