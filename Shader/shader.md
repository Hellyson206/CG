# Modelos de Iluminação

## Modelo de Luz Ambiente
 
É o modelo inicial e mais simples de iluminação. Representa a luz ambiente refletida nos objetos, em computação gráfica a luz existe apenas matematicamente. O modelo é aplicado em todos os vértices da primitiva individualmente, e durante a rasterização ocorre a interpolação das cores dos fragmentos. O modelo é expresso através da equação:

![ambientF](https://github.com/Shanksir/CG/blob/master/Shader/images/ambientF.png)

Onde **Ia** é a intensidade da luz ambiente e **ka** é o coeficiente de reflectância do objeto, também interpretado como a cor do objeto.

![ambientTeapot](https://github.com/Shanksir/CG/blob/master/Shader/images/ambientTeapot.png)

## Modelo de Luz Difusa

Tomando como base o sistema de Lambertian, onde o objeto reflete a luz recebida perfeitamente em todas as direções, o modelo de luz difusa considera o angulo formado entre o vetor normal do vértice **N** com o vetor **L** que aponta do vértice para o ponto de luz. Dependendo do valor do ângulo, as cores dos vértices da primitiva serão alterados e formarão as cores dos fragmentos através da interpolação durante a rasterização.

O modelo é expresso através da equação:

![diffuseF](https://github.com/Shanksir/CG/blob/master/Shader/images/diffuseF.png)

Onde **Ip** é a intensidade do ponto de luz, o **kd** é o coeficiente de reflectância do objeto e o **cos** é cosseno do angulo formado entre o vetor normal **N** e o vetor **L**. Para uma melhor aproximação da realidade, o modelo de luz difusa é utilizado em conjunto com o modelo de luz ambiente:

![ambientDiffuseF](https://github.com/Shanksir/CG/blob/master/Shader/images/ambientDiffuseF.png)

### Implementação

```C++
vec3 L = normalize(I_p_pos - (model_mat * vec4(obj_spc_vertex_pos, 1.0)).xyz);
vec3 N = normalize(mat3(transpose(inverse(model_mat))) * obj_spc_N);

I = I_a * k_a + I_p * k_d * max(dot(L, N), 0.0f);
```

Foi utilizada a função **max** para que o resultado do cosseno fosse sempre maior ou igual à zero, retornando sempre o maior valor entre o produto interno e zero. Ao executar temos o resultado:

![diffuseTeapot](https://github.com/Shanksir/CG/blob/master/Shader/images/diffuseTeapot.png)

## Modelo de Luz Especular

O modelo visa uma aproximação de brilho especular, levando em consideração o ângulo entre um vetor de reflexão de luz **R** do objeto e um vetor de visão **V**. A intensidade do brilho especular, dependendo do valor do ângulo, é maior ou menor. O modelo é expresso através da equação:

![specularF](https://github.com/Shanksir/CG/blob/master/Shader/images/specularF.png)

Onde o **Ip** é a intensidade do ponto de luz, o **ks** é o coeficiente de reflectância especular e o **cos** é o cosseno do ângulo entre os vetores **R** e **V**. Para uma melhor aproximação da realidade, o modelo de luz especular é utilizado em conjunto com o modelo de luz ambiente e com o modelo de luz difusa:

![ambientDiffuseSpecularF](https://github.com/Shanksir/CG/blob/master/Shader/images/ambientDiffuseSpecularF.png)

### Implementação

Utilizando da mesma propriedade do modelo difuso, a implementação fica:

```C++
vec3 L = normalize(I_p_pos - (model_mat * vec4(obj_spc_vertex_pos, 1.0)).xyz);
vec3 N = normalize(mat3(transpose(inverse(model_mat))) * obj_spc_N);

vec3 R = -reflect (L, N);
vec3 V = normalize(cam_pos - (model_mat * vec4(obj_spc_vertex_pos, 1.0)).xyz);

I = I_a * k_a + I_p * (k_d * max(dot(L, N), 0.0f) + k_s * pow(max(dot(R, V), 0.0f), n));
```

Ao executar temos o resultado:

![specularTeapot](https://github.com/Shanksir/CG/blob/master/Shader/images/specularTeapot.png)

## Referências

- WebGl Phong Shading: http://www.cs.toronto.edu/~jacobson/phong-demo/
- Notas de Aula do Prof. Christian Pagot
