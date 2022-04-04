// a simple phong shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec4 objPosition;

uniform sampler2D SeamlessTexture;
uniform sampler2D HatchingTexture;
uniform sampler2D RampTexture;

uniform vec3 cameraPosition;

uniform vec3 AmbientColour;

uniform vec3 LightDirection;
uniform vec3 LightColour;

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float specularPower; // specular material power

out vec4 FragColor;

// float Rim(in vec3 N, in vec3)

void main()
{
    // Make sure the normal and light direction are normalised
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);
    
    // Then calculate th lambert term (negative light direction)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    // Calculate view vector and reflection vector
    vec3 V = normalize(cameraPosition - vPosition.xyz);
    vec3 R = reflect(L, N);

    // Calculate specular term
    float specularTerm = pow(max(0, dot(R, V)), specularPower);

    //vec4 sampleXY = texture(SeamlessTexture, objPosition.xy * 0.1f);
    //vec4 sampleYZ = texture(SeamlessTexture, objPosition.yz * 0.1f);
    //vec4 sampleXZ = texture(SeamlessTexture, objPosition.xz * 0.1f);
    //vec4 sampleTP = N.x * N.x * sampleYZ + N.y * N.y * sampleXZ + N.z * N.z * sampleXY;
    //sampleTP = vec4(ivec4(sampleTP * 4 + 0.5f)) / 4;

    //float intensity = AmbientColour.r + lambertTerm + specularTerm;

    //vec4 hatching = texture(HatchingTexture, gl_FragCoord.xy / 5);
    //intensity = step(hatching.x, intensity);

    // Calculate each colour property
    //vec3 ambient = AmbientColour * Ka;
    //vec3 diffuse = (AmbientColour + step(vec3(0.5f, 0.5f, 0.5f), LightColour * lambertTerm)) * sampleTP.xyz;
    //LightColour * lambertTerm;
    //vec3 specular = LightColour * Ks * step(0.1f, specularTerm);

    //float rim = 1 - max(0, dot(N, V));
    //rim = rim * rim * rim;
    //float alpha = rim + specular.r;
    
    //if(alpha < 0.1f)
      //discard;

    //FragColor = vec4(ambient + diffuse + specular + rim, alpha);
    //FragColor = vec4(diffuse + specular, 1);
    //FragColor = vec4(intensity * vec3(1,1,1) * sampleTP.xyz, 1);


    FragColor = texture(RampTexture, vec2(lambertTerm, 0.03f));
}