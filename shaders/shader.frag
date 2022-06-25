uniform sampler2D texture;

varying in vec4 gradientColor;

void main()
{
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    if (texture2D(texture, gl_TexCoord[0].st).b > 0) {
        gl_FragColor = gradientColor;
    }
}