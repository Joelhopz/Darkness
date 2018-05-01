
static const float HALF_PI      = 1.5707963267948966192313216916397514420985;
static const float PI           = 3.1415926535897932384626433832795028841971;
static const float TWO_PI       = 6.2831853071795864769252867665590057683942;
static const float ONE_DIV_PI   = 0.3183098861837906715377675267450287240689;
static const float FLT_EPS      = 1e-10;

float3 createNormal(float3x3 TBN, float3 normalMapSample)
{
    //return mul(TBN, normalMapSample.xyz);
    float3 remappedNormal = (normalMapSample * 2.0f) - 1.0f;
    float3 normalMapNormal = normalize(float3(remappedNormal.x, remappedNormal.y, remappedNormal.z));
    return mul(TBN, normalMapNormal);
}

// Lys constants
static const float k0 = 0.00098, k1 = 0.9921, fUserMaxSPow = 0.2425;
static const float g_fMaxT = (exp2(-10.0 / sqrt(fUserMaxSPow)) - k0) / k1;
static const int nMipOffset = 0;


float specPowerToMip(float specPower, int mips)
{
    float smulMax = (exp2(-10.0 / sqrt(specPower)) - k0) / k1;
    return (mips - 1 - nMipOffset) * (1.0f - clamp(smulMax / g_fMaxT, 0.0, 1.0));
}

float2 envMapEquirect(float3 normal)
{
    float phi = acos(normal.y);
    float theta = atan2(normal.x, normal.z) + PI;
    return float2(theta / TWO_PI, phi / PI);
    
    /*float2 res;
    res.y = normal.y;
    res.x = normalize(normal.xz).x * 0.5;
    float s = sign(normal.z) * 0.5;
    res.x = 0.75 - s * (0.5 - res.x);
    res.y = 0.5 + 0.5 * res.y;
    return res;*/
}

float4 sRGBtoLinear(float4 sRGB)
{
    float3 sr = sRGB.xyz;
    return float4((sr <= 0.04045f) ? (sr / 12.92f) : (pow(sr + 0.055f, 2.4f) / 1.055f), sRGB.w);
}

float sRGBtoLinear(float sRGB)
{
    return (sRGB <= 0.04045f) ? (sRGB / 12.92f) : (pow(sRGB + 0.055f, 2.4f) / 1.055f);
}

float4 linearTosRGB(float4 RGB)
{
    float3 rg = RGB.xyz;
    return float4((rg <= 0.0031308f) ? (rg*12.92f) : (1.055f * pow(rg, 1.0f / 2.4f) - 0.055f), RGB.w);
}

float3 sRGBtoLinear(float3 sRGB)
{
    return (sRGB <= 0.04045f) ? (sRGB / 12.92f) : (pow(sRGB + 0.055f, 2.4f) / 1.055f);
}

float3 linearTosRGB(float3 RGB)
{
    return (RGB <= 0.0031308f) ? (RGB*12.92f) : (1.055f * pow(RGB, 1.0f / 2.4f) - 0.055f);
}

float linearDepth(float depth, float2 nearFar)
{
    //return 1.0 / (((nearFar.y - nearFar.x) - nearFar.x) * depth + 1.0);
	return ((nearFar.y - nearFar.x) - nearFar.x) * depth;
}

float3 boxLineIntersect(float3 bbMin, float3 bbMax, float3 origin, float3 direction, float3 cubemapPosition)
{
    float3 firstPlaneIntersect = (bbMax - origin) / direction;
    float3 secondPlaneIntersect = (bbMin - origin) / direction;
    float3 furthest = max(firstPlaneIntersect, secondPlaneIntersect);
    float distance = min(min(furthest.x, furthest.y), furthest.z);
    float3 intersectionPoint = origin + direction * distance;
    return intersectionPoint - cubemapPosition;
}

float rgbToLuminance(float3 rgb)
{
    return dot(rgb, float3(0.212671, 0.715160, 0.072169));
}

