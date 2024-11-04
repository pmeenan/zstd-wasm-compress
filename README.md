# zstd-wasm-compress
Compress-only wasm bindings for Zstandard for running on Cloudflare workers.

# Usage

## Loading/Initializing

Loading wasm in Cloudflare requires a few extra hoops since the source file is not "loadable" from standard emscripten loaders.

```javascript
import zstdlib from "../zstd-wasm-compress/bin/zstdlib.js";
import zstdwasm from "../zstd-wasm-compress/bin/zstdlib.wasm";

// Internal globals for managing state while waiting for the zstd wasm to load
let zstd = null;
let zstdLoaded = null;

export default {
    async fetch(request, env, ctx) {
        zstdInit(ctx).catch(E => console.log(E));
        ...
    }
}

async function zstdInit() {
  // we send our own instantiateWasm function
  // to the zstdlib module
  // so we can initialize the WASM instance ourselves
  // since Workers puts your wasm file in global scope
  // as a binding. In this case, this binding is called
  // `wasm` as that is the name Wrangler uses
  // for any uploaded wasm module
  if (zstd === null && zstdLoaded === null) {
    let resolve;
    zstdLoaded = new Promise((res, rej) => {
      resolve = res;
    });
    // Keep the request alive until wasm loads
    ctx.waitUntil(zstdLoaded);
    zstd = await zstdlib({
      instantiateWasm(info, receive) {
        let instance = new WebAssembly.Instance(zstdwasm, info);
        receive(instance);
        return instance.exports;
      },
      locateFile(path, scriptDirectory) {
        // scriptDirectory is undefined, so this is a
        // no-op to avoid exception "TypeError: Invalid URL string."
        return path
      },
    }).catch(E => console.log(E));
    postInit();
    resolve(true);
  }
}
```