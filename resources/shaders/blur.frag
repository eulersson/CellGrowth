#version 410 core

out float fColor;

in vec2 vTexCoords;

uniform sampler2D tInputSSAO;

int blurAmount = 2; // uniform int blurAmount;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(tInputSSAO, 0));
    float result = 0.0;

    if (blurAmount > 1) {
        for (int x = -blurAmount; x < blurAmount; ++x)
        {
            for (int y = -blurAmount; y < blurAmount; ++y) {
                vec2 offset = vec2(float(x), float(y)) * texelSize;
                result += texture(tInputSSAO, vTexCoords + offset).r;
            }
        }
        result /= float(blurAmount*blurAmount*blurAmount*blurAmount);
    } else {
        result += texture(tInputSSAO, vTexCoords).r;
    }

    fColor = result;
}
