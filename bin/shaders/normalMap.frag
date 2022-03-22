// a simple normal map colour shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

uniform vec3 cameraPosition;

uniform vec3 AmbientColour;

uniform vec3 LightDirection;
uniform vec3 LightColour;

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColors[MAX_LIGHTS];
uniform vec3 PointLightPositions[MAX_LIGHTS];

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float specularPower; // specular material power

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

out vec4 FragColor;

vec3 Diffuse(vec3 direction, vec3 colour, vec3 normal)
{
    return colour * max(0, dot(normal, -direction));
}

vec3 Specular(vec3 direction, vec3 colour, vec3 normal, vec3 view)
{
    vec3 R = reflect(direction, normal);
    float specularTerm = pow(max(0, dot(R, view)), specularPower);
    return specularTerm * colour;
}

void main()
{
    // Make sure the normal and light direction are normalised
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);

    mat3 TBN = mat3(T, B, N);

    vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
    vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
    vec3 texNormal = texture(normalTexture, vTexCoord).rgb;

    N = TBN * (texNormal * 2 - 1);

    // Then calculate th lambert term (negative light direction)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    // Calculate the diffuse lighting from the global source
    vec3 diffuseTotal = Diffuse(L, LightColour, N);

    // Calculate view vector and reflection vector
    vec3 V = normalize(cameraPosition - vPosition.xyz);
    vec3 R = reflect(L, N);

    // Calculate the specular lighting from the global source
    vec3 specularTotal =  Specular(L, LightColour, N, V);

    for(int i = 0; i < numLights; i++)
    {
        vec3 direction =  vPosition.xyz - PointLightPositions[i];
        float distance = length(direction);
        direction = direction/distance;

        vec3 colour = PointLightColors[i] / (distance * distance);
        diffuseTotal += Diffuse(direction, colour, N);
        specularTotal += Specular(direction, colour, N, V);
    }

    // Calculate specular term
    float specularTerm = pow(max(0, dot(R, V)), specularPower);

    // Calculate each colour property
    vec3 ambient = AmbientColour * Ka * texDiffuse;
    vec3 diffuse = Kd * texDiffuse * diffuseTotal;
    vec3 specular = Ks * texSpecular * specularTotal;

    FragColor = vec4(ambient + diffuse + specular, 1);
}