/* clang-format off */

#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include <stdint.h>

/**
 * @enum  AttributeSemantic
 * @brief Enum specifying the semantic meaning of a vertex attribute.
 */
typedef enum
{
  ATTR_POSITION,  /**< Vertex position attribute */
  ATTR_COLOR,     /**< Vertex color attribute */
  ATTR_NORMAL,    /**< Vertex normal vector attribute */
  ATTR_TEXCOORD,  /**< Vertex texture coordinate attribute */
} AttributeSemantic;

/**
 * @struct  IndexBuffer
 * @brief   Stores an array of indices used for indexed drawing.
 */
typedef struct
{
  unsigned int* data;   /**< Pointer to dynamically allocated array of indices */
  size_t        count;  /**< Number of indices stored in the buffer */
} IndexBuffer;

/**
 * @struct  VertexAttribute
 * @brief   Describes a single vertex attribute within a vertex layout.
 */
typedef struct
{
  AttributeSemantic semantic;               /**< Semantic meaning of the attribute */
  uint32_t          offset;                 /**< Byte offset from start of vertex */
  uint32_t          type_size;              /**< Size in bytes of the attribute data type */
  uint32_t          component_count;        /**< Number of components */
} VertexAttribute;

/**
 * @struct  VertexLayout
 * @brief   Describes the memory layout of a single vertex, including all attributes.
 */
typedef struct
{
  VertexAttribute *attributes;      /**< Pointer to array of vertex attributes */
  uint32_t         attribute_count; /**< Number of attributes in the array */
  uint32_t         vertex_stride;   /**< Total size in bytes of one vertex */
} VertexLayout;

/**
 * @struct  VertexBuffer
 * @brief   Stores vertex data and its layout for rendering or processing.
 */
typedef struct
{
  void         *data;              /**< Pointer to raw vertex data */
  size_t        size;              /**< Size in bytes of allocated vertex data */
  uint32_t      vertex_count;      /**< Number of vertices stored */
  VertexLayout  layout;            /**< Layout describing the vertex format */
  IndexBuffer  *indices;
} VertexBuffer;

/**
 * @brief Create a vertex layout by copying an array of vertex attributes.
 *
 * @param attributes      Pointer to an array of VertexAttribute describing the layout.
 * @param attribute_count Number of attributes in the array.
 * @param vertex_stride   Total size in bytes of a single vertex.
 *
 * @return A VertexLayout struct with copied attributes and stride set.
 */
VertexLayout vertex_layout_create (const VertexAttribute *attributes,
                                   uint32_t attribute_count,
                                   uint32_t vertex_stride);

/**
 * @brief Destroy a vertex layout, freeing any allocated memory.
 *
 * @param layout Pointer to the VertexLayout to destroy.
 */
void vertex_layout_destroy (VertexLayout *layout);

/**
 * @brief Create a vertex buffer initialized with given data or zeroed memory.
 *
 * Allocates memory for vertex_count vertices with the specified layout.
 * If data is provided, copies the data into the buffer. If data is NULL,
 * the buffer is zero-initialized.
 *
 * @param data          Pointer to vertex data to copy, or NULL.
 * @param layout        Vertex layout describing the buffer’s vertex format.
 * @param vertex_count  Number of vertices in the buffer.
 *
 * @return A VertexBuffer struct owning the allocated data and layout.
 *         If allocation fails, returns a buffer with data == NULL and vertex_count == 0.
 */
VertexBuffer vertex_buffer_create (const void* data,
                                   VertexLayout layout,
                                   uint32_t vertex_count);

/**
 * @brief Destroy a vertex buffer, freeing owned data and layout.
 *
 * @param buffer Pointer to the VertexBuffer to destroy.
 */
void vertex_buffer_destroy (VertexBuffer *buffer);

/**
 * @brief Update the contents of a vertex buffer.
 *
 * Copies `size` bytes from `data` into the buffer. If `size` is larger
 * than the allocated buffer, the buffer is reallocated. If smaller,
 * leftover bytes are zeroed out.
 *
 * The vertex count is updated based on the new data size and layout stride.
 *
 * @param buffer  Pointer to the VertexBuffer to update.
 * @param data    Pointer to source data.
 * @param size    Number of bytes to copy.
 */
void vertex_buffer_update (VertexBuffer *buffer,
                           const void *data,
                           size_t size);

/**
 * @brief Get a pointer to a specific attribute of a specific vertex.
 *
 * Searches the buffer’s layout for the attribute semantic and returns
 * a pointer to that attribute’s data for the given vertex index.
 *
 * @param buffer        Pointer to the VertexBuffer.
 * @param vertex_index  Index of the vertex (0-based).
 * @param semantic      The attribute semantic to locate.
 *
 * @return Pointer to the attribute data within the vertex, or NULL if
 *         the attribute or vertex index is invalid.
 */
void *vertex_buffer_get_attribute_pointer (const VertexBuffer *buffer,
                                           uint32_t vertex_index,
                                           AttributeSemantic semantic);

/**
 * @brief Create an index buffer and initialize its data.
 *
 * Allocates memory for an index buffer to hold `count` indices.
 * If `data` is not NULL, copies the provided indices into the buffer.
 * If `data` is NULL, the allocated buffer is zero-initialized.
 *
 * @param data  Pointer to an array of indices to copy, or NULL.
 * @param count Number of indices to allocate and copy.
 *
 * @return An IndexBuffer struct owning the allocated data and count.
 *         If allocation fails, buffer.data will be NULL and count will be 0.
 */
IndexBuffer
index_buffer_create(const unsigned int* data,
                                size_t count);

/**
 * @brief Destroy an index buffer, freeing its allocated memory.
 *
 * Frees the memory held by the index buffer and resets its members.
 * Does nothing if the buffer pointer or buffer data is NULL.
 *
 * @param buffer Pointer to the IndexBuffer to destroy.
 */
void
index_buffer_destroy(IndexBuffer* buffer);

/**
 * @brief Update the contents of an existing index buffer.
 *
 * Copies `count` indices from `data` into the buffer.
 * If the new count is greater than the current buffer size,
 * the buffer is reallocated to accommodate the new size.
 * If smaller, the extra memory is zeroed out.
 * Does nothing if the buffer pointer, data pointer is NULL,
 * or count is zero.
 *
 * @param buffer Pointer to the IndexBuffer to update.
 * @param data   Pointer to the source index data.
 * @param count  Number of indices to copy.
 */
void
index_buffer_update(IndexBuffer* buffer,
                    const unsigned int* data,
                    size_t count);

#endif /* BUFFER_H */
