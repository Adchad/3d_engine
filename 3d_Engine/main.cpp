#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <math.h>


struct vec3d
{
  float x, y, z;
};

struct triangle
{
  vec3d p[3];
};

struct mesh
{
  std::vector<triangle> tris;
};

struct mat4x4
{
  float m[4][4]= {0};
};




class engine3D
{
private:
  mesh meshcube;
  mat4x4 matproj;
  float fTheta;

  vec3d vCamera;

  void MultiplyMatrixVector(vec3d &i, vec3d &o, mat4x4 &m)
  {
    o.x = i.x *m.m[0][0] + i.y *m.m[1][0] + i.z *m.m[2][0] + m.m[3][0];
    o.y = i.x *m.m[0][1] + i.y *m.m[1][1] + i.z *m.m[2][1] + m.m[3][1];
    o.z = i.x *m.m[0][2] + i.y *m.m[1][2] + i.z *m.m[2][2] + m.m[3][2];
    float w = i.x *m.m[0][3] + i.y *m.m[1][3] + i.z *m.m[2][3] + m.m[3][3];
    if(w != 0.0f)
    {
      o.x /=w;
      o.y /=w;
      o.z /=w;
    }
  }


public:

  void init()
  {
    meshcube.tris = {
    		// SOUTH
    		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
    		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },
    		// EAST
    		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
    		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },
    		// NORTH
    		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
    		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },
    		// WEST
    		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
    		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },
    		// TOP
    		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
    		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },
    		// BOTTOM
    		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
    		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f }
    };

    //Matrix populate
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRation = 600.0f/800.0f;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.f * 3.14159f);


    matproj.m[0][0] = fAspectRation*fFovRad;
    matproj.m[1][1] = fFovRad;
    matproj.m[2][2] = fFar / (fFar - fNear);
    matproj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matproj.m[2][3] = 1.0f;
    matproj.m[3][3] = 0.0f;
  }


  void update(){
    mat4x4 matRotZ, matRotX;
	  float fElapsedTime=0;


    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    window.clear(sf::Color::Black);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }



        // clear the window with black color
        window.clear(sf::Color::Black);


        fTheta += 0.003f ;

    		// Rotation Z
    		matRotZ.m[0][0] = cosf(fTheta);
    		matRotZ.m[0][1] = sinf(fTheta);
    		matRotZ.m[1][0] = -sinf(fTheta);
    		matRotZ.m[1][1] = cosf(fTheta);
    		matRotZ.m[2][2] = 1;
    		matRotZ.m[3][3] = 1;


    		// Rotation X
    		matRotX.m[0][0] = 1;
    		matRotX.m[1][1] = cosf(fTheta * 0.4f);
    		matRotX.m[1][2] = sinf(fTheta * 0.4f);
    		matRotX.m[2][1] = -sinf(fTheta * 0.4f);
    		matRotX.m[2][2] = cosf(fTheta * 0.4f);
    		matRotX.m[3][3] = 1;

        // draw everything here...
        // window.draw(...);
        for (auto tri : meshcube.tris)
        {
          triangle triProjected,triTranslated,triRotatedZ, triRotatedZX;


          // Rotate in Z-Axis
			    MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
			    MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
			    MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

          // Rotate in X-Axis
          MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
          MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
          MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

          triTranslated=triRotatedZX;
          triTranslated.p[0].z = tri.p[0].z + 3.0f;
          triTranslated.p[1].z = tri.p[1].z + 3.0f;
          triTranslated.p[2].z = tri.p[2].z + 3.0f;


          vec3d normal, line1_normal, line2_normal;

          line1_normal.x = triTranslated.p[1].x - triTranslated.p[0].x;
          line1_normal.y = triTranslated.p[1].y - triTranslated.p[0].y;
          line1_normal.z = triTranslated.p[1].z - triTranslated.p[0].z;

          line2_normal.x = triTranslated.p[2].x - triTranslated.p[0].x;
          line2_normal.y = triTranslated.p[2].y - triTranslated.p[0].y;
          line2_normal.z = triTranslated.p[2].z - triTranslated.p[0].z;

          normal.x = line1_normal.y * line2_normal.z - line1_normal.z * line2_normal.y;
          normal.y = line1_normal.z * line2_normal.x - line1_normal.x * line2_normal.z;
          normal.z = line1_normal.x * line2_normal.y - line1_normal.y * line2_normal.x;

          float l = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
			    normal.x /= l; normal.y /= l; normal.z /= l;

          float dot_product_test = normal.x * (triTranslated.p[0].x - vCamera.x) + normal.y * (triTranslated.p[0].y - vCamera.y) + normal.z * (triTranslated.p[0].z - vCamera.z);

          if(dot_product_test<0)

          {
            MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matproj);
            MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matproj);
            MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matproj);


            sf::VertexArray triangle_shape(sf::Triangles, 3);

            // define the position of the triangle's points
            triangle_shape[0].position = sf::Vector2f(triProjected.p[0].x*400 + 300, triProjected.p[0].y*400 + 300);
            triangle_shape[1].position = sf::Vector2f(triProjected.p[1].x*400 + 300, triProjected.p[1].y*400 + 300);
            triangle_shape[2].position = sf::Vector2f(triProjected.p[2].x*400 + 300, triProjected.p[2].y*400 + 300);

            if(dot_product_test<-3){
              triangle_shape[0].color = sf::Color::White;
              triangle_shape[1].color = sf::Color::White;
              triangle_shape[2].color = sf::Color::White;
            }
            else if(dot_product_test<-2){
              triangle_shape[0].color = sf::Color(169,169,169);
              triangle_shape[1].color = sf::Color(169,169,169);
              triangle_shape[2].color = sf::Color(169,169,169);
            }
            else if(dot_product_test<-1){
              triangle_shape[0].color = sf::Color(105,105,105);
              triangle_shape[1].color = sf::Color(105,105,105);
              triangle_shape[2].color = sf::Color(105,105,105);
            }
            else {
              triangle_shape[0].color = sf::Color(80,80,80);
              triangle_shape[1].color = sf::Color(80,80,80);
              triangle_shape[2].color = sf::Color(80,80,80);
            }

            window.draw(triangle_shape);
          }
         }
        // end the current frame
        window.display();
  }
  }
};


int main()
{

    engine3D demo;
    demo.init();
    demo.update();
    return 0;
}
