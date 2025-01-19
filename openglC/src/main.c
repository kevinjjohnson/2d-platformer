#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "include/shader.h"
#include "include/texture.h"
#include "include/collison.h"
#include "include/batch_renderer.h"
#include "include/quad.h"
#include "include/camera.h"
#include "include/character.h"


typedef struct {
    int size[2];
    char* name;
} Settings;


player player_character;
int a_held = 0;
int d_held = 0;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        printf("Space pressed\n");
        player_character.has_jump = true;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        printf("Space released\n");
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        player_character.movement_dir = 1;
        d_held = 1;
    }
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        if (a_held)
            player_character.movement_dir = -1;
        else
            player_character.movement_dir = 0;
        d_held = 0;
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        player_character.movement_dir = -1;
        a_held = 1;
    }
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        if(d_held)
            player_character.movement_dir = 1;
        else
            player_character.movement_dir = 0;
        a_held = 0;
    }
}

void movePoints(point* points, int size, vec2 amt) {
    for (int i = 0; i < size; i++) {
        points[i].x += amt[0];
        points[i].y += amt[1];
    }
}

int main(void) {
    GLFWwindow* window; 
    Settings s;
    s.name = "i hate bug\0";
    s.size[0] = 1600;
    s.size[1] = 900;
    
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(s.size[0], s.size[1], s.name, NULL, NULL);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("GLEW INIT ERROR");
    }
    glClearColor(0.16f, 0.16f, 0.16f, 1.0f);


    uint32_t vertexShader;
    vertexShader = create_shader("resources/shaders/firstVertex.shader", GL_VERTEX_SHADER);
    
    uint32_t fragmentShader;
    fragmentShader = create_shader("resources/shaders/firstFragment.shader", GL_FRAGMENT_SHADER);
     
    uint32_t shaderProgram;
    shaderProgram = create_shader_program(vertexShader, fragmentShader);

    camera cam;
    init_camera(&cam);


    uint32_t projection = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projection, 1, GL_FALSE, (float*)cam.view_projection_matrix);

    vec2 translationVec;
    
    uint32_t texture1, texture2;
    texture1 = load_texture("resources/textures/player.png", GL_TEXTURE_2D, 0);

    texture2 = load_texture("resources/textures/floor.png", GL_TEXTURE_2D, 1);
    uint32_t tex = glGetUniformLocation(shaderProgram, "textures");
    int samplers[2] = { 0, 1 };
    glUniform1iv(tex, 2, samplers);

    quad ground;
    init_quad(&ground, (vec2) { 1200, 40 }, (vec2) {800, 60}, 1);
    
    
    init_player(&player_character, (vec2) {400, 400});

    batch_renderer renderer;
    init_batch_renderer(&renderer);

    float delta_time = 0.0f;
    float last_frame = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.num_draw_calls = 0;
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            printf("should be zoomin");
            //zoom_camera(&cam, cam.zoom - .01);
        }

        handle_jump(&player_character, delta_time);
        handle_horizontal(&player_character, delta_time);
        handle_gravity(&player_character, delta_time);
        printf("Player x velocity: %f\n", player_character.velocity[0]);
        printf("Player y velocity: %f\n", player_character.velocity[1]);
        apply_physics(&player_character, delta_time);

        
        //printf("bottom left of collider: (%f, %f)\n", polygon2[0].x, polygon2[0].y);
        if (isColliding((point*)player_character.quad.corners, 4, player_character.position, (point*)ground.corners, 4, ground.location, &translationVec)) {
            printf("polygons collided, translation vector is: (%f, %f)\n", translationVec[0], translationVec[1]);
            //location2[0] += translationVec[0];
            //location2[1] += translationVec[1];
            //movePoints(polygon2, 4, translationVec);
            player_character.is_grounded = true;
            move_player(&player_character, translationVec);
        }
        

        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindTextureUnit(0, texture1);
        glBindTextureUnit(1, texture2);
        uint32_t tex = glGetUniformLocation(shaderProgram, "textures");
        int samplers[2] = { 0, 1 };
        glUniform1iv(tex, 2, samplers);
        
        calculate_view_projection_matrix(&cam);
        projection = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projection, 1, GL_FALSE, (float*)cam.view_projection_matrix);
        add_quad(&renderer, ground);
        render_player(&renderer, player_character);

        draw_batch(&renderer);
        flush_renderer(&renderer);
        //printf("number of draw calls this frame: %d\n", renderer.num_draw_calls);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
