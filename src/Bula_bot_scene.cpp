//Always read docs.gl
//The comand "GL_Check_Error(__LINE__);" is used to check for error and it has to be placed in the line following the error you are trying to identify. Remeber that the errors are cumulative and it shows all the errors before the line  you are calling it.

//This goes to Mirro box not working

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
//#include <chrono>
#include <thread>
#include <vector>

#include "Bula_opengl.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "Bula_bot_parts.h"

static void GL_Check_Error(int line) { //This function has to be placed in the line below the one that it is being checked
    //Care must be taken because if it has errors before the current function being analysed it will show a random error untill all of them are solved
    while (GLenum error = glGetError()) {
        std::cout << "An error has been found in line " << line -1 << " or before: \n" << "Error in open GL(" << error << ")\n" << "Press enter to continue!" << std::endl;
        std::cin.get();
    }
    //The error show will appear in decimal to check for conversion it is needed to convert to hexadecimal to check in glew header file
}
float scrol_xoffset_global = 0;
float scrol_yoffset_global = 0;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    scrol_xoffset_global = xoffset;
    scrol_yoffset_global  = yoffset;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    int resolution = 700.0f;
    int window_width, window_height;
    window = glfwCreateWindow(resolution, resolution, "Bula Bot Scene", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //Used for synchronization
    if (glewInit() == GLEW_OK) std::cout << glGetString(GL_VERSION) << std::endl;

    float max_box_point = 50.0f; // Chosen max point for the image box
    float Image_resolution_x = 1139.0f;
    float Image_resolution_y = 577.0f;
    float Image_resolution = Image_resolution_x / Image_resolution_y; //Resolution of imported image
    float cam_x_translate = 25.0f;
    float cam_y_translate = 0.0f;
    float window_zoom = 150.0f;

    float positions[]{ //Data to be shown (pictures are 1139x577) *(577.0f/1139.0f)
        -max_box_point * (Image_resolution), -max_box_point, 0.0f, 0.0f,
         max_box_point * (Image_resolution), -max_box_point, 1.0f, 0.0f,
         max_box_point * (Image_resolution),  max_box_point, 1.0f, 1.0f,
        -max_box_point * (Image_resolution),  max_box_point, 0.0f, 1.0f
    };
    unsigned int indices[]{//Indices for the data above be shown
        0,1,2,
        2,3,0
    };

    float points_to_print[20];
    float robot_init_angle_m1 = (2.35f), robot_init_angle_m2 = (3.35f);
    float ground_angle = 0.0f;
    //Declarations to set up the robot
    Robot_arm test_arm;
    Robot_arm* selected_arm = &test_arm;
    std::cout << "Robots : " << &test_arm << " Robots : " << selected_arm << std::endl;
    float increment_pace = 0.01f;
    float increment_pace_angle = 3.1415926536f / 180.0f;
    float increment_height = 20.0f;
    float Initial_robot_x = 35.0f, Initial_robot_y = 25.763f + increment_height;
    float mouse_click_x = 0, mouse_click_y = 0;
    test_arm.set_robot_arm_points(robot_init_angle_m1, robot_init_angle_m2, false);
    test_arm.set_robot_init_position(Initial_robot_x, Initial_robot_y);
    test_arm.set_robot_origin(Initial_robot_x * 2.38f, (Initial_robot_y + 0.237f) * 2.38f); //Check if 0.237 is necessary *******************************
    test_arm.update_arm_points(points_to_print);
    float upward_angle_increment = 0.0f, forward_angle_increment = 0.0f;

    float P1a = points_to_print[0], P1b = points_to_print[1];
    float P2a = points_to_print[2], P2b = points_to_print[3];
    float P3a = points_to_print[4], P3b = points_to_print[5];
    float P4a = points_to_print[6], P4b = points_to_print[7];
    float P5a = points_to_print[8], P5b = points_to_print[9];
    float P6a = points_to_print[10], P6b = points_to_print[11];
    float P7a = points_to_print[12], P7b = points_to_print[13];
    float P8a = points_to_print[14], P8b = points_to_print[15];
    float P9a = points_to_print[16], P9b = points_to_print[17];
    float P10a = points_to_print[18], P10b = points_to_print[19];

    float le = 35, lc = 80, la = 80, lf = 35, li = 80, ld = 40, lh = 20, lg = 46.25, lb = 40, lj = 20, lk = 45.2355;// vertical drive arm, link, , forward drive arm, left piece of horizontal arm, right piece of horizontal arm, left part of triangular link, right part of triangular link, upper part of triangular link, link attached to base, grid attachment link, link to point of griping

    //Declaration to set up the cube
    float cube_initial_position, cube_size, cube_angle;
    cube_initial_position = 230.0f;
    cube_size = 12.0f * 2.38;
    cube_angle = 0.0;
    Box cube1(cube_initial_position, cube_size, cube_size, cube_angle);
    float update_x_cube = 0.0f, update_y_cube = 0.0f, update_angle_cube = 0.0f;
    float cube_scale = cube_size / 30.0f; //The image was generated with a cube of 30

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray va;
    VertexBuffer vb(positions, sizeof(positions));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int));

    glm::mat4 proj = glm::ortho(0.0f, window_zoom, 0.0f, window_zoom, -1.0f, 1.0f); //Contracts or stretches the object
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(cam_x_translate, cam_y_translate, 0.0f)); //Works like a camera
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); //Works moving the object
    glm::mat4 mvp = proj * view * model;

    glm::mat4 proj_cube = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f); //Contracts or stretches the object


    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    GLfloat set_colors[4] = { 0.2f, 0.6f, 1.0f, 1.0f }; //Variable used for coloring
    shader.setUniform4f("u_color", set_colors[0], set_colors[1], set_colors[2], set_colors[3]);
    shader.setUniformMat4f("u_MVP", mvp);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    Renderer renderer;

    //Loading images to textures
    Texture texture1("res/textures/Base.png");
    Texture texture2("res/textures/BaseLid.png");
    Texture texture3("res/textures/ServoHolder.png");
    Texture texture4("res/textures/LinkM1.png");
    Texture texture5("res/textures/LinkM2.png");
    Texture texture6("res/textures/LinkCommom.png");
    Texture texture7("res/textures/LongLink.png");
    Texture texture8("res/textures/LinkThreePoints.png");
    Texture texture9("res/textures/GripOpened.png");
    Texture texture10("res/textures/GripClosed.png");
    Texture texture11("res/textures/cube.png");
    Texture texture12("res/textures/BaseB.png");
    Texture texture13("res/textures/BaseLidB.png");
    Texture texture14("res/textures/ServoHolderB.png");
    Texture texture15("res/textures/LinkM1B.png");
    Texture texture16("res/textures/LinkM2B.png");
    Texture texture17("res/textures/LinkCommomB.png");
    Texture texture18("res/textures/LongLinkB.png");
    Texture texture19("res/textures/LinkThreePointsB.png");
    Texture texture20("res/textures/GripOpenedB.png");
    Texture texture21("res/textures/GripClosedB.png");
    Texture texture22("res/textures/cubeB.png");
    Texture texture23("res/textures/Cherno.png");
    
    texture1.Bind();
    shader.setUniform1i("u_texture", 0); // The second number needs to be the same as the "texture.Bind"
    //GL_Check_Error(__LINE__);

    float increment = 0.05f;
    
    //This creates a window for ImGUI
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    //Enables input of mouse and keyboard for current window
    Input input(window);

    glfwSetScrollCallback(window, scroll_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        ImGui_ImplGlfwGL3_NewFrame();

        //This creates a window for ImGUI

        static float angle_test = 0.0f, x_test = 0.0f, y_test = 0.0f;
        static float angle_test2 = 0.0f, x_test2 = 0.0f, y_test2 = 0.0f;
        static float angle_test3 = 0.0f, x_test3 = 0.0f, y_test3 = 0.0f;
        static float temp_angle_test1 = 0.0f, temp_angle_test2 = 0.0f;
        if (temp_angle_test1 != angle_test || temp_angle_test2 != angle_test2) {
            test_arm.set_robot_arm_points(forward_angle_increment + angle_test + robot_init_angle_m1, upward_angle_increment + angle_test2 + robot_init_angle_m2, false);
            temp_angle_test1 = angle_test;
            temp_angle_test2 = angle_test2;
        }
        static int counter = 0;
        ImGui::Text("Hello, world!");                                                            // Display some text (you can use a format string too)
        ImGui::SliderFloat("M1_test", &angle_test, -3.14159265359f, 3.14159265359f);             // Edit 1 float using a slider from 0.0f to 1.0f(these last two are the bounds)    
        ImGui::SliderFloat("M2_test", &angle_test2, -3.14159265359f, 3.14159265359f);            // Edit 1 float using a slider from 0.0f to 1.0f(these last two are the bounds)    
        ImGui::SliderFloat("Ground angle", &angle_test3, -3.14159265359f, 3.14159265359f);            // Edit 1 float using a slider from 0.0f to 1.0f(these last two are the bounds)    

        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


        //testing mouse position
        if (input.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
            if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                float angle_m1, angle_m2;
                auto [mouse_x, mouse_y] = input.GetMousePosition();
                glfwGetWindowSize(window, &window_width, &window_height);
                mouse_click_x = (mouse_x * window_zoom / resolution - cam_x_translate) * 2.38f;
                mouse_click_y = ((window_height - mouse_y) * window_zoom / resolution - cam_y_translate) * 2.38f;
                test_arm.search_point_robot_arm(test_arm, mouse_click_x, mouse_click_y, 0.1);
                angle_m1 = test_arm.robot_arm_get_searched_angle('1');
                if (angle_m1 != 0.0f) {
                    robot_init_angle_m1 = angle_m1;
                    //Clean increments
                    forward_angle_increment = 0.0f;
                    upward_angle_increment = 0.0f;
                    angle_test = 0.0f;
                    angle_test2 = 0.0f;
                    temp_angle_test1 = 0.0f;
                    temp_angle_test2 = 0.0f;
                }
                angle_m2 = test_arm.robot_arm_get_searched_angle('2');
                if (angle_m2 != 0.0f) {
                    robot_init_angle_m2 = angle_m2;
                    //Clean increments
                    forward_angle_increment = 0.0f;
                    upward_angle_increment = 0.0f;
                    angle_test = 0.0f;
                    angle_test2 = 0.0f;
                    temp_angle_test1 = 0.0f;
                    temp_angle_test2 = 0.0f;
                }
                if (angle_m1 == 0 && angle_m2 == 0) {
                    std::cout << "This point is not achievable by the current robot configuration. " << std::endl;
                }
                else {
                    test_arm.mov_robot_arm_by_servos(test_arm.robot_arm_get_searched_angle('1') - test_arm.robot_arm_get_angle('1'), test_arm.robot_arm_get_searched_angle('2') - test_arm.robot_arm_get_angle('2'), false);
                    test_arm.reset_robot_arm_searched_angle();
                }  
            }
        }
   
        //Move window with right ctrl button 
        if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            cam_x_translate += 5 * scrol_yoffset_global;
            view = glm::translate(glm::mat4(1.0f), glm::vec3(cam_x_translate, cam_y_translate, 0.0f)); //Works like a camera
            scrol_yoffset_global = 0;
        }  

        //Adapting zoom with mouse middle button
        if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
            window_zoom += 5*scrol_yoffset_global;
            proj = glm::ortho(0.0f, window_zoom, 0.0f, window_zoom, -1.0f, 1.0f); //Contracts or stretches the object
            scrol_yoffset_global = 0;
        }

        //Hold shift and arrows to move robot
        if (input.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
            if (input.IsKeyPressed(GLFW_KEY_RIGHT)) {
                if (test_arm.mirror_status()) {
                    test_arm.mov_robot_arm_by_servos(increment_pace, 0.0f, false);
                }
                else {
                    test_arm.mov_robot_arm_by_servos(-increment_pace, 0.0f, false);
                }

            }
            if (input.IsKeyPressed(GLFW_KEY_LEFT)) {
                if (test_arm.mirror_status()) {
                    test_arm.mov_robot_arm_by_servos(-increment_pace, 0.0f, false);
                }
                else {
                    test_arm.mov_robot_arm_by_servos(increment_pace, 0.0f, false);
                }

            }
            if (input.IsKeyPressed(GLFW_KEY_DOWN)) {
                test_arm.mov_robot_arm_by_servos(0.0f, -increment_pace, false);
            }
            if (input.IsKeyPressed(GLFW_KEY_UP)) {
                test_arm.mov_robot_arm_by_servos(0.0f, increment_pace, false);
            }
        }

        // press Ctrl LEFT to change grip status
        if (input.IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            test_arm.change_grip_status();
        }

        // press ALT RIGHT to control ground angle
        if (input.IsKeyPressed(GLFW_KEY_RIGHT_ALT)) {
            //Routine to incline the robot
            if (input.IsKeyPressed(GLFW_KEY_RIGHT)) {
                if (test_arm.mirror_status()) {
                    ground_angle = ground_angle - increment_pace_angle;                    
                    test_arm.set_robot_angle(-ground_angle, - increment_pace_angle, increment_height);
                }
                else {
                    ground_angle = ground_angle - increment_pace_angle;
                    test_arm.set_robot_angle(-ground_angle, - increment_pace_angle, increment_height);
                }

            }
            if (input.IsKeyPressed(GLFW_KEY_LEFT)) {
                if (test_arm.mirror_status()) {
                    ground_angle = ground_angle + increment_pace_angle;
                    test_arm.set_robot_angle(-ground_angle, increment_pace_angle, increment_height);
                }
                else {
                    ground_angle = ground_angle + increment_pace_angle;
                    test_arm.set_robot_angle(-ground_angle, increment_pace_angle, increment_height);
                }

            }
        }

        // press shift right to check debug status
        if (input.IsKeyPressed(GLFW_KEY_RIGHT_SHIFT)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            test_arm.print_arm_points();
            cube1.print_cube_coordinates();
            std::cout << "Cube angle: " << cube1.box_get_angle() << std::endl;
        }

        // press TAB to change current robot //*********************************************** TO DO LIST, make the selected robot blue
        if (input.IsKeyPressed(GLFW_KEY_TAB)) {
            //Create list of adress for robot arm
            //Create variable to move the specific robot and change its colors
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        // press ALT left to change mirror status 
        if (input.IsKeyPressed(GLFW_KEY_LEFT_ALT)) {
            test_arm.change_robot_mirror_status();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        //*********************************************************************************************************

        //Update points to be printed
        test_arm.update_arm_points(points_to_print);
        P1a = points_to_print[0], P1b = points_to_print[1];
        P2a = points_to_print[2], P2b = points_to_print[3];
        P3a = points_to_print[4], P3b = points_to_print[5];
        P4a = points_to_print[6], P4b = points_to_print[7];
        P5a = points_to_print[8], P5b = points_to_print[9];
        P6a = points_to_print[10], P6b = points_to_print[11];
        P7a = points_to_print[12], P7b = points_to_print[13];
        P8a = points_to_print[14], P8b = points_to_print[15];
        P9a = points_to_print[16], P9b = points_to_print[17];
        P10a = points_to_print[18], P10b = points_to_print[19];

        //The following lines display the images
        if (!test_arm.mirror_status()) {    
            //Base
            texture1.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(test_arm.get_initial_robot_position('x'), (test_arm.get_initial_robot_position('y') - 25.763f), 0.0f)); //Works moving the object
            model = glm::rotate(model, (float)(ground_angle), glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = proj * view * model;                                                //Update object position before sending it to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions to the shader
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Forward arm (image seem to be reversed so an angle of 0.032 was added**********)
            texture4.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(test_arm.get_origin_position('x') / 2.38f, test_arm.get_origin_position('y') / 2.38f, 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(0.032 + atan2((P3b - P1b), (P3a - P1a))), glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Upward arm
            texture5.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(test_arm.get_origin_position('x') / 2.38f, test_arm.get_origin_position('y') / 2.38f, 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(atan2((P5b - P1b), (P5a - P1a))), glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Link for upward arm
            texture6.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3((test_arm.get_origin_position('x') / 2.38f + ((le / 2.38) * cos(atan2((P5b - P1b), (P5a - P1a))))), (test_arm.get_origin_position('y') / 2.38f + ((le / 2.38) * sin(atan2((P5b - P1b), (P5a - P1a))))), 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(atan2((P6b - P5b), (P6a - P5a))), glm::vec3(0.0f, 0.0f, 1.0f)); 
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Long link
            texture7.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3((test_arm.get_origin_position('x') / 2.38f + ((la / 2.38) * cos(atan2((P3b - P1b), (P3a - P1a))))), (test_arm.get_origin_position('y') / 2.38f + ((la / 2.38) * sin(atan2((P3b - P1b), (P3a - P1a))))), 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(atan2((P8b - P6b), (P8a - P6a))), glm::vec3(0.0f, 0.0f, 1.0f)); 
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Link for triangle
            texture6.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3((test_arm.get_origin_position('x') / 2.38f + ((lb / 2.38) * cos(atan2((P2b - P1b), (P2a - P1a))))), (test_arm.get_origin_position('y') / 2.38f + ((lb / 2.38) * sin(atan2((P2b - P1b), (P2a - P1a))))), 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(atan2((P6b - P5b), (P6a - P5a))), glm::vec3(0.0f, 0.0f, 1.0f)); 
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Triangular link
            texture8.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3((test_arm.get_origin_position('x') / 2.38f + ((la / 2.38) * cos(atan2((P3b - P1b), (P3a - P1a))))), (test_arm.get_origin_position('y') / 2.38f + ((la / 2.38) * sin(atan2((P3b - P1b), (P3a - P1a))))), 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(atan2((P7b - P4b), (P7a - P4a))), glm::vec3(0.0f, 0.0f, 1.0f)); 
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Link for grip
            texture6.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(((test_arm.get_origin_position('x') / 2.38f + ((la / 2.38) * cos(atan2((P3b - P1b), (P3a - P1a))))) + ((lh / 2.38) * cos(atan2((P7b - P3b), (P7a - P3a))))), ((test_arm.get_origin_position('y') / 2.38f + ((la / 2.38) * sin(atan2((P3b - P1b), (P3a - P1a))))) + ((lh / 2.38) * sin(atan2((P7b - P3b), (P7a - P3a))))), 0.0f));  //Works moving the object for the second object
            model = glm::rotate(model, (float)(atan2((P9b - P7b), (P9a - P7a))), glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Cube
            texture11.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            update_angle_cube = cube1.box_get_angle();
            update_x_cube = cube1.box_get_point('a')->get_point_x();
            update_y_cube = cube1.box_get_point('a')->get_point_y();      
            model = glm::translate(glm::mat4(1.0f), glm::vec3((update_x_cube / 2.38f) + (cube_size / (2.0f * 2.38f)) * cos(update_angle_cube) - (cube_size / (2.0f * 2.38f)) * sin(update_angle_cube), (update_y_cube / 2.38f) + (cube_size / (2.0f * 2.38f)) * sin(update_angle_cube) + (cube_size / (2.0f * 2.38f)) * cos(update_angle_cube), 0.0f));    //Works moving the object for the second object
            model = glm::rotate(model, (float)(update_angle_cube), glm::vec3(0.0f, 0.0f, 1.0f));
            proj_cube = glm::ortho(-1.0f / cube_scale, 1.0f / cube_scale, -1.0f / cube_scale, 1.0f / cube_scale, -1.0f, 1.0f);            //Contracts or stretches the object
            mvp = proj * view * model * proj_cube;                                    //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions    

            //Servo holder
            texture3.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(test_arm.get_initial_robot_position('x') - (test_arm.get_initial_robot_position('y')-increment_height + 0.237f) * sin(ground_angle), (test_arm.get_initial_robot_position('y') - increment_height + 0.237) * cos(ground_angle) + increment_height, 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(ground_angle), glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Base lid
            texture2.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(test_arm.get_initial_robot_position('x') - (test_arm.get_initial_robot_position('y') - increment_height - 13.763f) * sin(ground_angle), (test_arm.get_initial_robot_position('y') - increment_height - 13.763f) * cos(ground_angle) + increment_height, 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(ground_angle), glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            if (!test_arm.grip_status()) {
                //Grip (resolution is a bit different 1126/1139)
                texture10.Bind();
                shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
                model = glm::translate(glm::mat4(1.0f), glm::vec3(((test_arm.get_origin_position('x') / 2.38f + ((la / 2.38) * cos(atan2((P3b - P1b), (P3a - P1a))))) + ((li / 2.38) * cos(atan2((P8b - P3b), (P8a - P3a))))) * (1126.0 / 1139.0), ((test_arm.get_origin_position('y') / 2.38f + ((la / 2.38) * sin(atan2((P3b - P1b), (P3a - P1a))))) + ((li / 2.38) * sin(atan2((P8b - P3b), (P8a - P3a))))), 0.0f));   //Works moving the object for the second object
                model = glm::rotate(model, (float)(atan2((P9b - P8b), (P9a - P8a)) - 1.0385), glm::vec3(0.0f, 0.0f, 1.0f));
                mvp = proj * view * model;                                                //Update object positions to the shader
                shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
                renderer.Draw(va, ib, shader);                                            //Draw new positions
            }
            else {
                //Grip (resolution is a bit different 1126/1139)
                texture9.Bind();
                shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
                model = glm::translate(glm::mat4(1.0f), glm::vec3(((test_arm.get_origin_position('x') / 2.38f + ((la / 2.38) * cos(atan2((P3b - P1b), (P3a - P1a))))) + ((li / 2.38) * cos(atan2((P8b - P3b), (P8a - P3a))))) * (1126.0 / 1139.0), ((test_arm.get_origin_position('y') / 2.38f + ((la / 2.38) * sin(atan2((P3b - P1b), (P3a - P1a))))) + ((li / 2.38) * sin(atan2((P8b - P3b), (P8a - P3a))))), 0.0f));   //Works moving the object for the second object
                model = glm::rotate(model, (float)(atan2((P9b - P8b), (P9a - P8a)) - 1.0385), glm::vec3(0.0f, 0.0f, 1.0f));
                mvp = proj * view * model;                                                //Update object positions to the shader
                shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
                renderer.Draw(va, ib, shader);                                            //Draw new positions
            }
        }        
        if (test_arm.mirror_status()) {
            //Base
            texture1.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(test_arm.get_initial_robot_position('x'), (test_arm.get_initial_robot_position('y') - 25.763f), 0.0f)); //Works moving the object
            model = glm::rotate(model, (float)(ground_angle), glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = proj * view * model;                                                //Update object position before sending it to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions to the shader
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Forward arm (image seem to be reversed so an angle of 0.032 was added**********)
            texture4.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(test_arm.get_origin_position('x') / 2.38f, test_arm.get_origin_position('y') / 2.38f, 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(0.032 + atan2((P3b - P1b), (P3a - P1a))), glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Upward arm
            texture5.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(test_arm.get_origin_position('x') / 2.38f, test_arm.get_origin_position('y') / 2.38f, 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(atan2((P5b - P1b), (P5a - P1a))), glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Link for upward arm
            texture6.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3((test_arm.get_origin_position('x') / 2.38f + ((le / 2.38) * cos(atan2((P5b - P1b), (P5a - P1a))))), (test_arm.get_origin_position('y') / 2.38f + ((le / 2.38) * sin(atan2((P5b - P1b), (P5a - P1a))))), 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(atan2((P6b - P5b), (P6a - P5a))), glm::vec3(0.0f, 0.0f, 1.0f)); 
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Long link
            texture7.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3((test_arm.get_origin_position('x') / 2.38f + ((la / 2.38) * cos(atan2((P3b - P1b), (P3a - P1a))))), (test_arm.get_origin_position('y') / 2.38f + ((la / 2.38) * sin(atan2((P3b - P1b), (P3a - P1a))))), 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(3.1415926536f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, (float)(-atan2((P8b - P6b), (P8a - P6a))), glm::vec3(0.0f, 0.0f, 1.0f)); 
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Link for triangle
            texture6.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(((test_arm.get_origin_position('x') / 2.38f) + ((lb / 2.38) * cos(atan2((P2b - P1b), (P2a - P1a))))), ((test_arm.get_origin_position('y') / 2.38f) + ((lb / 2.38) * sin(atan2((P2b - P1b), (P2a - P1a))))), 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(atan2((P6b - P5b), (P6a - P5a))), glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Link for grip
            texture6.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(((test_arm.get_origin_position('x') / 2.38f + ((la / 2.38) * cos(atan2((P3b - P1b), (P3a - P1a))))) + ((lh / 2.38) * cos(atan2((P7b - P3b), (P7a - P3a))))), ((test_arm.get_origin_position('y') / 2.38f + ((la / 2.38) * sin(atan2((P3b - P1b), (P3a - P1a))))) + ((lh / 2.38) * sin(atan2((P7b - P3b), (P7a - P3a))))), 0.0f));  //Works moving the object for the second object
            model = glm::rotate(model, (float)(atan2((P9b - P7b), (P9a - P7a))), glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Triangular link
            texture8.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3((test_arm.get_origin_position('x') / 2.38f + ((la / 2.38) * cos(atan2((P3b - P1b), (P3a - P1a))))), (test_arm.get_origin_position('y') / 2.38f + ((la / 2.38) * sin(atan2((P3b - P1b), (P3a - P1a))))), 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(3.1415926536f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, (float)(atan2((P7b - P4b), (P7a - P4a))), glm::vec3(0.0f, 0.0f, -1.0f)); 
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Cube
            texture11.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            update_angle_cube = cube1.box_get_angle();
            update_x_cube = cube1.box_get_point('a')->get_point_x();
            update_y_cube = cube1.box_get_point('a')->get_point_y();
            model = glm::translate(glm::mat4(1.0f), glm::vec3((update_x_cube / 2.38f) + (cube_size / (2.0f * 2.38f))*cos(update_angle_cube) - (cube_size / (2.0f * 2.38f)) * sin(update_angle_cube), (update_y_cube / 2.38f) + (cube_size / (2.0f * 2.38f)) * sin(update_angle_cube) + (cube_size / (2.0f * 2.38f)) * cos(update_angle_cube), 0.0f));    //Works moving the object for the second object
            model = glm::rotate(model, (float)(update_angle_cube), glm::vec3(0.0f, 0.0f, 1.0f));
            proj_cube = glm::ortho(-1.0f / cube_scale, 1.0f / cube_scale, -1.0f / cube_scale, 1.0f / cube_scale, -1.0f, 1.0f);            //Contracts or stretches the object
            mvp = proj * view * model * proj_cube;                                    //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions 

            //Servo holder
            texture3.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(test_arm.get_initial_robot_position('x') - (test_arm.get_initial_robot_position('y') - increment_height) * sin(ground_angle), (test_arm.get_initial_robot_position('y') - increment_height) * cos(ground_angle) + increment_height + 0.237f, 0.0f)); //Works moving the object for the second object
            // model = glm::translate(glm::mat4(1.0f), glm::vec3(test_arm.get_initial_robot_position('x') - (test_arm.get_initial_robot_position('y') - increment_height + 0.237f) * sin(ground_angle), (test_arm.get_initial_robot_position('y') - increment_height + 0.237) * cos(ground_angle) + increment_height, 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(3.1415926536f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, (float)(ground_angle), glm::vec3(0.0f, 0.0f, -1.0f));
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            //Base lid
            texture2.Bind();
            shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
            model = glm::translate(glm::mat4(1.0f), glm::vec3(test_arm.get_initial_robot_position('x') - (test_arm.get_initial_robot_position('y') - increment_height - 13.763f) * sin(ground_angle), (test_arm.get_initial_robot_position('y') - increment_height - 13.763f) * cos(ground_angle) + increment_height, 0.0f)); //Works moving the object for the second object
            model = glm::rotate(model, (float)(ground_angle), glm::vec3(0.0f, 0.0f, 1.0f));
            mvp = proj * view * model;                                                //Update object positions to the shader
            shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
            renderer.Draw(va, ib, shader);                                            //Draw new positions

            
           if (!test_arm.grip_status()) {
                //Grip (resolution is a bit different 1126/1139)
                texture10.Bind();
                shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
                model = glm::translate(glm::mat4(1.0f), glm::vec3(((test_arm.get_origin_position('x') / 2.38f + ((la / 2.38) * cos(atan2((P3b - P1b), (P3a - P1a))))) + ((li / 2.38) * cos(atan2((P8b - P3b), (P8a - P3a))))) * (1126.0 / 1139.0), ((test_arm.get_origin_position('y') / 2.38f + ((la / 2.38) * sin(atan2((P3b - P1b), (P3a - P1a))))) + ((li / 2.38) * sin(atan2((P8b - P3b), (P8a - P3a))))), 0.0f));   //Works moving the object for the second object
                model = glm::rotate(model, (float)(3.1415926536f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, (float)(-atan2((P9b - P8b), (P9a - P8a)) - 1.0385), glm::vec3(0.0f, 0.0f, 1.0f));
                mvp = proj * view * model;                                                //Update object positions to the shader
                shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
                renderer.Draw(va, ib, shader);                                            //Draw new positions
            }
            else {
                //Grip (resolution is a bit different 1126/1139)
                texture9.Bind();
                shader.setUniform1i("u_texture", 0);                                      // The second number needs to be the same as the "texture.Bind"
                model = glm::translate(glm::mat4(1.0f), glm::vec3(((test_arm.get_origin_position('x') / 2.38f + ((la / 2.38) * cos(atan2((P3b - P1b), (P3a - P1a))))) + ((li / 2.38) * cos(atan2((P8b - P3b), (P8a - P3a))))) * (1126.0 / 1139.0), ((test_arm.get_origin_position('y') / 2.38f + ((la / 2.38) * sin(atan2((P3b - P1b), (P3a - P1a))))) + ((li / 2.38) * sin(atan2((P8b - P3b), (P8a - P3a))))), 0.0f));   //Works moving the object for the second object
                model = glm::rotate(model, (float)(3.1415926536f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, (float)(-atan2((P9b - P8b), (P9a - P8a)) - 1.0385), glm::vec3(0.0f, 0.0f, 1.0f));
                mvp = proj * view * model;                                                //Update object positions to the shader
                shader.setUniformMat4f("u_MVP", mvp);                                     //Send new positions 
                renderer.Draw(va, ib, shader);                                            //Draw new positions
            }
        }
        
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

//Old Main

/*
void main() {
    {
        Timer timer;
        Box cube1(97, 5);
        std::cout << "The adress of cube1 is : " << Box::Box_Get_Adress(2, 0)[0] << std::endl;
        Robot_arm test_arm;
        test_arm.print_arm_points();
        test_arm.change_grip_status();
        test_arm.print_arm_points();
        test_arm.mov_robot_arm_by_servos(-60 * 3.14159265358979323846 / 180, -94 * 3.14159265358979323846 / 180);
        test_arm.print_arm_points();
        cube1.print_cube_coordinates();
        test_arm.change_grip_status();
        test_arm.print_arm_points();
        test_arm.mov_robot_arm_by_servos(60 * 3.14159265358979323846 / 180, 94 * 3.14159265358979323846 / 180);
        test_arm.print_arm_points();
        cube1.print_cube_coordinates();
        test_arm.mov_robot_arm_by_servos(-30 * 3.14159265 / 180, 0 * 3.14159265 / 180);
        test_arm.print_arm_points();
        cube1.print_cube_coordinates();
        test_arm.change_grip_status();
        test_arm.print_arm_points();
        cube1.print_cube_coordinates();
        std::cout << "The x coordinate of the first box point is : " << Box::Box_Get_Adress(2, 0)[0]->box_get_point('a')->get_point_x() << std::endl;
        /*
        Car test_car(0, 0, 100, 10, 5);
        test_car.print_cube_coordinates();
        test_arm.search_point_robot_arm(test_arm, 39.7559, 86.8279, 0.0001);
        std::cout << "The searched angle for M1 is : " << test_arm.robot_arm_get_searched_angle('1') * (float)(180 / 3.14159265) << ". The searched angle for M2 is: " << test_arm.robot_arm_get_searched_angle('2') * (float)(180 / 3.14159265) << std::endl;
        std::cout << "Test servo search ********************************** " << std::endl;
        test_arm.search_servo_point_robot_arm(test_arm, 39.7559, 86.8279, 0.1, 5.2735 * 3.14159265 / 180);
        std::cout << "The searched angle for M1 is : " << test_arm.robot_arm_get_searched_angle('1') * (float)(180 / 3.14159265) << ". The searched angle for M2 is: " << test_arm.robot_arm_get_searched_angle('2') * (float)(180 / 3.14159265) << std::endl;
        test_arm.change_robot_mirror_status();
        test_arm.print_arm_points();
        test_arm.search_point_robot_arm(test_arm, -39.7559, 86.8279, 0.000001);
        std::cout << "The searched angle for M1 is : " << test_arm.robot_arm_get_searched_angle('1') * (float)(180 / 3.14159265) << ". The searched angle for M2 is: " << test_arm.robot_arm_get_searched_angle('2') * (float)(180 / 3.14159265) << std::endl;
        test_arm.set_robot_angle(25.5 * 3.14159265 / 180.0);
        test_arm.print_arm_points();
        test_arm.search_point_robot_arm(test_arm, 1.49729, 95.4849, 0.000001);
        std::cout << "The searched angle for M1 is : " << test_arm.robot_arm_get_searched_angle('1') * (float)(180 / 3.14159265) << ". The searched angle for M2 is: " << test_arm.robot_arm_get_searched_angle('2') * (float)(180 / 3.14159265) << std::endl;
        test_arm.change_robot_mirror_status();
        test_arm.set_robot_angle(-25.5 * 3.14159265 / 180.0);
        test_arm.print_arm_points();
        test_arm.search_point_robot_arm(test_arm, -1.49729, 95.4849, 0.000001);
        std::cout << "The searched angle for M1 is : " << test_arm.robot_arm_get_searched_angle('1') * (float)(180 / 3.14159265) << ". The searched angle for M2 is: " << test_arm.robot_arm_get_searched_angle('2') * (float)(180 / 3.14159265) << std::endl;
        Robot_Car robot_car_test(5);
        robot_car_test.print_cube_coordinates();
        robot_car_test.print_arm_points();
        robot_car_test.mov_robot_car_coordinates(robot_car_test,10,1);
        robot_car_test.print_cube_coordinates();
        robot_car_test.print_arm_points();
        

std::cout << "The number of robots were : " << Robot_arm::Robot_Get_Counting(2) << " The number of cubes were : " << Box::Box_Get_Counting(2) << " The number of cars were : " << Car::Car_Get_Counting(2) << " The number of robot cars were : " << Robot_Car::Robot_Car_Get_Counting(2) << std::endl;
std::cout << " " << std::endl;
std::cout << "The programm is finished running !" << std::endl;
    }
    std::cin.get();
}

// To do list

    //Pick up while angled is moving crazy and picking wrong position
    //Batch rendering images
    //Abstract everything
    //Test two robots, test blue for selected
    //Test movement of car
    //Função save point pra criar linked list com os pontos desejados
    //Deletar pontos indesejados
    //Criar arquivo txt para amazenar os angulos
    //Fazer robo pegar o box automaticamente
    //Função move para percorrer linked list e nela checar colisao (mover somente em x caso colisao)
    //Salvar imagem da tela (https://www.ti-enxame.com/pt/c%2B%2B/como-tirar-uma-captura-de-tela-no-opengl/973055474/)
    //Juntar em video (https://stackoverflow.com/questions/46444474/c-ffmpeg-create-mp4-file)
    //Include the images in the .exe file https://stackoverflow.com/questions/47414607/how-to-include-data-object-files-images-etc-in-program-and-access-the-symbol/47415163#47415163
    //**************************
    //thread para dois primeiros mecanismos de quatro barras
    //thread para dois mecanismos funcionarem ao msm tempo
    //****************************
    //Modelo dinamico
    //Criar leitura stl
    //Passar modelo para 3d
    //*****************************
    //Criar mobile app
    //Criar funcao de conectar bluetooth 
*/
