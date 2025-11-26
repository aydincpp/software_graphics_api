#ifndef FB_H
#define FB_H

#include <linux/fb.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

/**
 * @struct Framebuffer
 * @brief Represents a Linux framebuffer device.
 *
 */
typedef struct
{
  int                        fd;            /**< File descriptor for framebuffer device */
  struct fb_fix_screeninfo   finfo;         /**< Fixed screen information */
  struct fb_var_screeninfo   vinfo;         /**< Variable screen information */
  size_t                     size;          /**< Byte size of render area */
  float                      aspect;        /**< Aspect ratio of the screen */
  uint8_t                   *fbp;           /**< Pointer to mapped framebuffer memory */
  uint8_t                   *back_buffer;   /**< Pointer to backbuffer */
} Framebuffer;

/**
 * @brief Initializes the framebuffer.
 *
 * @param fb    Pointer to a Framebuffer structure.
 * @param path  Path to the framebuffer device (e.g., "/dev/fb0").
 * @return true if initialization is successful, false otherwise.
 */
bool
fb_init (Framebuffer* fb,
         const char* path);

/**
 * @brief Clears the framebuffer by resetting it to black.
 *
 * @param fb Pointer to a Framebuffer structure.
 */
void
fb_clear (Framebuffer* fb);

/**
 * @brief Presents the back buffer to the framebuffer device.
 *
 * @param fb Pointer to a Framebuffer structure.
 */
void
fb_present (Framebuffer* fb);

/**
 * @brief Opens the framebuffer device at the specified path.
 *
 * @param fb   Pointer to Framebuffer struct to initialize.
 * @param path Filesystem path to framebuffer device.
 * @return true if successfully opened, false otherwise.
 */
bool
fb_open (Framebuffer *fb,
         const char *path);

/**
 * @brief Closes the framebuffer device and releases resources.
 *
 * @param fb Pointer to Framebuffer struct to close.
 */
void
fb_close (Framebuffer *fb);


/**
 * @brief Retrieves and stores the fixed and variable screen info of the framebuffer.
 *
 * @param fb Pointer to Framebuffer struct.
 * @return true if info was successfully retrieved, false otherwise.
 */
bool
fb_get_info (Framebuffer *fb);

/**
 * @brief Applies the variable screen info settings to the framebuffer device.
 *
 * @param fb Pointer to Framebuffer struct.
 * @return true if info was successfully applied, false otherwise.
 */
bool
fb_put_info (Framebuffer *fb);

/**
 * @brief Maps the framebuffer device memory into the process address space.
 *
 * @param fb Pointer to Framebuffer struct.
 * @return true if mapping succeeded, false otherwise.
 */
bool
fb_map (Framebuffer *fb);

/**
 * @brief Clears the framebuffer by filling it with the specified RGB color.
 *
 * Alpha channel is set to fully opaque.
 *
 * @param fb Pointer to Framebuffer struct.
 * @param r  Red    channel (0-255).
 * @param g  Green  channel (0-255).
 * @param b  Blue   channel (0-255).
 */
bool
fb_unmap (Framebuffer *fb);

void
fb_clear_color (Framebuffer *fb,
                uint8_t r,
                uint8_t g,
                uint8_t b);

#endif /* FB_H */
