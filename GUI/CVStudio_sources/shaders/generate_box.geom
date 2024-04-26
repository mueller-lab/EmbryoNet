#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 5) out;

uniform vec2 pointPosition; // Uniform variable for the point position in pixels
uniform vec2 size_pixels;   // Size of the quad in pixels
uniform vec2 resolution;    // Screen resolution in pixels

void main() {
    vec2 pos  = 2 * (pointPosition / resolution) - vec2(1.0);
    pos = pos * vec2(1.0, -1.0); // Flip y-coordinate to match arrows' space
    vec2 size = 2 * size_pixels / resolution;
    
    gl_Position = vec4(pos.x - size.x / 2, pos.y - size.y / 2, 0.0, 1.0);
    EmitVertex();
    gl_Position = vec4(pos.x + size.x / 2, pos.y - size.y / 2, 0.0, 1.0);
    EmitVertex();
    gl_Position = vec4(pos.x + size.x / 2, pos.y + size.y / 2, 0.0, 1.0);
    EmitVertex();
    gl_Position = vec4(pos.x - size.x / 2, pos.y + size.y / 2, 0.0, 1.0);
    EmitVertex();
    gl_Position = vec4(pos.x - size.x / 2, pos.y - size.y / 2, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
