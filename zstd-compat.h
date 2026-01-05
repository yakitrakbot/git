#ifndef ZSTD_COMPAT_H
#define ZSTD_COMPAT_H

#ifdef HAVE_ZSTD

/*
 * Decompress zstd-compressed data.
 * Returns allocated buffer with decompressed data, or NULL on error.
 * Sets *dst_size to the size of the decompressed data.
 * Caller is responsible for freeing the returned buffer.
 */
void *zstd_decompress(const void *src, size_t src_size, size_t *dst_size);

/*
 * Check if data appears to be zstd-compressed by examining the magic number.
 * Returns 1 if data starts with zstd magic bytes (0x28 0xB5 0x2F 0xFD),
 * 0 otherwise.
 */
int is_zstd_content(const void *data, size_t size);

#endif /* HAVE_ZSTD */

#endif /* ZSTD_COMPAT_H */
