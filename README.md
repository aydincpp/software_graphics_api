# Software Renderer

![demo](./demo.png)

This project is a software renderer written from scratch in C99 for Linux. It has no dependencies and only works with the CPU.
I started this project because I wanted to learn how the graphics API processes pixels.
After reading a few blog posts about the graphics pipeline, I still couldn't understand "EXACTLY" what was going on, so I decided to implement a small graphics API from scratch.
I will document everything I learn, you can read it if you want.

## What's a Frame?
A frame is a complete image, composed of a grid of pixels that the display shows at one moment in time.

## What's a Framebuffer?
A framebuffer refers to a portion of either system RAM (in a software or simple framebuffer) or GPU VRAM (in a hardware accelerated framebuffer). It keeps the color information for every pixel in a frame.

Each pixel's color is made up of several channels: red, green, blue, and sometimes alpha for transparency. Each channel usually uses 8 bits (1 byte), which means that a 24-bit color depth includes 8 bits for each channel (red, green, and blue).

What about the alpha channel? The alpha channel is useless in the majority of framebuffers, particularly `/dev/fb0` on Linux. The RGB bits are simply sent to the screen by the display controller (described later), which does not blend or interpret transparency. This is so because alpha blending is not a feature of the standard framebuffer hardware, but rather a compositing feature of software or a GPU. What would happen if the alpha value was set to 0 (`0x00`)? In essence, the screen will say, "F*ck you! Your dumb alpha doesn't matter to me!"😂.

> **Note:** The byte order can vary depending on the framebuffer configuration (for example, some systems use **BGRA** instead of **RGBA**).

The table below shows how a 32-bit pixel layout in memory looks like.
pixel's color value is `0x00FF8040` (ARGB32)
| Byte Offset  | Channel  | Description            | value                |
|--------------|----------|------------------------|----------------------|
| +0           | Blue     | Blue intensity         | `0x40`               |
| +1           | Green    | Green intensity        | `0x80`               |
| +2           | Red      | Red intensity          | `0xFF`               |
| +3           | Alpha    | Transparency / unused  | `0x00`               |

Yes, this is how each pixel is stored in a framebuffer folks. Cool right? So I know exactly what you are thinking right now (maybe). If the order is ARGB, then why is the first byte for blue, the second for green, the third for red, and the last byte for alpha? Shouldn't the bytes be stored in ARGB order in memory as well? Why does it appear as BGRA? Did I mistype it? No!

Most CPUs today (x86, x86_64, ARM in most cases) use little-endian byte order. So now I have to explain little-endian. Sorry, not little-indian!

### What's little-endian?
Little-endian means the **least significant** byte comes first in memory. When storing multi-byte values (like our 32-bit pixel), the bytes are flipped compared to the human-readable hex notation.

So the value 0x00FF8040 is stored in memory as:
```text
0x40 0x80 0xFF 0x00
```

This is why your ARGB apears reversed in memory - it’s just how the CPU organizes bytes for efficiency. There is also big-endian, which is the opposite of little-endian (the most significant byte comes first).

You must be aware of the precise mapping of your color bytes in memory when writing pixels directly to the framebuffer, or else you risk displaying blue instead of red. You must obtain this information from the **framebuffer driver** because, as I indicated, it is system-dependent.

## What's a Framebuffer Driver?
So, you have this special block of memory called a framebuffer where your pixel's color info lives, right? But how does your computer know where that memory is and how to get the pixels onto your screen? The framebuffer driver is used in this situation.

A framebuffer driver is a special type of software in an operating system that communicates directly with the graphics hardware inside a computer. Its responsibility is to manage the framebuffer memory and tell the OS where the framebuffer is located in the memory. Different computers and graphics cards use different hardware with different capabilities. The framebuffer driver hides these details, allowing your program to simply write pixels to the framebuffer (`/dev/fb0` on Linux, for example) without worrying about the specifics. The driver configures screen resolution, color depth, refresh rates, and other important parameters by communicating with the hardware. The driver creates device files, such as `/dev/fb0`, which allow your programs to open, map, and write to the framebuffer memory.

### Why do you need the driver if you’re writing directly to memory?
Because you don’t just magically get a block of memory for your pixels. The driver sets everything up for you and makes sure your writes go to the right place, in the right format, and that the **display hardware** knows what to do with it. Without the driver, your program wouldn’t know how to talk to the hardware, and your pixels would be nowhere near the screen.

# What's a display controller?
The display controller (a.k.a. CRTC - Cathode Ray Tube Controller, an old term that stuck around) is the piece of hardware that takes care of actually sending the data from the framebuffer memory to your screen.

### What does the display controller do?
The display controller’s job is to read the pixel data from the framebuffer memory over and over, many times per second, to refresh the image on your screen. It takes those RGB values inside the framebuffer and turns them into electrical signals (or digital ones like HDMI, DisplayPort, LVDS) that the monitor understands. It also generates the synchronization signals (**horizontal sync** and **vertical sync**) to tell the monitor when each new line or frame begins.

### Can't the CPU just push pixel data to to the screen?
No. The display controller pulls the data out of the framebuffer automatically.
When you update pixels in memory, the display controller picks them up on its next refresh cycle and shows them. That’s why writing to the framebuffer instantly updates what’s displayed. The controller is doing all the real-time work behind the scenes.
