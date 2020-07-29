# Pipeline Gráfico

## Pipeline
 
Pipeline gráfico é a sequência de passos que manipulam vértices com o objetivo de criar uma representação 2D de uma cena 3D. É dividido em 6 espaços pelos quais os vértices passam até terem coordenada de tela.

![Pipeline](https://github.com/Shanksir/CG/blob/master/Pipeline/images/Pipeline.png)

No espaço do Objeto estão os vértices do objeto ainda intocados sem terem sofrido qualquer tipo de manipulação. Os objetos são postos todos juntos ao irem para o espaço do Universo. 

![ObjToWorld](https://github.com/Shanksir/CG/blob/master/Pipeline/images/ObjToWorld.png)

Os vértices são levados ao espaço do Universo através da matriz Model que é composta pelas transformações que podem ser escalas, rotações ou translações. Todas as transformações são descritas como matrizes, permitindo que sejam compostas através de uma multiplicação de matrizes.

## Exercício

Importante ressaltar que foi utilizada a biblioteca **GLM** no código template, logo as matrizes de transformações estão na notação vetor coluna.

Compilando o código template disponibilizado, é exibido:

![Template](https://github.com/Shanksir/CG/blob/master/Pipeline/images/Template.png)

### Escala

Para aplicar a escala com fatores **(x, y, z) = (1/3, 3/2, 1)**, foi utilizada a matriz:

![ScaleMatrix](https://github.com/Shanksir/CG/blob/master/Pipeline/images/ScaleMatrix.png)

Igualando a matriz **Model** à matriz de **Escala** para aplicar as transformações nos vértices.

```C++
float scale_array[16] = {1.0f/3.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.5f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f};

glm::mat4 model_mat = glm::make_mat4(scale_array);
```

Ao executar temos o resultado:

![Scale](https://github.com/Shanksir/CG/blob/master/Pipeline/images/Scale.png)

### Translação

Para aplicar a translação com fatores **(x, y, z) = (1, 0, 0)**, foi utilizada a matriz:

![TransMatrix](https://github.com/Shanksir/CG/blob/master/Pipeline/images/TransMatrix.png)

Igualando a matriz **Model** à matriz de **Translação** para aplicar as transformações nos vértices.

```C++
float trans_array[16] = {1.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f,
                         1.0f, 0.0f, 0.0f, 1.0f};

glm::mat4 model_mat = glm::make_mat4(trans_array);
```

Ao executar temos o resultado:

![Trans](https://github.com/Shanksir/CG/blob/master/Pipeline/images/Trans.png)

### Projeção

Aplicando o fator de projeção **d = 0.5** na matriz **Projection**:

![ProjMatrix](https://github.com/Shanksir/CG/blob/master/Pipeline/images/ProjMatrix.png)

Igualando à matriz de **Projeção**:

```C++
float proj_array[16] = {1.0f, 0.0f, 0.0f, 0.0f, 
                        0.0f, 1.0f, 0.0f, 0.0f, 
                        0.0f, 0.0f, 1.0f, 0.0f, 
                        0.0f, 0.0f, 0.0f, 1.0f};

glm::mat4 proj_mat = glm::make_mat4(proj_array);
```

Ao executar temos o resultado:

![Proj](https://github.com/Shanksir/CG/blob/master/Pipeline/images/Proj.png)

### Posição da Câmera

Após os vértices terem sido transformados até o espaço do Universo, o próximo passo é levá-los ao espaço de Câmera. É preciso criar um sistema de coordenadas X Y Z da câmera e realizar uma mudança de base. Utilizando como base o vetor Up da câmera que geralmente é up(0, 1, 0), sua posição e o ponto para onde está apontando, é possível gerar o sistema da câmera.

![WorldToCam](https://github.com/Shanksir/CG/blob/master/Pipeline/images/WorldToCam.png)

O Z da câmera pode ser gerado através de um vetor formado entre a posição da câmera e o ponto para onde ela está apontando. É importante que os vetores sejam normalizados para que tenham tamanho unitário, então também é divido pelo seu comprimento:

**`Zc = (camPos - LookAt)/||camPos - LookAt||`**

Como o produto vetorial entre dois vetores gera um novo vetor que é perpendicular, o X pode ser gerado através de um produto vetorial entre o **Up** e o **Zc**:

**`Xc = (Up x Zc)/||Up x Zc||`**

Seguindo o mesmo raciocínio que na geração do Xc, o Yc também pode ser gerado através de um produto vetorial entre o **Zc** e o **Xc**. Porém não precisa ser normalizado já que o produto vetorial entre dois vetores unitários gera um vetor perpendicular também unitário:

**`Yc = (Zc x Xc)`**

Agora se tem o sistema de coordenadas da câmera, o próximo passo é multiplicar os vértices pela View, matriz que contém rotações e translações e que realiza uma mudança de bases do espaço do Universo para o espaço da Câmera.

![View](https://github.com/Shanksir/CG/blob/master/Pipeline/images/View.png)

Onde U, V e N corresponde respectivamente à Xc, Yc e Zc. Sem a matriz T, a mudança de bases funcionaria apenas para bases que estivessem no mesmo eixo e como é pouco provável que a origem do sistema da câmera coincida com a origem do espaço do Universo, é incluído um vetor T que ligue as duas origens das bases para que a mudança funcione para todos os casos. Logo, o vetor T corresponde ao vetor de posição da câmera.

Fazendo com a posição da câmera em **(-0.1, 0.1, 0.25)** apontando para a origem do sistema do Universo. Código em C++ utilizando a biblioteca [GLM](https://glm.g-truc.net/0.9.9/index.html):

```C++
/* Camera Settings */
    
float look_at_array[3] = {0.0f, 0.0f, 0.0f},
      camPos_array[3]  = {-0.1f, 0.1f, 0.25f},
      camUp_array[3]   = {0.0f, 1.0f, 0.0f};

glm::vec3 Look_At = glm::make_vec3(look_at_array);
glm::vec3 camPos  = glm::make_vec3(camPos_array);
glm::vec3 camUp   = glm::make_vec3(camUp_array);

glm::vec3 camZ = glm::make_vec3(glm::normalize(camPos - Look_At));
glm::vec3 camX = glm::make_vec3(glm::normalize(glm::cross(camUp, camZ)));
glm::vec3 camY = glm::make_vec3(glm::cross(camZ, camX));

float Bt_array[16] = {camX.x, camY.x, camZ.x, 0.0f,
                      camX.y, camY.y, camZ.y, 0.0f,
                      camX.z, camY.z, camZ.z, 0.0f,
                        0.0f,   0.0f,   0.0f, 1.0f};

float T_array[16] = {     1.0f,      0.0f,      0.0f, 0.0f,
                          0.0f,      1.0f,      0.0f, 0.0f,
                          0.0f,      0.0f,      1.0f, 0.0f,
                         -camPos.x, -camPos.y, -camPos.z, 1.0f};

glm::mat4 view_mat = glm::make_mat4(Bt_array) * glm::make_mat4(T_array);
```

*PS: É importante ressaltar que nesse pipeline será levado em consideração a projeção perspectiva, garantindo que o que tiver mais longe da câmera seja renderizado menor e o que está perto será renderizado maior.* 

Ao executar temos o resultado:

![CamPos](https://github.com/Shanksir/CG/blob/master/Pipeline/images/CamPos.png)

### Transformações Livres

Neste exercício, foram adicionados dois triângulos:

```C++
float vertices[] = {-0.25f, -0.5f, -0.1f, 0.75f, 0.0f, 0.0f, // red triangle (closer)
                     0.25f,  0.5f, -0.1f, 0.75f, 0.0f, 0.0f,
                     0.75f, -0.5f, -0.1f, 0.75f, 0.0f, 0.0f,
                    -0.75f, -0.5f, -0.4f, 0.0f, 0.0f, 0.75f, // blue triangle (farther)
                    -0.25f,  0.5f, -0.4f, 0.0f, 0.0f, 0.75f,
                     0.25f, -0.5f, -0.4f, 0.0f, 0.0f, 0.75f,
                     0.25f, -0.5f, -0.8f, 0.0f, 0.75f, 0.0f, // green triangle
                     0.75f,  0.5f, -0.8f, 0.0f, 0.75f, 0.0f,
                     1.25f, -0.5f, -0.8f, 0.0f, 0.75f, 0.0f,
                    -1.75f, -0.5f, -0.6f, 0.75f, 0.57f, 0.0f,// yellow triangle
                    -1.25f,  0.5f, -0.6f, 0.75f, 0.57f, 0.0f,
                    -0.75f, -0.5f, -0.6f, 0.75f, 0.57f, 0.0f};
```
As transformações utilizadas foram:

- Escala em **(0.5, 0.5, 0.5)**
- Rotação de **180°** em torno do eixo **Z**
- Translação em **(-0.25, 0.5, 0)** 
- Rotação de **30°** em torno do eixo **Y**

```C++
float scale_array[16] = {0.5f, 0.0f, 0.0f, 0.0f,
                         0.0f, 0.5f, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.5f, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f};

float trans_array[16] = {1.0f,  0.0f, 0.0f,  0.0f,
                         0.0f,  1.0f, 0.0f,  0.0f,
                         0.0f,  0.0f, 1.0f,  0.0f,
                       -0.25f,  0.5f, 0.0f,  1.0f};

// Rotation about Z axis
float zangle = glm::pi<float>();
float rotz_array[16] = {glm::cos(zangle), glm::sin(zangle), 0.0f, 0.0f,
                       -glm::sin(zangle), glm::cos(zangle), 0.0f, 0.0f,
                                    0.0f,             0.0f, 1.0f, 0.0f,
                                    0.0f,             0.0f, 0.0f, 1.0f};

  // Rotation about Y axis
float yangle = glm::pi<float>()/6;
float roty_array[16] = {glm::cos(yangle), 0.0f, -glm::sin(yangle), 0.0f,
                                    0.0f, 1.0f,              0.0f, 0.0f,
                       -glm::sin(yangle), 0.0f,  glm::cos(yangle), 0.0f,
                                    0.0f, 0.0f,              0.0f, 1.0f};

glm::mat4 model_mat = glm::make_mat4(roty_array) * glm::make_mat4(trans_array) * glm::make_mat4(rotz_array) * glm::make_mat4(scale_array);
```
A câmera foi posicionada em **(0.05, 0.05, 0.2)** com fator de projeção **d = 0.8**.

Ao executar temos o resultado:

![FreeTrans](https://github.com/Shanksir/CG/blob/master/Pipeline/images/FreeTrans.png)

## Referências

- GLM API Documentation: https://glm.g-truc.net/0.9.9/api/index.html
- Notas de Aula do Prof. Christian Pagot
