uniform highp int typeShading; // 1 - плоское затененеи, 2 - плавное ГУРО, 3 - плавное ФОНГО

uniform sampler2D u_texture;
uniform highp vec4 u_light_position;
uniform highp float u_light_power;
uniform highp vec3 u_light_color; //Используем вектор RGB для задания цвета источника света 

varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;

// плавное затенение Методам Фонга
vec4 phongShading() {
    vec4 diff_mat_color = texture2D(u_texture, v_texcoord);
    vec3 normal = normalize(v_normal);
    vec3 light_dir = normalize(v_position.xyz - u_light_position.xyz);
    vec3 view_dir = normalize(-v_position.xyz);

    // Расчет интенсивности света (диффузная составляющая):
    float diffuse = max(dot(normal, light_dir), 0.0);

    // Расчет интенсивности света (зеркальная составляющая):
    vec3 reflected_light_dir = reflect(-light_dir, normal);
    float specular = pow(max(dot(reflected_light_dir, view_dir), 0.0), 32.0);

    // Расчет цвета для вершины:
    vec3 vertex_color = vec3(0.0);
    vertex_color += diff_mat_color.xyz * u_light_power * diffuse * u_light_color;
    vertex_color += vec3(1.0) * u_light_power * specular;

    // Интерполяция цвета между вершинами:
    vec3 prev_color = vec3(0.0);
    vec3 next_color = vec3(0.0);
    vec3 avg_color = vec3(0.0);
    vec3 interp_color = vec3(0.0);

    if(gl_FrontFacing) { // Если полигон смотрит вперед
	prev_color = vec3(gl_FragCoord.z > gl_FragCoord.z - v_position.w ? texture2D(u_texture, v_texcoord - vec2(0.0, 0.01)).xyz : vertex_color);
	next_color = vec3(gl_FragCoord.z > gl_FragCoord.z + v_position.w ? texture2D(u_texture, v_texcoord + vec2(0.0, 0.01)).xyz : vertex_color);
    } else { // Если полигон смотрит назад
	prev_color = vec3(gl_FragCoord.z < gl_FragCoord.z - v_position.w ? texture2D(u_texture, v_texcoord - vec2(0.0, 0.01)).xyz : vertex_color);
	next_color = vec3(gl_FragCoord.z < gl_FragCoord.z + v_position.w ? texture2D(u_texture, v_texcoord + vec2(0.0, 0.01)).xyz : vertex_color);
    }

    avg_color = (prev_color + next_color + vertex_color) / 3.0;
    interp_color = mix(vertex_color, avg_color, 0.5);
    return vec4(interp_color, diff_mat_color.w);
}

// плавное затенение Методам Гуро

vec4 guroShading() {
    vec4 diff_mat_color = texture2D(u_texture, v_texcoord);
     vec3 normal = normalize(v_normal);
     vec3 light_dir = normalize(v_position.xyz - u_light_position.xyz);
     
     // Расчет интенсивности света (диффузная составляющая):
     float diffuse = max(dot(normal, light_dir), 0.0);
     
     // Расчет цвета для вершины:
     vec3 vertex_color = vec3(0.0);
     vertex_color += diff_mat_color.xyz * u_light_power * diffuse * u_light_color;
     
     // Интерполяция цвета между вершинами:
     vec3 prev_color = vec3(0.0);
     vec3 next_color = vec3(0.0);
     vec3 avg_color = vec3(0.0);
     vec3 interp_color = vec3(0.0);
     
     if(gl_FrontFacing) { // Если полигон смотрит вперед
	 prev_color = vec3(gl_FragCoord.z > gl_FragCoord.z - v_position.w ? texture2D(u_texture, v_texcoord - vec2(0.0, 0.01)).xyz : vertex_color);
	 next_color = vec3(gl_FragCoord.z > gl_FragCoord.z + v_position.w ? texture2D(u_texture, v_texcoord + vec2(0.0, 0.01)).xyz : vertex_color);
     } else { // Если полигон смотрит назад
	 prev_color = vec3(gl_FragCoord.z < gl_FragCoord.z - v_position.w ? texture2D(u_texture, v_texcoord - vec2(0.0, 0.01)).xyz : vertex_color);
	 next_color = vec3(gl_FragCoord.z < gl_FragCoord.z + v_position.w ? texture2D(u_texture, v_texcoord + vec2(0.0, 0.01)).xyz : vertex_color);
     }
     
     avg_color = (prev_color + next_color + vertex_color) / 3.0;
     interp_color = mix(vertex_color, avg_color, 0.5);
     
     return vec4(interp_color, diff_mat_color.w);
}

// ПЛОСОКОЕ ЗАТЕНЕИЕ 

vec4 flatShading() {
    vec4 diff_mat_color = texture2D(u_texture, v_texcoord);
    vec3 light_direction = normalize(u_light_position.xyz - v_normal);
    float diffuse = max(dot(v_normal, light_direction), 0.0);
    return diff_mat_color * vec4(u_light_color* u_light_power * diffuse, 1.0);
}

void main() {
    
    vec4 shading;
    if (typeShading == 1) {
	shading = flatShading();
    } else if (typeShading == 2) {
	shading = guroShading();
    } else if (typeShading == 3) {
	shading = phongShading();
    }

    gl_FragColor = shading;
}




