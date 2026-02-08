#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blazValue;
uniform vec3 waterColor; 
uniform bool useWaterColor; 

uniform vec3 ambientLightColor; 
uniform vec3 lightPos;
uniform vec3 lightColor; 

uniform bool useLighting; 

uniform vec3 materialSpecular;
uniform float shininess; 
uniform vec3 lightSpecular;
uniform vec3 viewPosition;

uniform vec4 modelColor; // Changed to vec4 to include transparency
uniform bool hasTexture;
uniform bool notMap;

void main()
{
    if(useLighting){
        if (useWaterColor) {
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            //ambient / diffuse
            vec3 ambient = ambientLightColor * waterColor;
            vec3 diffuse = diff * lightColor * waterColor;
            //specular
            vec3 viewDir = normalize(viewPosition - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm); 
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
            vec3 specular = lightSpecular * spec * materialSpecular;
            //combine
            vec3 result = ambient + diffuse + specular;
            FragColor = vec4(result, 1.0);
        } 
        else if (notMap) {
            if (hasTexture) {

            } else {
                vec3 norm = normalize(Normal);
                vec3 lightDir = normalize(lightPos - FragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                //ambient / diffuse
                vec3 ambient = ambientLightColor * modelColor.rgb;
                vec3 diffuse = diff * lightColor * modelColor.rgb;
                //specular
                vec3 viewDir = normalize(viewPosition - FragPos);
                vec3 reflectDir = reflect(-lightDir, norm); 
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
                vec3 specular = lightSpecular * spec * materialSpecular;
                //combine
                vec3 result = ambient + diffuse + specular;
                FragColor = vec4(result, modelColor.a); 
            }
        }
        else
        {
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            //textures
            vec3 texColor1 = texture(texture1, TexCoord).rgb;
            vec3 texColor2 = texture(texture2, TexCoord).rgb;
            vec3 objectColor = mix(texColor1, texColor2, blazValue);
            //ambient / diffuse
            vec3 ambient = ambientLightColor * objectColor;
            vec3 diffuse = diff * lightColor * objectColor;
            //specular
            vec3 viewDir = normalize(viewPosition - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm); 
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
            vec3 specular = lightSpecular * spec * materialSpecular;
            //combine
            vec3 result = ambient + diffuse + specular;
            FragColor = vec4(result * 0.85, modelColor.a); 
        }
    }
    else {
        if (useWaterColor) {
            FragColor = vec4(waterColor, 1.0);
        } 
        else if (notMap) {
            if (hasTexture) {
                vec3 texColor1 = texture(texture1, TexCoord).rgb;
                vec3 texColor2 = texture(texture2, TexCoord).rgb;
                vec3 objectColor = mix(texColor1, texColor2, blazValue);
                FragColor = vec4(objectColor, modelColor.a); 
            } else {
                FragColor = vec4(modelColor.rgb, modelColor.a); 
            }
        }
        else
        {
            vec3 texColor1 = texture(texture1, TexCoord).rgb;
            vec3 texColor2 = texture(texture2, TexCoord).rgb;
            vec3 objectColor = mix(texColor1, texColor2, blazValue);
            FragColor = vec4(objectColor, modelColor.a); 
        }
    }
}