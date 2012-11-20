// LunarGOO(r320) output
#version 130
uniform sampler2D inputImageTexture3;
uniform float frameTime;
uniform sampler2D inputImageTexture;
uniform sampler2D inputImageTexture4;
uniform sampler2D inputImageTexture2;
in vec2 textureCoordinate;
const vec2 const29 = vec2(0.5);
const float const31 = 0.9;
const float const34 = 2;
const float const37 = 0.0075;
const float const39 = 1;
const float const48 = 10;
const vec2 const52 = vec2(12.9898, 78.233);
const float const55 = 43758.5;
const float const60 = 0.0625;
const float const62 = 0.375;
const bool const70 = false;
const vec4 const71 = vec4(0);
const float const74 = 100;
const float const83 = -0;
const float const92 = 16;
const float const95 = 4;
const float const111 = 20;
const float const121 = 0.1;
const vec3 const127 = vec3(0.2125, 0.7154, 0.0721);
const float const130 = 1500;
const float const132 = 1000;
const float const136 = 3.14;
const float const140 = 0.7;

void main()
{
    bool and_tmp_0;
    vec4 direct_t_0;
    bool and_tmp88_0;
    bool and_tmp95_0;
    bool and_tmp101_0;
    vec4 direct_t45_0;
    float darknessFactor_0;
    vec2 temp28 = textureCoordinate - const29;
    vec2 temp30 = vec2(const31);
    vec2 centerTexCoord2 = temp28 * temp30;
    float temp33 = frameTime / const34;
    float sin_retval3 = sin(temp33);
    float temp36 = sin_retval3 * const37;
    float temp38 = const39 - temp36;
    vec2 temp40 = vec2(temp38);
    vec2 temp41 = centerTexCoord2 * temp40;
    vec2 aTextCoord4 = temp41 + const29;
    float temp43 = textureCoordinate.y + sin_retval3;
    float mod_retval8 = mod(temp43, const39);
    vec2 temp45 = textureCoordinate;
    temp45.y = mod_retval8.x;
    vec4 texture2D_retval11 = texture2D(inputImageTexture, textureCoordinate);
    float rtime13 = frameTime / const48;
    float temp49 = rtime13 / const34;
    vec2 temp50;
    temp50.xy = vec2(temp49, temp49);
    float dot_retval16 = dot(temp50, const52);
    float sin_retval1718 = sin(dot_retval16);
    float temp54 = sin_retval1718 * const55;
    float fract_retval19 = fract(temp54);
    float mod_retval2021 = mod(fract_retval19, const39);
    float temp58 = textureCoordinate.x - mod_retval2021;
    float mod_retval2325 = mod(temp58, const60);
    float temp61 = mod_retval2325 + const62;
    float temp63 = textureCoordinate.y - mod_retval2021;
    float mod_retval2729 = mod(temp63, const60);
    float temp65 = mod_retval2729 + const62;
    vec2 temp66;
    temp66.xy = vec2(temp61, temp65);
    bool temp67 = mod_retval2021 > textureCoordinate.x;
    if (temp67) {
        float temp68 = textureCoordinate.x + const60;
        bool and_tmp33 = mod_retval2021 < temp68;
        and_tmp_0 = and_tmp33;
    }
    else {
        and_tmp_0 = const70;
    }
    
    direct_t_0 = const71;
    if (and_tmp_0) {
        vec4 texture2D_retval3536 = texture2D(inputImageTexture3, temp66);
        direct_t_0 = texture2D_retval3536;
    }
    
    float temp73 = frameTime / const74;
    float mod_retval4041 = mod(temp73, const39);
    vec3 temp76;
    temp76.xyz = texture2D_retval11.xyz;
    float temp77 = direct_t_0.w * mod_retval4041;
    float temp78 = const39 - temp77;
    vec3 temp79 = vec3(temp78);
    vec3 temp80 = temp76 * temp79;
    vec4 temp81 = texture2D_retval11;
    temp81.xyz = temp80.xyz;
    float temp82 = const83 - rtime13;
    float temp84 = temp82 / const39;
    vec2 temp85;
    temp85.xy = vec2(temp84, temp49);
    float dot_retval6263 = dot(temp85, const52);
    float sin_retval6465 = sin(dot_retval6263);
    float temp88 = sin_retval6465 * const55;
    float fract_retval6667 = fract(temp88);
    float mod_retval6869 = mod(fract_retval6667, const39);
    float mod_retval7172 = mod(rtime13, const92);
    float floor_retval73 = floor(mod_retval7172);
    float mod_retval7576 = mod(rtime13, const95);
    float floor_retval7778 = floor(mod_retval7576);
    float temp97 = floor_retval73 * const60;
    float temp98 = mod_retval2325 + temp97;
    float temp99 = textureCoordinate.y - mod_retval6869;
    float mod_retval8486 = mod(temp99, const60);
    float temp101 = floor_retval7778 * const60;
    float temp102 = mod_retval8486 + temp101;
    vec2 temp103;
    temp103.xy = vec2(temp98, temp102);
    if (temp67) {
        float temp104 = textureCoordinate.x + const60;
        bool and_tmp8892 = mod_retval2021 < temp104;
        and_tmp88_0 = and_tmp8892;
    }
    else {
        and_tmp88_0 = const70;
    }
    
    if (and_tmp88_0) {
        bool and_tmp9598 = mod_retval6869 > textureCoordinate.y;
        and_tmp95_0 = and_tmp9598;
    }
    else {
        and_tmp95_0 = const70;
    }
    
    if (and_tmp95_0) {
        float temp107 = textureCoordinate.y + const60;
        bool and_tmp101104 = mod_retval6869 < temp107;
        and_tmp101_0 = and_tmp101104;
    }
    else {
        and_tmp101_0 = const70;
    }
    
    direct_t45_0 = const71;
    if (and_tmp101_0) {
        vec4 texture2D_retval108109 = texture2D(inputImageTexture3, temp103);
        direct_t45_0 = texture2D_retval108109;
    }
    
    float temp110 = frameTime / const111;
    float mod_retval113114 = mod(temp110, const39);
    vec3 temp113;
    temp113.xyz = temp81.xyz;
    float temp114 = direct_t45_0.w * mod_retval113114;
    float temp115 = const39 - temp114;
    vec3 temp116 = vec3(temp115);
    vec3 temp117 = temp113 * temp116;
    vec4 texture2D_retval116117 = texture2D(inputImageTexture4, temp45);
    vec3 temp119;
    temp119.xyz = temp117.xyz;
    float temp120 = texture2D_retval116117.x * const121;
    float temp122 = const39 + temp120;
    vec3 temp123 = vec3(temp122);
    vec3 temp124 = temp119 * temp123;
    vec3 temp125;
    temp125.xyz = temp124.xyz;
    float dot_retval119120 = dot(temp125, const127);
    vec4 temp128 = vec4(dot_retval119120, dot_retval119120, dot_retval119120, const39);
    float mod_retval125126 = mod(frameTime, const130);
    bool temp131 = mod_retval125126 < const132;
    darknessFactor_0 = const39;
    if (temp131) {
        float mod_retval128129 = mod(frameTime, const74);
        float temp134 = mod_retval128129 / const74;
        float temp135 = temp134 * const136;
        float sin_retval130131 = sin(temp135);
        float temp138 = sin_retval130131 / const34;
        float darknessFactor132 = temp138 + const140;
        darknessFactor_0 = darknessFactor132;
    }
    
    vec4 texture2D_retval134135 = texture2D(inputImageTexture2, aTextCoord4);
    vec3 temp142;
    temp142.xyz = temp128.xyz;
    vec3 temp143 = vec3(darknessFactor_0);
    vec3 temp144 = temp142 * temp143;
    vec3 temp145;
    temp145.xyz = temp144.xyz;
    float temp146 = const39 - texture2D_retval134135.w;
    vec3 temp147 = vec3(temp146);
    vec3 temp148 = temp145 * temp147;
    vec4 temp149 = const71;
    temp149.x = temp148.x;
    vec4 temp150 = temp149;
    temp150.y = temp148.y;
    gl_FragColor = temp150;
    gl_FragColor.z = temp148.z;
    vec4 temp152 = vec4(temp148.x, temp148.y, temp148.z, const39);
    gl_FragColor = temp152;
    
}