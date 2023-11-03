Texture2D g_texture0 : register(t0);
Texture2D g_texture2 : register(t2);

SamplerState g_sampler : register(s0);

cbuffer PixelConstantBuffer : register(b0)
{
    float time;
    float pad1;
    float pad2;
    float pad3;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};


//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

#define DITHERING
#define BACKGROUND

//#define TONEMAPPING

//-------------------
#define pi 3.14159265
#define R(p, a) p=cos(a)*p+sin(a)*float2(p.y, -p.x)

// iq's noise
float noise(in float3 x)
{
    float3 p = floor(x);
    float3 f = frac(x);
    f = f * f * (3.0 - 2.0 * f);
    float2 uv = (p.xy + float2(37.0, 17.0) * p.z) + f.xy;
    float2 rg = g_texture0.SampleLevel(g_sampler, (uv + 0.5) / 256.0, 0.0).yx;
    return 1. - 0.82 * lerp(rg.x, rg.y, f.z);
}

float fbm(float3 p)
{
    return noise(p * .06125) * .5 + noise(p * .125) * .25 + noise(p * .25) * .125 + noise(p * .4) * .2;
}

float length2(float2 p)
{
    return sqrt(p.x * p.x + p.y * p.y);
}

float length8(float2 p)
{
    p = p * p;
    p = p * p;
    p = p * p;
    return pow(p.x + p.y, 1.0 / 8.0);
}


float Disk(float3 p, float3 t)
{
    float2 q = float2(length2(p.xy) - t.x, p.z * 0.5);
    return max(length8(q) - t.y, abs(p.z) - t.z);
}

//==============================================================
// otaviogood's noise from https://www.shadertoy.com/view/ld2SzK
//--------------------------------------------------------------
// This spiral noise works by successively adding and rotating sin waves while increasing frequency.
// It should work the same on all computers since it's not based on a hash function like some other noises.
// It can be much faster than other noise functions if you're ok with some repetition.
static const float nudge = 0.9; // size of perpendicular vector
float normalizer = 1.0 / sqrt(1.0 + nudge * nudge); // pythagorean theorem on that perpendicular to maintain scale
float SpiralNoiseC(float3 p)
{
    float n = 0.0; // noise amount
    float iter = 2.0;
    for (int i = 0; i < 8; i++)
    {
        // add sin and cos scaled inverse with the frequency
        n += -abs(sin(p.y * iter) + cos(p.x * iter)) / iter; // abs for a ridged look
        // rotate by adding perpendicular and scaling down
        p.xy += float2(p.y, -p.x) * nudge;
        p.xy *= normalizer;
        // rotate on other axis
        p.xz += float2(p.z, -p.x) * nudge;
        p.xz *= normalizer;
        // increase the frequency
        iter *= 1.733733;
    }
    return n;
}

float NebulaNoise(float3 p)
{
    float final = Disk(p.xzy, float3(2.0, 1.8, 1.25));
    final += fbm(p * 90.);
    final += SpiralNoiseC(p.zxy * 0.5123 + 100.0) * 3.0;

    return final;
}

float map(float3 p)
{
	R(p.xz, 0.008 * pi + time * 0.1);

    float NebNoise = abs(NebulaNoise(p / 0.5) * 0.5);
    
    return NebNoise + 0.07;
}
//--------------------------------------------------------------

// assign color to the media
float3 computeColor(float density, float radius)
{
	// color based on density alone, gives impression of occlusion within
	// the media
    float3 result = lerp(float3(1.0, 0.9, 0.8), float3(0.4, 0.15, 0.1), density);
	
	// color added to the media
    float3 colCenter = 7. * float3(0.8, 1.0, 1.0);
    float3 colEdge = 1.5 * float3(0.48, 0.53, 0.5);
    result *= lerp(colCenter, colEdge, min((radius + .05) / .9, 1.15));
	
    return result;
}

bool RaySphereIntersect(float3 org, float3 dir, out float near, out float far)
{
    float b = dot(dir, org);
    float c = dot(org, org) - 8.;
    float delta = b * b - c;
    if (delta < 0.0) 
        return false;
    float deltasqrt = sqrt(delta);
    near = -b - deltasqrt;
    far = -b + deltasqrt;
    return far > 0.0;
}

// Applies the filmic curve from John Hable's presentation
// More details at : http://filmicgames.com/archives/75
float3 ToneMapFilmicALU(float3 _color)
{
    _color = max(float3(0.0, 0.0, 0.0), _color - float3(0.004, 0.004, 0.004));
    _color = (_color * (6.2 * _color + float3(0.5, 0.5, 0.5))) / (_color * (6.2 * _color + float3(1.7, 1.7, 1.7)) + float3(0.06, 0.06, 0.06));
    return _color;
}

