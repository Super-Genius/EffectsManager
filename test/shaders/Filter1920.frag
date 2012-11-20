 uniform sampler2D inputImageTexture;
 uniform sampler2D inputImageTexture2;      // r = movie frame, g = noise, b = dust/lines
 
 uniform highp float frameTime;
 uniform highp float vignetteX;
 uniform highp float vignetteY;
 
 varying  highp vec2 textureCoordinate;
 
 const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);
 
 
 highp float rand(highp vec2 co){
     return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
 }
 
 lowp float computeFilmLines (highp float rtime)
{
	//
	highp vec2 	charMapCoord;
	highp float 	texInterval 	= 512.0/32.0;
	highp float 	textureOffset 	= (1.0/512.0) * 32.0;
	
    
	highp float timeLoop = mod(rand(vec2(rtime/100.0, rtime/100.0)), 1.0);
    highp float xindex = mod(rand(vec2(rtime, rtime)), 16.0);
	
	charMapCoord.x  = (mod(textureCoordinate.x - timeLoop, textureOffset) + (xindex * textureOffset)) * (512.0/640.0);
	charMapCoord.y  = (mod(textureCoordinate.y - timeLoop, textureOffset) + (6.0 * textureOffset)) * (512.0/960.0);
    
    highp float direct_t = 0.0;
    
	if (timeLoop > textureCoordinate.x  && 
        timeLoop < textureCoordinate.x + textureOffset //&&
        //			timeLoop > gl_TexCoord[0].y  && 
        //			timeLoop < gl_TexCoord[0].y + textureOffset 
        
        ) {
	 	direct_t = (1.0-texture2D(inputImageTexture2, charMapCoord).b);
	}
	return direct_t;
}

 lowp float computeFilmSpecks (highp float rtime)
{
    //
    highp vec2      charMapCoord;
    highp float     texInterval     = 512.0/32.0;
    highp float     textureOffset   = (1.0/512.0) * 32.0;
    
    
    highp float timeLoop_x = mod(rand(vec2(rtime/2.0, rtime/2.0)), 1.0);
    highp float timeLoop_y = mod(rand(vec2(-rtime/1.0, rtime/2.0)), 1.0);
    //highp float timeLoop = mod(rtime, 1.0);

    //
    highp float xindex = floor(mod(rtime, 16.0));
    highp float yindex = floor(mod(rtime, 4.0));
    charMapCoord.x  = (mod(textureCoordinate.x - timeLoop_x, textureOffset) + (xindex * textureOffset)) * (512.0 / 640.0);
    charMapCoord.y  = (mod(textureCoordinate.y - timeLoop_y, textureOffset) + (yindex * textureOffset)) * (512.0 / 960.0);
    //charMapCoord.x  = (mod(textureCoordinate.x - timeLoop_x, textureOffset) + (xindex * textureOffset)) * (512.0 / 640.0);
    //charMapCoord.y  = (mod(textureCoordinate.y - timeLoop_y, textureOffset) + (yindex * textureOffset)) * (512.0 / 960.0);

    highp float direct_t = 0.0;
    
    if ( timeLoop_x > textureCoordinate.x  && 
        timeLoop_x < textureCoordinate.x + textureOffset &&
                  timeLoop_y > textureCoordinate.y  && 
                  timeLoop_y < textureCoordinate.y + textureOffset 
        
        ) {
        direct_t = (1.0-texture2D(inputImageTexture2, charMapCoord).b);
    }
    return direct_t;
}

 lowp float computeVignette()
{
    lowp float d = distance(textureCoordinate, vec2(0.5,0.5));
    return smoothstep(vignetteX, vignetteY, d);
}
 
 void main()
 {
     highp mat4 colorMat;
     colorMat[0] = vec4( 0.3588, 0.7044, 0.1368, 0.0 );
     colorMat[1] = vec4(0.2990, 0.5870, 0.1140, 0.0);
     colorMat[2] = vec4(0.2392, 0.4696, 0.0912 ,0.0);
     colorMat[3] = vec4(0,0,0,1.0);
     
     highp float actualFrameTime;
     
     actualFrameTime = 0.0;
     if ( 1 == 1 ) {
         actualFrameTime = frameTime;
     }
     
     highp vec2 centerTexCoord 	= (textureCoordinate - vec2(0.5,0.5)) * 0.9;
     highp float fAnimation = 1.0-((sin(actualFrameTime/2.5) * 0.0075));
     highp vec2 aTextCoord 		= centerTexCoord * fAnimation;
     aTextCoord 		= (aTextCoord) + vec2(0.5,0.5);
     
     highp vec2 filmTexCoord 	= textureCoordinate;
     //filmTexCoord.x = fAnimation;
     highp vec2 noiseTexCoord   = textureCoordinate;
     noiseTexCoord.y = mod(noiseTexCoord.y+sin(actualFrameTime/2.0), 1.0);
     
     highp vec4 textureColor 	= texture2D(inputImageTexture, textureCoordinate);

     highp float m3 = mod( float( frameTime ), 1500.0 );
     if (1 == 1)
     {
         textureColor.rgb *= computeVignette();
     }
     
     // film
     if ( 1 == 0 ) {
         //		if ( 1.0 / mod(frameTime, 100)
         highp float filmColor		= texture2D(inputImageTexture2, filmTexCoord).b;
         textureColor  *= 1.0-(filmColor * mod(frameTime/100.0, 1.0));
     }

     // film lines
     if ( 1 == 1 ) {
         highp float visibleTime = mod(frameTime, 200.0);
         if (visibleTime > 100.0) {
             textureColor.rgb  *= 1.0-((computeFilmLines(frameTime/100.0) * .3) * mod(frameTime/150.0, 1.0));
         }
     }

     // film specks
     if ( 1 == 1 ) {
         textureColor.rgb  *= 1.0-(computeFilmSpecks(frameTime/10.0) * mod(frameTime/20.0, 1.0));
     }

     // grain
     if ( 1 == 1) {
         highp float noiseColor	= texture2D(inputImageTexture2, noiseTexCoord).g;
         textureColor.rgb 		=  textureColor.rgb * (1.0 + (noiseColor * 0.3));	
     }
     
     // sepia
     if ( 1 == 0 ) {
         highp float intensity 		= 0.75;
         highp vec4 sepiaColor 		= textureColor * colorMat;
         textureColor 		= (intensity * sepiaColor) + ((1.0 - intensity) * textureColor);
     }
     
     // grey
     if ( 1 == 1 ) {
         highp float luminance = dot(textureColor.xyz, W);
         textureColor 			= vec4(vec3(luminance), 1.0);
     }
     
     highp float darknessFactor = 1.0;//for 1/7 of the time- darkenss Factor is constant
     
     // flicker
     if ( 1 == 0 ) {
         if( m3 < 1000.0 )//for 6/7 of the time- darkenss factor is variable
             darknessFactor = sin( mod(float( frameTime ), 100.0)/100.0 * 3.14 ) / 2.0 + 0.7;//results -- was 1.5 versus 1.0 in flickering
     }	
     
     // old movie frame, animated
     //highp float alpha 		= 1.0 - texture2D(inputImageTexture2, aTextCoord).r;
     highp float alpha 		= texture2D(inputImageTexture2, aTextCoord).r;
     textureColor           = vec4(vec3(textureColor.xyz) * darknessFactor, 1.0);
     
     //	textureColor = noiseColor;
     gl_FragColor.rgb = textureColor.xyz * alpha;
     gl_FragColor.a = 1.0;
     //gl_FragColor.rgba = vec4(texture2D(inputImageTexture2, textureCoordinate).r); 
     //gl_FragColor.rgba = vec4(0.0, 0.0, 1.0, 1.0);
 }
 
