varying out vec4 gradientColor;

void main()
{
    vec4 lightBlue = vec4(0.0, 0.4, 0.6, 1.0);
    vec4 darkBlue = vec4(0.0, 0.1, 0.4, 1.0);

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    gl_FrontColor = gl_Color;



    float alpha = gl_Position.y + 1;

    gradientColor = mix(darkBlue, lightBlue, alpha);
}