// a advanced post processing shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;
uniform int postProcessTarget;
uniform int width;
uniform int height;
uniform int pixelateStrength;

out vec4 FragColor;

vec4 Default(vec2 texCoord){
    return texture(colourTarget, texCoord);
}

vec4 BoxBlur(vec2 texCoord){
    vec2 texel = 1.0f / textureSize(colourTarget, 0);
    vec4 colour = texture(colourTarget, texCoord);
    colour += texture(colourTarget, texCoord + texel * vec2(-1, 1));
    colour += texture(colourTarget, texCoord + texel * vec2(-1, 0));
    colour += texture(colourTarget, texCoord + texel * vec2(-1, -1));
    
    colour += texture(colourTarget, texCoord + texel * vec2(0, 1));
    colour += texture(colourTarget, texCoord + texel * vec2(0, -1));
    
    colour += texture(colourTarget, texCoord + texel * vec2(1, 1));
    colour += texture(colourTarget, texCoord + texel * vec2(1, 0));
    colour += texture(colourTarget, texCoord + texel * vec2(1, -1));
    
    return colour / 9;
}

vec4 Distort(vec2 texCoord){
    vec2 mid = vec2(0.5f);
    float distanceFromCentre = distance(texCoord, mid);
    vec2 normalizedCoord = normalize(texCoord - mid);
    float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.05f;
    vec2 newCoord = mid + bias * normalizedCoord;
    return texture(colourTarget, newCoord);
}

vec4 SobelEdgeDetection(vec2 texCoord){
    float w = 1.0f / width;
    float h = 1.0f / height;

    vec4 n[9];

    n[0] = texture(colourTarget, texCoord + vec2(-w,-h));
    n[1] = texture(colourTarget, texCoord + vec2(0,-h));
    n[2] = texture(colourTarget, texCoord + vec2(w,-h));
    n[3] = texture(colourTarget, texCoord + vec2(-w,0));
    n[4] = texture(colourTarget, texCoord);
    n[5] = texture(colourTarget, texCoord + vec2(w,0));
    n[6] = texture(colourTarget, texCoord + vec2(-w,h));
    n[7] = texture(colourTarget, texCoord + vec2(0,h));
    n[8] = texture(colourTarget, texCoord + vec2(w,h));

    vec4 sobelEdgeH = n[2] + (2.0f*n[5]) + n[8] - (n[0] + (2.0f * n[3]) + n[6]);
    vec4 sobelEdgeV = n[0] + (2.0f*n[1]) + n[2] - (n[6] + (2.0f * n[7]) + n[8]);

    vec4 sobel = sqrt((sobelEdgeH * sobelEdgeH) + (sobelEdgeV * sobelEdgeV));

    return vec4(1 - sobel.rgb, 1.0);
}

vec4 Sepia(vec2 texCoord){
    vec4 baseColour = texture(colourTarget, texCoord);
    vec4 sepia = vec4(
        dot(baseColour.rgb, vec3(0.393f, 0.769f, 0.189f)),
        dot(baseColour.rgb, vec3(0.349f, 0.686f, 0.168f)),
        dot(baseColour.rgb, vec3(0.272f, 0.534f, 0.131f)),
        1        
    );
    return sepia;
}

vec4 GreyScale(vec2 texCoord){
    vec4 baseColour = texture(colourTarget, texCoord);
    float avg = (baseColour.r + baseColour.g + baseColour.b) / 3.0f;
    return vec4(avg, avg, avg, 1);
}

vec4 Invert(vec2 texCoord){
    vec4 baseColour = texture(colourTarget, texCoord);
    vec4 invert = vec4(
        1 - baseColour.r,
        1 - baseColour.g,
        1 - baseColour.b,
        1
    );
    return invert;
}

vec4 Pixelate(vec2 texCoord){
    vec2 grid_uv = round(texCoord * float(pixelateStrength)) / float(pixelateStrength);
    vec4 baseColour = texture(colourTarget, grid_uv);
    return baseColour;
}

void main()
{
    // First calculate the texels size
    vec2 texSize = textureSize(colourTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // Adjust the scale and coordinates
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

    // This will output, the desired post processing effect
    switch(postProcessTarget)    
    {
        case 0: // Default
        {
            FragColor = Default(texCoord);
            break;
        }
        case 1: // Box Blur
        {
            FragColor = BoxBlur(texCoord);
            break;
        }
        case 2: // Distort
        {
            FragColor = Distort(texCoord);
            break;
        }
        case 3: // Edge Detection
        {
            FragColor = SobelEdgeDetection(texCoord);
            break;

        }
        case 4: // Sepia
        {
            FragColor = Sepia(texCoord);
            break;
        }
        case 5: // Scanlines
        {
            FragColor = Default(texCoord);
            break;
        }
        case 6: // Grey Scale
        {
            FragColor = GreyScale(texCoord);
            break;
        }
        case 7: // Invert
        {
            FragColor = Invert(texCoord);
            break;
        }
        case 8: // Pixilizer
        {
            FragColor = Pixelate(texCoord);
            break;
        }
        case 9: // Posterization
        {
            FragColor = Default(texCoord);
            break;
        }
        case 10: // Distance Fog
        {
            FragColor = Default(texCoord);
            break;
        }
        case 11: // Depth of Field
        {
            FragColor = Default(texCoord);
            break;
        }
    }
}