float4 mainImage(in float2 fragCoord)
{
    float2 iResolution = float2(1600.0, 900.0);
    float4 fragColor = (float4) 0;

	// ro: ray origin
	// rd: direction of the ray
    float3 rd = normalize(float3((fragCoord.xy - 0.5 * iResolution.xy) / iResolution.y, 1.));
    float3 ro = float3(0.0, 0.0, -6.0);
    
	// ld, td: local, total density 
	// w: weighting factor
    float ld = 0., td = 0., w = 0.;

	// t: length of the ray
	// d: distance function
    float d = 1., t = 0.;
    
    const float h = 0.1;
   
    float4 sum = float4(0.0, 0.0, 0.0, 0.0);
   
    float min_dist = 0.0, max_dist = 0.0;

    if (RaySphereIntersect(ro, rd, min_dist, max_dist))
    {
       
        t = min_dist * step(t, min_dist);
   
	// raymarch loop
        for (int i = 0; i < 64; i++)
        {
	 
            float3 pos = ro + t * rd;
  
		// Loop break conditions.
            if (td > 0.9 || d < 0.1 * t || t > 10. || sum.a > 0.99 || t > max_dist)
                break;
        
        // evaluate distance function
            float d = map(pos);
		       
		// change this string to control density 
            d = max(d, 0.0);
        
        // point light calculations
            float3 ldst = float3(0.0, 0.0, 0.0) - pos;
            float lDist = max(length(ldst), 0.001);

        // the color of light 
            float3 lightColor = float3(1.0, 0.5, 0.25);
        
            sum.rgb += (float3(0.67, 0.75, 1.00) / (lDist * lDist * 10.) / 80.); // star itself
            sum.rgb += (lightColor / exp(lDist * lDist * lDist * .08) / 30.); // bloom
        
            if (d < h)
            {
			// compute local density 
                ld = h - d;
            
            // compute weighting factor 
                w = (1. - td) * ld;
     
			// accumulate density
                td += w + 1. / 200.;
		
                float4 col = float4(computeColor(td, lDist), td);
            
            // emission
                sum += sum.a * float4(sum.rgb, 0.0) * 0.2;
            
			// uniform scale density
                col.a *= 0.2;
			// colour by alpha
                col.rgb *= col.a;
			// alpha blend in contribution
                sum = sum + col * (1.0 - sum.a);
       
            }
      
            td += 1. / 70.;

#ifdef DITHERING
        //idea from https://www.shadertoy.com/view/lsj3Dw
            float2 uv = fragCoord.xy / iResolution.xy;
            uv.y *= 120.;
            uv.x *= 280.;
            d = abs(d) * (.8 + 0.08 * g_texture2.Sample(g_sampler, float2(uv.y, -uv.x + 0.5 * sin(4. * time + uv.y * 4.0))).r);
#endif 
		
        // trying to optimize step size near the camera and near the light source
            t += max(d * 0.1 * max(min(length(ldst), length(ro)), 1.0), 0.01);
        
        }
    
    // simple scattering
        sum *= 1. / exp(ld * 0.2) * 0.6;
        
        sum = clamp(sum, 0.0, 1.0);
   
        sum.xyz = sum.xyz * sum.xyz * (3.0 - 2.0 * sum.xyz);
    
    }

#ifdef BACKGROUND
    // stars background
    if (td < 0.8)
    {
        float noiseResult = noise(rd * 500.0);
        float3 stars = float3(noiseResult * 0.5 + 0.5, noiseResult * 0.5 + 0.5, noiseResult * 0.5 + 0.5);
        float3 starbg = float3(0.0, 0.0, 0.0);
        starbg = lerp(starbg, float3(0.8, 0.9, 1.0), smoothstep(0.99, 1.0, stars) * clamp(dot(float3(0.0, 0.0, 0.0), rd) + 0.75, 0.0, 1.0));
        starbg = clamp(starbg, 0.0, 1.0);
        sum.xyz += starbg;
    }
#endif
   
#ifdef TONEMAPPING
    fragColor = float4(ToneMapFilmicALU(sum.xyz*2.2),1.0);
#else
    fragColor = float4(sum.xyz, 1.0);
#endif
    
    return fragColor;
}

float4 main(PSInput input) : SV_TARGET
{
    float4 color = (float4) 0;
    
    color = mainImage(input.uv);
    
    color = float4(1.0f, 0.0f, 0.0f, 1.0f);
    
    return color;
}