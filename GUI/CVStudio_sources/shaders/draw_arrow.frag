# version 330
#define AA 4
#define PI 3.1415926
#define ITERS 64

const float deg2rad = PI / 180.0;

uniform vec3 rotation;   // = vec3(0.0, 0.0, 90.0); //X, Y, Z [degrees]
uniform vec2 position;   // = vec2(400.0, 225.0);   //X, Y [pixels]
uniform vec2 resolution; //

out vec4 out_color;

// Signed distance function for the capped cone primitive https://iquilezles.org/articles/distfunctions
float sdCone(vec3 p, vec3 a, vec3 b, float ra, float rb)
{
    float rba  = rb-ra;
    float baba = dot(b-a,b-a);
    float papa = dot(p-a,p-a);
    float paba = dot(p-a,b-a)/baba;

    float x = sqrt(papa - paba*paba*baba);

    float cax = max(0.0, x-((paba<0.5)?ra:rb));
    float cay = abs(paba-0.5)-0.5;

    float k = rba*rba + baba;
    float f = clamp( (rba*(x-ra)+paba*baba)/k, 0.0, 1.0 );

    float cbx = x-ra - f*rba;
    float cby = paba - f;
    
    float s = (cbx < 0.0 && cay < 0.0) ? -1.0 : 1.0;
    
    return s*sqrt( min(cax*cax + cay*cay*baba,
                       cbx*cbx + cby*cby*baba) );
}

// Compute rotation matrix from Euler angles
mat3 eulerToMat3(float pitch, float yaw, float roll) // X, Y, Z
{
    // Convention: YXZ order (yaw, pitch, roll)
    float cp = cos(pitch);
    float sp = sin(pitch);
    float cy = cos(yaw);
    float sy = sin(yaw);
    float cr = cos(roll);
    float sr = sin(roll);

    mat3 m;
    m[0] = vec3(cy*cr + sy*sp*sr,  sr*cp, -sy*cr + cy*sp*sr);
    m[1] = vec3(-cy*sr + sy*sp*cr, cr*cp,  sr*sy + cy*sp*cr);
    m[2] = vec3(sy*cp,            -sp,     cy*cp);

    return m;
}


// Exponential smoothing
float smin(float a, float b, float k)
{
    float h = clamp(0.5 + 0.5*(b-a)/k, 0.0, 1.0);
    return mix(b, a, h) - k*h*(1.0-h);
}

// Comparing the distances
vec2 opU(vec2 d1, vec2 d2)
{
    float k = 0.01;
    float dst = smin(d1.x, d2.x, k);
    float mat = (d1.x < d2.x) ? d1.y : d2.y;
    return vec2(dst, mat);
}


vec2 draw_arrow(in vec3 pos, vec3 axis, float ID)
{
    float arrow_len = 0.135;
    float thickness = 0.005;
    
    vec3 axis_1 = normalize(axis) * arrow_len;
    
    vec2 cyl  = vec2( sdCone(pos, vec3(0.0), axis_1, thickness, thickness), ID );
    vec2 cone = vec2( sdCone(pos, axis_1, axis_1*1.2, thickness*4., 0.0),   ID );
    
    return opU(cyl, cone);
}


vec2 draw_axes(in vec3 ray_hit)
{   
    mat3 R = eulerToMat3(rotation.x * deg2rad,
                         rotation.y * deg2rad,
                         rotation.z * deg2rad);
                         
    vec2 aX = draw_arrow(ray_hit, vec3(1., 0., 0.)*R, 0.);
    vec2 aY = draw_arrow(ray_hit, vec3(0.,-1., 0.)*R, 1.);
    vec2 aZ = draw_arrow(ray_hit, vec3(0., 0., 1.)*R, 2.);
    
    vec2 res = opU(aX, aY);
    res = opU(res, aZ);
    
    return res;
}

// Calculates normals from sampled signed distance https://iquilezles.org/articles/normalsSDF
vec3 calc_normals(in vec3 ray_hit)
{
    vec2 e = vec2(1.0,-1.0) * 0.5773;
    const float eps = 0.0005;
    return normalize( e.xyy*draw_axes(ray_hit + e.xyy*eps).x + // .x stays for distance value
					  e.yyx*draw_axes(ray_hit + e.yyx*eps).x + 
					  e.yxy*draw_axes(ray_hit + e.yxy*eps).x + 
					  e.xxx*draw_axes(ray_hit + e.xxx*eps).x );
}
    

mat3 setCamera(in vec3 ro, in vec3 ta, float cr)
{
	vec3 cw = normalize(ta-ro);
	vec3 cp = vec3(sin(cr), cos(cr),0.0);
	vec3 cu = normalize( cross(cw,cp) );
	vec3 cv =          ( cross(cu,cw) );
    return mat3(cu, cv, cw);
}

// Get arrow color depending on the material id in the pixel
vec3 getRGBVector(float value)
{
    vec3 colors[3];
    colors[0] = vec3(1.0, 0.0, 0.0);  // Red
    colors[1] = vec3(0.0, 1.0, 0.0);  // Green
    colors[2] = vec3(0.0, 0.0, 1.0);  // Blue

    int index = int(mod(value, 3.0));
    float fraction = fract(value);
    vec3 color1 = colors[index];
    vec3 color2 = colors[(index + 1) % 3];
    return mix(color1, color2, fraction);
}


void main()
{  
    vec2 screen_pos = 2.0*position/resolution.xy - 1.0; // normalized pixel coordinates
    vec2 stretch = vec2(-resolution.x / resolution.y, 1.0); //inverts X-axis and stretches coordinates to rectangle

    vec3 ta = vec3(screen_pos*stretch, 0.0); //camera position
    vec3 ro = ta + vec3(0., 0., 1.);
    mat3 ca = setCamera(ro, ta, 0.0); // camera-to-world transformation
    vec4 tot = vec4(0.0); // final accumulated pixel color + alpha channel
    
 
    for(int m=0; m<AA; m++) // anti-aliasing loops
    {
        for(int n=0; n<AA; n++)
        {
            // Oversampled pixel coordinates
            vec2 o = vec2(float(m), float(n)) / float(AA) - 0.5;
            vec2 p = (2.0*(gl_FragCoord.xy+o)-resolution.xy) / resolution.y;
            
            // Initializing the ray in orthographiv projection
            vec3 rd = ca * vec3(0.0, 0.0, 1.0); // constant ray direction (down Z)
            vec3 rayOrigin = ro + ca * vec3(p, 0.0); // ray origin changes with p

            // Raymarching
            const float tmax = 15.0; //far clip
            float t = 0.0;
            float mat_ID = 0.0;
            
            for(int i=0; i<ITERS; i++)
            {
                vec3 ray_hit = rayOrigin + t*rd;
                vec2 buf = draw_axes(ray_hit);
                float h = buf.x;
                mat_ID  = buf.y;
                if(h<0.0001 || t>tmax ) break;
                t += h;
            }
            
            // Shading/lighting	
            const vec3 lightDir = vec3(0.57703);
            const vec3 amb = vec3(0.1);
            vec4 col = vec4(0.0);
            
            if(t < tmax)
            {
                vec3 ray_hit = rayOrigin + t*rd;
                vec3 nor = calc_normals(ray_hit);
                float dif = clamp(dot(nor, lightDir), 0.0, 1.0);
                col = vec4(getRGBVector(mat_ID)*(dif+amb), 1.0);
            }
            
            // Gamma correction, just a beauty touch
            col = pow(col, vec4(0.4545));
            tot += col;
        }
    }
	out_color = tot / float(AA*AA);
}