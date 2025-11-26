#include "buffer.h"
#include <stdlib.h>
#include <string.h>

VertexLayout
vertex_layout_create (const VertexAttribute *attributes,
                      uint32_t attribute_count,
                      uint32_t vertex_stride)
{
  VertexLayout layout     = { 0 };
  layout.attribute_count  = attribute_count;
  layout.vertex_stride    = vertex_stride;

  if (attribute_count > 0)
    {
      layout.attributes = malloc (sizeof (VertexAttribute) * attribute_count);
      if (layout.attributes)
        {
          memcpy (layout.attributes, attributes,
                  sizeof (VertexAttribute) * attribute_count);
        }
      else
        {
          layout.attribute_count = 0;
        }
    }
  else
    {
      layout.attributes = NULL;
    }

  return layout;
}

void
vertex_layout_destroy (VertexLayout *layout)
{
  if (layout && layout->attributes)
    {
      free (layout->attributes);
      layout->attributes      = NULL;
      layout->attribute_count = 0;
      layout->vertex_stride   = 0;
    }
}

VertexBuffer
vertex_buffer_create (const void *data,
                      VertexLayout layout,
                      uint32_t vertex_count)
{
  VertexBuffer buffer = { 0 };
  buffer.vertex_count = vertex_count;
  buffer.layout       = layout;
  buffer.size         = (size_t)vertex_count * layout.vertex_stride;
  buffer.data         = malloc (buffer.size);

  if (!buffer.data)
    {
      buffer.size = 0;
      buffer.vertex_count = 0;
      return buffer;
    }

  if (data)
    {
      memcpy (buffer.data, data, buffer.size);
    }
  else
    {
     memset (buffer.data, 0, buffer.size);
    }

  return buffer;
}

void
vertex_buffer_destroy (VertexBuffer *buffer)
{
  if (buffer)
    {
      free (buffer->data);
      buffer->data          = NULL;
      buffer->size          = 0;
      buffer->vertex_count  = 0;
      vertex_layout_destroy (&buffer->layout);
    }
}

void
vertex_buffer_update (VertexBuffer *buffer,
                      const void *data,
                      size_t size)
{
  if (size == 0 || !buffer || !data)
    return;

  if (size > buffer->size)
    {
      void *new_data  = realloc (buffer->data, size);
      if (!new_data)
        return;
      buffer->data    = new_data;
      buffer->size    = size;
    }
  else if (size < buffer->size)
    {
      memset ((char *)buffer->data + size, 0, buffer->size - size);
    }

  memcpy (buffer->data, data, size);

  if (buffer->layout.vertex_stride > 0)
    {
      buffer->vertex_count = (uint32_t)(size / buffer->layout.vertex_stride);
    }
}

void *
get_attribute_pointer (const VertexBuffer *buffer,
                                     uint32_t vertex_index,
                                     AttributeSemantic semantic)
{
  if (!buffer || !buffer->data || vertex_index >= buffer->vertex_count)
    {
      return NULL;
    }

  for (uint32_t i = 0; i < buffer->layout.attribute_count; ++i)
    {
      VertexAttribute *attr = &buffer->layout.attributes[i];
      if (attr->semantic == semantic)
        {
          uint8_t *base = (uint8_t *)buffer->data;
          return base + (vertex_index * buffer->layout.vertex_stride)
                 + attr->offset;
        }
    }
  return NULL;
}

IndexBuffer
index_buffer_create (const unsigned int *data,
                     size_t count)
{
  IndexBuffer buffer;
  buffer.count          = count;
  size_t size_bytes     = count * sizeof (unsigned int);
  buffer.data           = (unsigned int *)malloc (size_bytes);

  if (!buffer.data)
    {
      buffer.count = 0;
      return buffer;
    }

  if (data)
    {
      memcpy (buffer.data, data, size_bytes);
    }
  else
    {
      memset (buffer.data, 0, size_bytes);
    }

  return buffer;
}

void
index_buffer_destroy (IndexBuffer *buffer)
{
  if (!buffer || !buffer->data)
    {
      return;
    }

  free (buffer->data);
  buffer->data = NULL;
  buffer->count = 0;
}

void
index_buffer_update (IndexBuffer *buffer,
                     const unsigned int *data,
                     size_t count)
{
  if (!buffer || !data || count == 0)
    {
      return;
    }

  size_t size_bytes     = count         * sizeof (unsigned int);
  size_t old_size_bytes = buffer->count * sizeof (unsigned int);

  if (count > buffer->count)
    {
      unsigned int *new_data = realloc (buffer->data, size_bytes);
      if (!new_data)
        {
          return;
        }

      buffer->data  = new_data;
      buffer->count = count;
    }
  else if (count < buffer->count)
    {
      memset ((char *)buffer->data + size_bytes, 0, old_size_bytes - size_bytes);
      buffer->count = count;
    }

  memcpy (buffer->data, data, size_bytes);
}