/*bool linePlaneIntersect(float3 origin, float3 direction, float3 planePos, float3 planeNormal, out float3 intersectionPoint)
{
    intersectionPoint = float3(0.0f, 0.0f, 0.0f);
    float rdot = dot(direction, planeNormal);
    if (rdot < FLT_EPS)
    {
        return false;
    }
    intersectionPoint = origin + ((dot(planeNormal, (origin - planePos)) / rdot) * direction);
    return true;
};

bool boxLineIntersect(float3 bbMin, float3 bbMax, float3 origin, float3 direction, out float3 boxIntersectPoint)
{
    float3 intersect = float3(0.0f, 0.0f, 0.0f);

    float3 posx_norm = float3(-1.0f, 0.0f, 0.0f);
    float3 posx_pos = float3(bbMax.x, 0.0f, 0.0f);
    if (linePlaneIntersect(origin, direction, posx_pos, posx_norm, intersect))
    {
        boxIntersectPoint = intersect;
        return true;
    }

    float3 negx_norm = float3(1.0f, 0.0f, 0.0f);
    float3 negx_pos = float3(bbMin.x, 0.0f, 0.0f);
    if (linePlaneIntersect(origin, direction, negx_pos, negx_norm, intersect))
    {
        boxIntersectPoint = intersect;
        return true;
    }

    float3 posy_norm = float3(0.0f, -1.0f, 0.0f);
    float3 posy_pos = float3(0.0f, bbMax.y, 0.0f);
    if (linePlaneIntersect(origin, direction, posy_pos, posy_norm, intersect))
    {
        boxIntersectPoint = intersect;
        return true;
    }

    float3 negy_norm = float3(0.0f, 1.0f, 0.0f);
    float3 negy_pos = float3(0.0f, bbMin.y, 0.0f);
    if (linePlaneIntersect(origin, direction, negy_pos, negy_norm, intersect))
    {
        boxIntersectPoint = intersect;
        return true;
    }

    float3 posz_norm = float3(0.0f, 0.0f, -1.0f);
    float3 posz_pos = float3(0.0f, 0.0f, bbMax.z);
    if (linePlaneIntersect(origin, direction, posz_pos, posz_norm, intersect))
    {
        boxIntersectPoint = intersect;
        return true;
    }

    float3 negz_norm = float3(0.0f, 0.0f, 1.0f);
    float3 negz_pos = float3(0.0f, 0.0f, bbMin.z);
    if (linePlaneIntersect(origin, direction, negz_pos, negz_norm, intersect))
    {
        boxIntersectPoint = intersect;
        return true;
    }

    return false;
};*/

/*float3x3 orthoNormalBase(float3 n)
{
    float s = sign(n);
    float a = -1.0f / (s + n.z);
    float b = n.x * n.y * a;
    return float3x3(
        n,
        float3(1.0f + s * n.x * n.x * a, s * b, -s * n.x),
        float3(b, s + n.y * n.y * a, -n.y));
}*/

// Samples a texture with Catmull-Rom filtering, using 9 texture fetches instead of 16.
// See http://vec3.ca/bicubic-filtering-in-fewer-taps/ for more details
float4 SampleTextureCatmullRom(in Texture2D<float4> tex, in sampler linearSampler, in float2 uv, in float2 texSize)
{
    // We're going to sample a a 4x4 grid of texels surrounding the target UV coordinate. We'll do this by rounding
    // down the sample location to get the exact center of our "starting" texel. The starting texel will be at
    // location [1, 1] in the grid, where [0, 0] is the top left corner.
    float2 samplePos = uv * texSize;
    float2 texPos1 = floor(samplePos - 0.5f) + 0.5f;

    // Compute the fractional offset from our starting texel to our original sample location, which we'll
    // feed into the Catmull-Rom spline function to get our filter weights.
    float2 f = samplePos - texPos1;

    // Compute the Catmull-Rom weights using the fractional offset that we calculated earlier.
    // These equations are pre-expanded based on our knowledge of where the texels will be located,
    // which lets us avoid having to evaluate a piece-wise function.
    float2 w0 = f * (-0.5f + f * (1.0f - 0.5f * f));
    float2 w1 = 1.0f + f * f * (-2.5f + 1.5f * f);
    float2 w2 = f * (0.5f + f * (2.0f - 1.5f * f));
    float2 w3 = f * f * (-0.5f + 0.5f * f);

    // Work out weighting factors and sampling offsets that will let us use bilinear filtering to
    // simultaneously evaluate the middle 2 samples from the 4x4 grid.
    float2 w12 = w1 + w2;
    float2 offset12 = w2 / (w1 + w2);

    // Compute the final UV coordinates we'll use for sampling the texture
    float2 texPos0 = texPos1 - 1;
    float2 texPos3 = texPos1 + 2;
    float2 texPos12 = texPos1 + offset12;

    texPos0 /= texSize;
    texPos3 /= texSize;
    texPos12 /= texSize;

    float4 result = 0.0f;
    result += tex.SampleLevel(linearSampler, float2(texPos0.x, texPos0.y), 0.0f) * w0.x * w0.y;
    result += tex.SampleLevel(linearSampler, float2(texPos12.x, texPos0.y), 0.0f) * w12.x * w0.y;
    result += tex.SampleLevel(linearSampler, float2(texPos3.x, texPos0.y), 0.0f) * w3.x * w0.y;

    result += tex.SampleLevel(linearSampler, float2(texPos0.x, texPos12.y), 0.0f) * w0.x * w12.y;
    result += tex.SampleLevel(linearSampler, float2(texPos12.x, texPos12.y), 0.0f) * w12.x * w12.y;
    result += tex.SampleLevel(linearSampler, float2(texPos3.x, texPos12.y), 0.0f) * w3.x * w12.y;

    result += tex.SampleLevel(linearSampler, float2(texPos0.x, texPos3.y), 0.0f) * w0.x * w3.y;
    result += tex.SampleLevel(linearSampler, float2(texPos12.x, texPos3.y), 0.0f) * w12.x * w3.y;
    result += tex.SampleLevel(linearSampler, float2(texPos3.x, texPos3.y), 0.0f) * w3.x * w3.y;

    return result;
}

