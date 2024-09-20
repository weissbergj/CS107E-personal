#ifndef FB_H
#define FB_H

/*
 * Low-level framebuffer routines. These provide a hardware
 * abstraction for displaying to screen that a graphics library can
 * then use to support drawing primitives.
 *
 * Students implement this module for assignment 6.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

typedef enum { FB_SINGLEBUFFER = 0, FB_DOUBLEBUFFER = 1 } fb_mode_t;

/*
 * `fb_init` : Required initialization for framebuffer
 *
 * Initialize the framebuffer.
 *
 * @param width  the requested width in pixels of the framebuffer
 * @param height the requested height in pixels of the framebuffer
 * @param mode   whether the framebuffer should be
 *                      single buffered (FB_SINGLEBUFFER)
 *                      or double buffered (FB_DOUBLEBUFFER)
 *
 * The depth of the framebuffer is always 4 bytes; ech pixel is
 * 32-bit value in format BGRA.
 *
 * If the requested size can be successfully accommodated by the underlying
 * hardware, the function returns normally. Otherwise an assert will be raised
 * within de/hdmi to report the problem.
 *
 * A subsequent call to fb_init after the first does a reinitialization.
 * All previous framebuffer memory is deallocated and the framebuffer is
 * re-initialized for the requested configuration.
 */
void fb_init(int width, int height, fb_mode_t mode);

/*
 * `fb_get_width`
 *
 * Get the current width in pixels of the framebuffer.
 *
 * @return    the width in pixels
 */
int fb_get_width(void);

/*
 * `fb_get_height`
 *
 * Get the current height in pixels of the framebuffer.
 *
 * @return    the height in pixels
 */
int fb_get_height(void);

/*
 * `fb_get_depth`
 *
 * Get the current depth in bytes of a single pixel.
 * In our case, depth is always 4.
 *
 * @return    the depth in bytes
 */
int fb_get_depth(void);

/*
 * `fb_get_draw_buffer`
 *
 * Get the start address of the framebuffer memory into which the
 * client can draw pixels. The address returned is the start of an
 * array of bytes of capacity width*height*depth.
 *
 * If in single buffering mode, there is only one buffer in use, so the
 * returned address does not change. That buffer is on-screen at all
 * times. The client draws to that one buffer, and all updates are
 * immediately displayed.
 *
 * In double buffering mode, there are two buffers: the one
 * currently on-screen and a second buffer that is off-screen. The
 * address returned by `fb_get_draw_buffer` corresponds to the buffer
 * that is currently off-screen. The off-screen buffer is sometimes
 * called the draw buffer. The client will do all drawing to the off-screen
 * buffer and when ready, calls `fb_swap_buffer` to exchange the
 * on-screen and off-screen buffers. The swap brings the updated
 * drawing on-screen in one smooth update.
 *
 * Note the address is returned as `void*`. Client should store into
 * a properly typed pointer so as to access the pixel data according
 * to their desired scheme (1-d, 2-d, etc.)
 *
 * @return    the address of the current draw (off-screen) buffer
 */
void* fb_get_draw_buffer(void);

/*
 * `fb_swap_buffer`
 *
 * Fast exchange of front (on-screen) and back (off-screen) buffers.
 * The buffer that was previously off-screen is now displayed on-screen
 * and buffer that was on-screen moves off-screen.
 *
 * If not in double buffering mode, there is only one buffer and this
 * function has no effect.
 */
void fb_swap_buffer(void);

#endif
