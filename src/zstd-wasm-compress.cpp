#include <stdlib.h>    // malloc, free, exit
#include <stdio.h>     // fprintf, perror, fopen, etc.
#include <string.h>    // strlen, strcat, memset, strerror
#include <errno.h>     // errno
#include <sys/stat.h>  // stat
#define ZSTD_STATIC_LINKING_ONLY
#include <zstd.h>      // presumes zstd library is installed

#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

size_t _ZSTD_compress(unsigned int output_ptr, int num_bytes, unsigned int input_ptr, int num_bytes_in, int level) {
	return ZSTD_compress((uint8_t *) output_ptr, num_bytes, (uint8_t *) input_ptr, num_bytes_in, level);
}

unsigned int _ZSTD_createCCtx(void) {
  return (unsigned int)ZSTD_createCCtx();
}

size_t _ZSTD_freeCCtx(unsigned int cctx) {
  return ZSTD_freeCCtx((ZSTD_CCtx*) cctx);
}

size_t _ZSTD_CCtx_reset(unsigned int cctx, ZSTD_ResetDirective reset) {
  return ZSTD_CCtx_reset((ZSTD_CCtx*) cctx, reset);
}

int ZSTD_cParam_getBounds_lower(ZSTD_cParameter cParam) {
  return ZSTD_cParam_getBounds(cParam).lowerBound;
}

int ZSTD_cParam_getBounds_upper(ZSTD_cParameter cParam) {
  return ZSTD_cParam_getBounds(cParam).upperBound;
}

size_t _ZSTD_CCtx_setParameter(unsigned int cctx, ZSTD_cParameter param, int value) {
  return ZSTD_CCtx_setParameter((ZSTD_CCtx*) cctx, param, value);
}

size_t _ZSTD_compressCCtx(unsigned int cctx, unsigned int dst, size_t dstCapacity, unsigned int src, size_t srcSize, int compressionLevel) {
  return ZSTD_compressCCtx((ZSTD_CCtx*) cctx, (void*) dst, dstCapacity, (void*) src, srcSize, compressionLevel);
}

size_t _ZSTD_compress2(unsigned int cctx, unsigned int dst, size_t dstCapacity, unsigned int src, size_t srcSize) {
  return ZSTD_compress2((ZSTD_CCtx*) cctx, (void*) dst, dstCapacity, (const void*) src, srcSize);
}

size_t _ZSTD_compressStream2(unsigned int cctx, ZSTD_outBuffer &output, ZSTD_inBuffer &input, ZSTD_EndDirective endOp) {
  return ZSTD_compressStream2( (ZSTD_CCtx*) cctx, &output, &input, endOp);
}

// Dictionary
size_t _ZSTD_compress_usingDict(unsigned int ctx, unsigned int dst, size_t dstCapacity, unsigned int src, size_t srcSize, unsigned int dict,size_t dictSize, int compressionLevel) {
  return ZSTD_compress_usingDict((ZSTD_CCtx*) ctx, (void*) dst, dstCapacity, (const void*) src, srcSize, (const void*) dict, dictSize, compressionLevel);
}

unsigned int _ZSTD_createCDict(unsigned int dictBuffer, size_t dictSize, int compressionLevel) {
  return (unsigned int)ZSTD_createCDict((const void*) dictBuffer, dictSize, compressionLevel);
}

size_t _ZSTD_freeCDict(unsigned int CDict) {
  return ZSTD_freeCDict((ZSTD_CDict*) CDict);
}

size_t _ZSTD_compress_usingCDict(unsigned int cctx, unsigned int dst, size_t dstCapacity, unsigned int src, size_t srcSize, unsigned int cdict) {
  return ZSTD_compress_usingCDict((ZSTD_CCtx*) cctx, (void*) dst, dstCapacity, (const void*) src, srcSize, (const ZSTD_CDict*) cdict);
}

unsigned int _ZSTD_createCDict_byReference(unsigned int dictBuffer, size_t dictSize, int compressionLevel) {
  return (unsigned int)ZSTD_createCDict_byReference((const void*)dictBuffer, dictSize, compressionLevel);
}

size_t _ZSTD_CCtx_loadDictionary_byReference(unsigned int cctx, unsigned int dict, size_t dictSize) {
  return ZSTD_CCtx_loadDictionary_byReference((ZSTD_CCtx*) cctx, (const void*) dict, dictSize);
}

size_t _ZSTD_CCtx_refCDict(unsigned int cctx, unsigned int cdict) {
  return ZSTD_CCtx_refCDict((ZSTD_CCtx*) cctx, (const ZSTD_CDict*)cdict);
}

size_t _ZSTD_CCtx_refPrefix(unsigned int cctx, unsigned int prefix, size_t prefixSize) {
  return ZSTD_CCtx_refPrefix((ZSTD_CCtx*) cctx, (const void*) prefix, prefixSize);
}

// Structure setters/getters
static unsigned int ZSTD_inBuffer_src_getter(const ZSTD_inBuffer &inBuffer) {
    return (unsigned int)inBuffer.src;
}

static void ZSTD_inBuffer_src_setter(ZSTD_inBuffer &inBuffer, unsigned int src) {
    inBuffer.src = (void *)src;
}

