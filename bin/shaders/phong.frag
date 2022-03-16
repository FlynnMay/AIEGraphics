// a simple phong shader
#version 410

in vec4 vPosition;
in vec3 vNormal;

uniform vec3 cameraPosition;

uniform vec3 AmbientColour;

uniform vec3 LightDirection;
uniform vec3 LightColour;

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float specularPower; // specular material power

out vec4 FragColor;

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

    // Calculate each colour property
    vec3 ambient = AmbientColour * Ka;
    vec3 diffuse = LightColour * lambertTerm;
    vec3 specular = LightColour * Ks * specularTerm;

    FragColor = vec4(ambient + diffuse + specular, 1);
}