// https://software.intel.com/en-us/node/503873
float3 RGB_YCoCg(float3 c)
{
    // Y = R/4 + G/2 + B/4
    // Co = R/2 - B/2
    // Cg = -R/4 + G/2 - B/4
    return float3(
        c.x / 4.0 + c.y / 2.0 + c.z / 4.0,
        c.x / 2.0 - c.z / 2.0,
        -c.x / 4.0 + c.y / 2.0 - c.z / 4.0
        );
}

// https://software.intel.com/en-us/node/503873
float3 YCoCg_RGB(float3 c)
{
    // R = Y + Co - Cg
    // G = Y + Cg
    // B = Y - Co - Cg
    return saturate(float3(
        c.x + c.y - c.z,
        c.x + c.z,
        c.x - c.y - c.z
        ));
}

static const float2 halton[16] =
{
    float2(0.000000, 0.000000),
    float2(0.500000, 0.333333),
    float2(0.250000, 0.666667),
    float2(0.750000, 0.111111),
    float2(0.125000, 0.444444),
    float2(0.625000, 0.777778),
    float2(0.375000, 0.222222),
    float2(0.875000, 0.555556),
    float2(0.062500, 0.888889),
    float2(0.562500, 0.037037),
    float2(0.312500, 0.370370),
    float2(0.812500, 0.703704),
    float2(0.187500, 0.148148),
    float2(0.687500, 0.481481),
    float2(0.437500, 0.814815),
    float2(0.937500, 0.259259)
};

static const float A = 0.15;
static const float B = 0.50;
static const float C = 0.10;
static const float D = 0.20;
static const float E = 0.02;
static const float F = 0.30;
static const float W = 11.2;

float3 Uncharted2TonemapHelper(float3 x)
{
    return ((x*(A*x + C*B) + D*E) / (x*(A*x + B) + D*F)) - E / F;
}

float3 Uncharted2Tonemap(float3 x)
{
    float ExposureBias = 2.0f;
    float3 curr = Uncharted2TonemapHelper(ExposureBias * x);

    float3 whiteScale = 1.0f / Uncharted2TonemapHelper(W);
    float3 color = curr*whiteScale;

    return linearTosRGB(color);
}

float3 JimHejlRichardBurgessDawsonTonemap(float3 val)
{
    float3 x = max(0, val - 0.004);
    float3 retColor = (x*(6.2*x + .5)) / (x*(6.2*x + 1.7) + 0.06);
    return retColor;
}

float3 ReinhardTonemapping(float3 val)
{
    float3 texColor = val;
    texColor = texColor / (1 + texColor);
    return pow(texColor, 1 / 2.2);
}

// The code in this file was originally written by Stephen Hill (@self_shadow), who deserves all
// credit for coming up with this fit and implementing it. Buy him a beer next time you see him. :)

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
static const float3x3 ACESInputMat =
{
    { 0.59719, 0.35458, 0.04823 },
    { 0.07600, 0.90834, 0.01566 },
    { 0.02840, 0.13383, 0.83777 }
};

// ODT_SAT => XYZ => D60_2_D65 => sRGB
static const float3x3 ACESOutputMat =
{
    { 1.60475, -0.53108, -0.07367 },
    { -0.10208,  1.10813, -0.00605 },
    { -0.00327, -0.07276,  1.07602 }
};

float3 RRTAndODTFit(float3 v)
{
    float3 a = v * (v + 0.0245786f) - 0.000090537f;
    float3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

float3 ACESFitted(float3 color)
{
    color = mul(ACESInputMat, color);

    // Apply RRT and ODT
    color = RRTAndODTFit(color);

    color = mul(ACESOutputMat, color);

    // Clamp to [0, 1]
    color = saturate(color);

    return color;
}
