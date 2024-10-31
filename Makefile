all:
	emcc -v
	AR=emar CC=emcc $(MAKE) -C zstd/lib libzstd.a MOREFLAGS="-Werror"
	emcc --bind -I ./zstd/lib/ --pre-js './src/pre.js' src/zstd-wasm-compress.cpp -o ./bin/zstdlib.js zstd/lib/libzstd.a -s DYNAMIC_EXECUTION=0 -s TEXTDECODER=0 -s MODULARIZE=1 -s FILESYSTEM=0 -s ENVIRONMENT="web" -s EXPORTED_RUNTIME_METHODS='["cwrap", "setValue"]' -s EXPORTED_FUNCTIONS='["_malloc"]' -s ALLOW_MEMORY_GROWTH=1 -s EXPORT_NAME="zstdlib"
clean:
	$(MAKE) -C zstd clean
	rm -rf ./bin/*
