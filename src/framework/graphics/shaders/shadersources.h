/*
 * Copyright (c) 2010-2017 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef PAINTEROGL2_SHADERSOURCES_H
#define PAINTEROGL2_SHADERSOURCES_H

static const std::string glslMainVertexShader = "\n\
    vec4 calculatePosition();\n\
    void main() {\n\
        gl_Position = calculatePosition();\n\
    }\n";

static const std::string glslMainWithTexCoordsVertexShader = "\n\
    attribute vec2 a_TexCoord;\n\
    uniform mat3 u_TextureMatrix;\n\
    varying vec2 v_TexCoord;\n\
    vec4 calculatePosition();\n\
    void main()\n\
    {\n\
        gl_Position = calculatePosition();\n\
        v_TexCoord = (u_TextureMatrix * vec3(a_TexCoord,1.0)).xy;\n\
    }\n";

static std::string glslPositionOnlyVertexShader = "\n\
    attribute vec2 a_Vertex;\n\
    uniform mat3 u_TransformMatrix;\n\
    uniform mat3 u_ProjectionMatrix;\n\
    uniform float u_Depth;\n\
    vec4 calculatePosition() {\n\
        return vec4((u_ProjectionMatrix * u_TransformMatrix * vec3(a_Vertex.xy, 1.0)).xy, u_Depth / 16384.0, 1.0);\n\
    }\n";

static const std::string glslMainFragmentShader = "\n\
    uniform float u_Depth;\n\
    vec4 calculatePixel();\n\
    void main()\n\
    {\n\
        gl_FragColor = calculatePixel();\n\
        if(gl_FragColor.a < 0.01 && u_Depth > 0.0)\n\
	        discard;\n\
    }\n";

static const std::string glslTextureSrcFragmentShader = "\n\
    varying vec2 v_TexCoord;\n\
    uniform vec4 u_Color;\n\
    uniform sampler2D u_Tex0;\n\
    vec4 calculatePixel() {\n\
        return texture2D(u_Tex0, v_TexCoord) * u_Color;\n\
    }\n";


static const std::string glslSolidColorFragmentShader = "\n\
    uniform vec4 u_Color;\n\
    vec4 calculatePixel() {\n\
        return u_Color;\n\
    }\n";

static const std::string glslSolidColorOnTextureFragmentShader = "\n\
    uniform vec4 u_Color;\n\
    varying vec2 v_TexCoord;\n\
    uniform sampler2D u_Tex0;\n\
    vec4 calculatePixel() {\n\
        if(texture2D(u_Tex0, v_TexCoord).a > 0.01)\n\
            return u_Color;\n\
        return vec4(0,0,0,0);\n\
    }\n";


static const std::string glslOutfitVertexShader = "\n\
    attribute vec2 a_TexCoord;\n\
    uniform mat3 u_TextureMatrix;\n\
    varying vec2 v_TexCoord;\n\
    varying vec2 v_TexCoord2;\n\
    attribute vec2 a_Vertex;\n\
    uniform mat3 u_TransformMatrix;\n\
    uniform mat3 u_ProjectionMatrix;\n\
    uniform float u_Depth;\n\
    uniform vec2 u_Offset;\n\
    void main()\n\
    {\n\
        gl_Position = vec4((u_ProjectionMatrix * u_TransformMatrix * vec3(a_Vertex.xy, 1.0)).xy, u_Depth / 16384.0, 1.0);\n\
        v_TexCoord = (u_TextureMatrix * vec3(a_TexCoord,1.0)).xy;\n\
        v_TexCoord2 = (u_TextureMatrix * vec3(a_TexCoord + u_Offset,1.0)).xy;\n\
    }\n";

static const std::string glslOutfitFragmentShader = "\n\
    uniform mat4 u_Color;\n\
    varying vec2 v_TexCoord;\n\
    varying vec2 v_TexCoord2;\n\
    uniform sampler2D u_Tex0;\n\
    void main()\n\
    {\n\
        gl_FragColor = texture2D(u_Tex0, v_TexCoord);\n\
        vec4 texcolor = texture2D(u_Tex0, v_TexCoord2);\n\
        if(texcolor.r > 0.9)\n\
            gl_FragColor *= texcolor.g > 0.9 ? u_Color[0] : u_Color[1];\n\
        else if(texcolor.g > 0.9)\n\
            gl_FragColor *= u_Color[2];\n\
        else if(texcolor.b > 0.9)\n\
            gl_FragColor *= u_Color[3];\n\
        if(gl_FragColor.a < 0.01) discard;\n\
    }\n";

#endif
