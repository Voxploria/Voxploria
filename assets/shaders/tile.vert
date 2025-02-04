#version 450
layout(location = 0) in vec2 inPos;

layout(push_constant) uniform PushConstants {
    vec2 offset;
    int tileType;
    int pad; // for 16-byte alignment
} pc;

layout(location = 0) out vec2 fragUV;

void main() {
    // Position: shift the unit quad (with inPos in [-0.5,0.5]) by the world offset.
    vec2 pos = inPos + pc.offset;
    // Convert world position to normalized device coordinates (assumes a 1024x768 window).
    vec2 ndc = pos / vec2(512.0, 384.0) - 1.0;
    gl_Position = vec4(ndc, 0.0, 1.0);

    // Compute UV coordinates.
    // inPos is in [-0.5,0.5]; add 0.5 to map to [0,1].
    vec2 uv = inPos + 0.5;
    // Assume texture atlas with 4 columns. Adjust uv.x based on tileType.
    float atlasColumns = 4.0;
    // (tileType of 1 uses column 0, tileType of 2 uses column 1, etc.)
    uv.x = (uv.x + float(pc.tileType - 1)) / atlasColumns;
    fragUV = uv;
}
