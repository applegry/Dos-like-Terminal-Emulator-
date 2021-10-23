uniform float time;
uniform vec2 resolution;
uniform sampler2D texture;
uniform vec4 clr;
uniform float pw;

#define iTime time
#define iResolution resolution
#define AA 1
const float tau =atan(1.)*8.;

float rand(vec2 co) {
    return fract(sin(mod(dot(co.xy ,vec2(12.9898,78.233)),3.14))*43758.5453);
}


void mainImage( out vec4 o, in vec2 fragCoord ){
	
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec2 uv = (fragCoord-iResolution.xy/2.)/iResolution.y;
    o = mix(vec4(clr.r,clr.g,clr.b,clr.a),vec4(pixel.r,pixel.g,pixel.b,1),pw);
    float l=cos(fragCoord.y);
    l*=l;
    l/=3.;
    l+=0.8+rand(uv*iTime);
    
    o*=l;
	//o = pixel;
}

void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
    gl_FragColor.a = 0.5;
}