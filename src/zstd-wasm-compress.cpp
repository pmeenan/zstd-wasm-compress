#include <stdlib.h>    // malloc, free, exit
#include <stdio.h>     // fprintf, perror, fopen, etc.
#include <string.h>    // strlen, strcat, memset, strerror
#include <errno.h>     // errno
#include <sys/stat.h>  // stat
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

// Dictionary
size_t _ZSTD_compress_usingDict(unsigned int ctx, unsigned int dst, size_t dstCapacity, unsigned int src, size_t srcSize, unsigned int dict,size_t dictSize, int compressionLevel) {
  return ZSTD_compress_usingDict((ZSTD_CCtx*) ctx, (void*) dst, dstCapacity, (const void*) src, srcSize, (const void*) dict, dictSize, compressionLevel);
}

unsigned int _ZSTD_createCDict(unsigned int dictBuffer, size_t dictSize, int compressionLevel) {
  return (unsigned int)ZSTD_createCDict((const void*) dictBuffer, dictSize, compressionLevel)
}

size_t _ZSTD_freeCDict(unsigned int CDict) {
  return ZSTD_freeCDict((ZSTD_CDict*) CDict);
}

size_t _ZSTD_compress_usingCDict(unsigned int cctx, unsigned int dst, size_t dstCapacity, unsigned int src, size_t srcSize, unsigned int cdict) {
  return ZSTD_compress_usingCDict((ZSTD_CCtx*) cctx, (void*) dst, dstCapacity, (const void*) src, srcSize, (const ZSTD_CDict*) cdict);
}


EMSCRIPTEN_BINDINGS(zstd) {
  // Wrapped functions
	function("ZSTD_compress", &_ZSTD_compress);
	function("ZSTD_compress2", &_ZSTD_compress2);
	function("ZSTD_createCCtx", &_ZSTD_createCCtx);
	function("ZSTD_freeCCtx", &_ZSTD_freeCCtx);
	function("ZSTD_CCtx_reset", &_ZSTD_CCtx_reset);
	function("ZSTD_cParam_getBounds_lower", &ZSTD_cParam_getBounds_lower);
	function("ZSTD_cParam_getBounds_upper", &ZSTD_cParam_getBounds_upper);
	function("ZSTD_CCtx_setParameter", &_ZSTD_CCtx_setParameter);
	function("ZSTD_compressCCtx", &_ZSTD_compressCCtx);

	function("ZSTD_compress_usingDict", &_ZSTD_compress_usingDict);
	function("ZSTD_createCDict", &_ZSTD_createCDict);
	function("ZSTD_freeCDict", &_ZSTD_freeCDict);
	function("ZSTD_compress_usingCDict", &_ZSTD_compress_usingCDict);

	// Helper functions
	function("ZSTD_compressBound", &ZSTD_compressBound);
	function("ZSTD_isError", &ZSTD_isError);
  function("ZSTD_maxCLevel", &ZSTD_maxCLevel);
	function("ZSTD_versionNumber", &ZSTD_versionNumber);
	function("ZSTD_CStreamInSize", &ZSTD_CStreamInSize);
	function("ZSTD_CStreamOutSize", &ZSTD_CStreamOutSize);

  // ZSTD_ResetDirective
  constant("ZSTD_reset_session_only", ZSTD_reset_session_only);
  constant("ZSTD_reset_parameters", ZSTD_reset_parameters);
  constant("ZSTD_reset_session_and_parameters", ZSTD_reset_session_and_parameters);

  // ZSTD_EndDirective
  constant("ZSTD_e_continue", ZSTD_e_continue);
  constant("ZSTD_e_flush", ZSTD_e_flush);
  constant("ZSTD_e_end", ZSTD_e_end);

  // ZSTD_cParameter
  constant("ZSTD_c_compressionLevel", ZSTD_c_compressionLevel);
  constant("ZSTD_c_windowLog", ZSTD_c_windowLog);
  constant("ZSTD_c_hashLog", ZSTD_c_hashLog);
  constant("ZSTD_c_chainLog", ZSTD_c_chainLog);
  constant("ZSTD_c_searchLog", ZSTD_c_searchLog);
  constant("ZSTD_c_minMatch", ZSTD_c_minMatch);
  constant("ZSTD_c_targetLength", ZSTD_c_targetLength);
  constant("ZSTD_c_strategy", ZSTD_c_strategy);
  constant("ZSTD_c_enableLongDistanceMatching", ZSTD_c_enableLongDistanceMatching);
  constant("ZSTD_c_ldmHashLog", ZSTD_c_ldmHashLog);
  constant("ZSTD_c_ldmMinMatch", ZSTD_c_ldmMinMatch);
  constant("ZSTD_c_ldmBucketSizeLog", ZSTD_c_ldmBucketSizeLog);
  constant("ZSTD_c_ldmHashRateLog", ZSTD_c_ldmHashRateLog);
  constant("ZSTD_c_contentSizeFlag", ZSTD_c_contentSizeFlag);
  constant("ZSTD_c_checksumFlag", ZSTD_c_checksumFlag);
  constant("ZSTD_c_dictIDFlag", ZSTD_c_dictIDFlag);
}
