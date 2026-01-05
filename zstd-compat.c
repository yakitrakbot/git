/*
 * zstd wrappers for decompressing .json.zst files during pack-objects
 */
#include "git-compat-util.h"
#include "zstd-compat.h"

#ifdef HAVE_ZSTD

#include <zstd.h>

/* Zstd magic number: 0x28 0xB5 0x2F 0xFD */
#define ZSTD_MAGIC_NUMBER 0xFD2FB528

int is_zstd_content(const void *data, size_t size)
{
	const unsigned char *buf = data;

	if (size < 4)
		return 0;

	/* Check for zstd magic number (little-endian) */
	return buf[0] == 0x28 &&
	       buf[1] == 0xB5 &&
	       buf[2] == 0x2F &&
	       buf[3] == 0xFD;
}

void *zstd_decompress(const void *src, size_t src_size, size_t *dst_size)
{
	unsigned long long decompressed_size;
	void *dst;
	size_t result;

	if (!is_zstd_content(src, src_size))
		return NULL;

	/* Get the decompressed size from the frame header */
	decompressed_size = ZSTD_getFrameContentSize(src, src_size);
	if (decompressed_size == ZSTD_CONTENTSIZE_UNKNOWN ||
	    decompressed_size == ZSTD_CONTENTSIZE_ERROR)
		return NULL;

	/* Sanity check: don't allocate more than 1GB */
	if (decompressed_size > (1ULL << 30))
		return NULL;

	dst = xmalloc(decompressed_size);

	result = ZSTD_decompress(dst, decompressed_size, src, src_size);
	if (ZSTD_isError(result)) {
		free(dst);
		return NULL;
	}

	*dst_size = result;
	return dst;
}

#endif /* HAVE_ZSTD */