static unsigned int ZSTD_outBuffer_dst_getter(const ZSTD_outBuffer &outBuffer) {
    return (unsigned int)outBuffer.dst;
}

static void ZSTD_outBuffer_dst_setter(ZSTD_outBuffer &outBuffer, unsigned int dst) {
    outBuffer.dst = (void *)dst;
}

EMSCRIPTEN_BINDINGS(zstdwasm) {
  // Wrapped functions
	function("compress", &_ZSTD_compress);
	function("compress2", &_ZSTD_compress2);
	function("createCCtx", &_ZSTD_createCCtx);
	function("freeCCtx", &_ZSTD_freeCCtx);
	function("CCtx_reset", &_ZSTD_CCtx_reset);
	function("cParam_getBounds_lower", &ZSTD_cParam_getBounds_lower);
	function("cParam_getBounds_upper", &ZSTD_cParam_getBounds_upper);
	function("CCtx_setParameter", &_ZSTD_CCtx_setParameter);
	function("compressCCtx", &_ZSTD_compressCCtx);
  function("compressStream2", &_ZSTD_compressStream2);

	function("compress_usingDict", &_ZSTD_compress_usingDict);
	function("createCDict", &_ZSTD_createCDict);
	function("freeCDict", &_ZSTD_freeCDict);
	function("compress_usingCDict", &_ZSTD_compress_usingCDict);
	function("createCDict_byReference", &_ZSTD_createCDict_byReference);
  function("CCtx_loadDictionary_byReference", &_ZSTD_CCtx_loadDictionary_byReference);
	function("CCtx_refCDict", &_ZSTD_CCtx_refCDict);
  function("CCtx_refPrefix", &_ZSTD_CCtx_refPrefix);

	// Helper functions
	function("compressBound", &ZSTD_compressBound);
	function("isError", &ZSTD_isError);
  function("maxCLevel", &ZSTD_maxCLevel);
	function("versionNumber", &ZSTD_versionNumber);
	function("CStreamInSize", &ZSTD_CStreamInSize);
	function("CStreamOutSize", &ZSTD_CStreamOutSize);

  // ZSTD_ResetDirective
  enum_<ZSTD_ResetDirective>("ResetDirective")
    .value("reset_session_only", ZSTD_ResetDirective::ZSTD_reset_session_only)
    .value("reset_parameters", ZSTD_ResetDirective::ZSTD_reset_parameters)
    .value("reset_session_and_parameters", ZSTD_ResetDirective::ZSTD_reset_session_and_parameters);

  // ZSTD_EndDirective
  enum_<ZSTD_EndDirective>("EndDirective")
    .value("e_continue", ZSTD_EndDirective::ZSTD_e_continue)
    .value("e_flush", ZSTD_EndDirective::ZSTD_e_flush)
    .value("e_end", ZSTD_EndDirective::ZSTD_e_end);

  // ZSTD_cParameter
  enum_<ZSTD_cParameter>("cParameter")
    .value("c_compressionLevel", ZSTD_cParameter::ZSTD_c_compressionLevel)
    .value("c_windowLog", ZSTD_cParameter::ZSTD_c_windowLog)
    .value("c_hashLog", ZSTD_cParameter::ZSTD_c_hashLog)
    .value("c_chainLog", ZSTD_cParameter::ZSTD_c_chainLog)
    .value("c_searchLog", ZSTD_cParameter::ZSTD_c_searchLog)
    .value("c_minMatch", ZSTD_cParameter::ZSTD_c_minMatch)
    .value("c_targetLength", ZSTD_cParameter::ZSTD_c_targetLength)
    .value("c_strategy", ZSTD_cParameter::ZSTD_c_strategy)
    .value("c_enableLongDistanceMatching", ZSTD_cParameter::ZSTD_c_enableLongDistanceMatching)
    .value("c_ldmHashLog", ZSTD_cParameter::ZSTD_c_ldmHashLog)
    .value("c_ldmMinMatch", ZSTD_cParameter::ZSTD_c_ldmMinMatch)
    .value("c_ldmBucketSizeLog", ZSTD_cParameter::ZSTD_c_ldmBucketSizeLog)
    .value("c_ldmHashRateLog", ZSTD_cParameter::ZSTD_c_ldmHashRateLog)
    .value("c_contentSizeFlag", ZSTD_cParameter::ZSTD_c_contentSizeFlag)
    .value("c_checksumFlag", ZSTD_cParameter::ZSTD_c_checksumFlag)
    .value("c_dictIDFlag", ZSTD_cParameter::ZSTD_c_dictIDFlag);

  // Structures
  class_<ZSTD_inBuffer>("inBuffer")
    .constructor<>()
    .property("src", &ZSTD_inBuffer_src_getter, &ZSTD_inBuffer_src_setter)
    .property("size", &ZSTD_inBuffer::size)
    .property("pos", &ZSTD_inBuffer::pos);
  class_<ZSTD_outBuffer>("outBuffer")
    .constructor<>()
    .property("dst", &ZSTD_outBuffer_dst_getter, &ZSTD_outBuffer_dst_setter)
    .property("size", &ZSTD_outBuffer::size)
    .property("pos", &ZSTD_outBuffer::pos);
}
