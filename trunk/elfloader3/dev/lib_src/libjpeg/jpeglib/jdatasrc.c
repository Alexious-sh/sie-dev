/*
 * jdatasrc.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains decompression data source routines for the case of
 * reading JPEG data from a file (or any stdio stream).  While these routines
 * are sufficient for most applications, some will want to use a different
 * source manager.
 * IMPORTANT: we assume that fread() will correctly transcribe an array of
 * JOCTETs from 8-bit-wide elements on external storage.  If char is wider
 * than 8 bits on your machine, you may need to do some tweaking.
 */

/* this is not a core library module, so it doesn't define JPEG_INTERNALS */
#include "jinclude.h"
#include "jpeglib.h"
#include "jerror.h"


/* Expanded data source object for stdio input */

typedef struct {
  struct jpeg_source_mgr pub;	/* public fields */
  char *buffer;		/* start of buffer */
} my_source_mgr;

typedef my_source_mgr * my_src_ptr;

#define INPUT_BUF_SIZE  16384	/* choose an efficiently fread'able size */


/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */

METHODDEF(void)
init_source (j_decompress_ptr cinfo)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;
  src->pub.bytes_in_buffer = INPUT_BUF_SIZE;
  src->pub.next_input_byte = (JOCTET const *)src->buffer;

  /* We reset the empty-input-file flag for each image,
   * but we don't clear the input buffer.
   * This is correct behavior for reading a series of images from one source.
   */
}
METHODDEF(boolean)
fill_input_buffer (j_decompress_ptr cinfo)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;
  src->buffer+=INPUT_BUF_SIZE;
  src->pub.next_input_byte = (JOCTET const *)src->buffer;
  src->pub.bytes_in_buffer = INPUT_BUF_SIZE;
  return TRUE;
}



/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * Writers of suspendable-input applications must note that skip_input_data
 * is not granted the right to give a suspension return.  If the skip extends
 * beyond the data currently in the buffer, the buffer can be marked empty so
 * that the next read will cause a fill_input_buffer call that can suspend.
 * Arranging for additional bytes to be discarded before reloading the input
 * buffer is the application writer's problem.
 */

METHODDEF(void)
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;

  /* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
  src->pub.next_input_byte += (size_t) num_bytes;
  src->pub.bytes_in_buffer -= (size_t) num_bytes;
}

METHODDEF(void)
term_source (j_decompress_ptr cinfo)
{
  /* no work necessary here */
}

GLOBAL(void)
jpeg_stdio_src(j_decompress_ptr cinfo, void * buf)
{
  my_src_ptr src;

  /* The source object and input buffer are made permanent so that a series
   * of JPEG images can be read from the same file by calling jpeg_stdio_src
   * only before the first one.  (If we discarded the buffer at the end of
   * one image, we'd likely lose the start of the next one.)
   * This makes it unsafe to use this manager and a different source
   * manager serially with the same JPEG object.  Caveat programmer.
   */
  if (cinfo->src == NULL) {	/* first time for this JPEG object? */
    cinfo->src = (struct jpeg_source_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  sizeof(my_source_mgr));
  }
  src = (my_src_ptr) cinfo->src;
  src->pub.init_source = init_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = term_source;
  src->buffer = buf;
  src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */
}

