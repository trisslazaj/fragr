// plasma.frag — a classic demoscene plasma: colored bands from summed sines.
// Shadertoy-style: define mainImage; fragr wraps it and supplies the uniforms.

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    // Pixel coordinate rescaled to 0..1 on both axes.
    vec2 uv = fragCoord / iResolution.xy;

    // Sum of sines at ~10 cycles across the image: two axis-aligned waves
    // plus a diagonal. Their interference is the plasma pattern.
    float v = sin(uv.x * 10.0) + sin(uv.y * 10.0) + sin((uv.x + uv.y) * 10.0);

    // Map the scalar to a color: cos() swings -1..1, remapped to 0..1, with
    // R/G/B phase-shifted by (0,2,4) so a single value becomes a rainbow.
    vec3 col = 0.5 + 0.5 * cos(v + vec3(0.0, 2.0, 4.0));

    fragColor = vec4(col, 1.0);
}
