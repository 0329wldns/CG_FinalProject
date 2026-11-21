#version 330 core
in vec3 VertColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 color = vec3(1.0, 1.0, 1.0);
uniform float transparency = 1.0;
uniform bool useTexture = false;
uniform sampler2D adTexture;

void main()
{
    vec4 surfaceColor = vec4(VertColor * color, transparency);
    if (useTexture)
        surfaceColor *= texture(adTexture, TexCoord);
    FragColor = surfaceColor;
}
