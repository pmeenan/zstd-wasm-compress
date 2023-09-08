all:
	emcc -v
	AR=emar CC=emcc $(MAKE) -C zstd/lib libzstd.a MOREFLAGS="-Werror"
	emcc --bind -I ./zstd/lib/ src/zstd-wasm-compress.cpp -o ./bin/zstd.js zstd/lib/libzstd.a -s EXPORT_ES6=1 -s MODULARIZE -s EXPORT_NAME="zstd"
clean:
	$(MAKE) -C zstd clean
