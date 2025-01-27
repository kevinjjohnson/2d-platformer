#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <math.h>

#include "include/shader.h"
#include "include/texture.h"
#include "include/collison.h"
#include "include/batch_renderer.h"
#include "include/quad.h"
#include "include/camera.h"
#include "include/character.h"
#include "include/engine.h"

typedef struct {
    int size[2];
    char* name;
} Settings;


frame_input input = {0, false, false, false };

player player_character;
int a_held = 0;
int d_held = 0;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        input.space_held = true;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        input.space_held = false;
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        input.d_held = true;
        input.last = 1;
    }
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        input.d_held = false;
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        input.a_held = true;
        input.last = -1;
    }
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        input.a_held = false;
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
    //glfwSwapInterval(0);
    glfwSetKeyCallback(window, key_callback);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("GLEW INIT ERROR");
    }
    glClearColor(0.16f, 0.16f, 0.16f, 1.0f);

    batch_renderer renderer;
    init_batch_renderer(&renderer);
    
    camera cam;
    init_camera(&cam);

    vec2 translationVec;
    
    uint32_t texture1, texture2;
    texture1 = load_texture("resources/textures/player.png", GL_TEXTURE_2D, 0);

    texture2 = load_texture("resources/textures/floor.png", GL_TEXTURE_2D, 1);
    uint32_t tex = glGetUniformLocation(renderer.shader_program, "textures");
    int samplers[2] = { 0, 1 };
    glUniform1iv(tex, 2, samplers);

    quad ground;
    init_quad(&ground, (vec2) { 1200, 40 }, (vec2) {800, 60}, 1);
    collider c1 = {
        ground.corners,
        4,
        ground.location
    };


    quad platform1;
    init_quad(&platform1, (vec2) { 200, 20 }, (vec2) { 400, 190 }, 1);
    collider c2 = {
        platform1.corners,
        4,
        platform1.location
    };
    
    init_player(&player_character, (vec2) {400, 400});
    collider player_collider = {
        player_character.quad.corners,
        4,
        player_character.position
    };
    
    collider collidables[10];
    collidables[0] = c1;
    collidables[1] = c2;
    int num_collidables = 2;


    float delta_time = 0.0f;
    float last_frame = 0.0f;
    float lerp_speed = 0.5f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        translationVec[0] = 0;
        translationVec[1] = 0;
        renderer.num_draw_calls = 0;
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        float fps = 1 / delta_time;
        printf("fps: %f\n", fps);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            printf("should be zoomin");
            //zoom_camera(&cam, cam.zoom - .01);
        }
        handle_input(input, &player_character);
        
        handle_jump(input, &player_character, delta_time);
        handle_horizontal(&player_character, delta_time);
        handle_gravity(input, &player_character, delta_time);
        //printf("Player x velocity: %f\n", player_character.velocity[0]);
        //printf("Player y velocity: %f\n", player_character.velocity[1]);
        apply_physics(&player_character, delta_time);

        player_character.is_grounded = false;
        //printf("bottom left of collider: (%f, %f)\n", polygon2[0].x, polygon2[0].y);
        
        for (int i = 0; i < num_collidables; i++) {
            if (check_collision(player_collider, collidables[i], &translationVec)) {
                if (translationVec[1] > 0) {
                    printf("1st if\n");
                    player_character.is_grounded = true;
                }
                if (translationVec[1] < 0) {
                    printf("1st if\n");
                    player_character.velocity[1] = 0;
                }
                move_player(&player_character, translationVec);
            }
        }
        
        /*
        if (is_colliding((point*)player_character.quad.corners, 4, player_character.position, (point*)ground.corners, 4, ground.location, &translationVec)) {
            //printf("polygons collided, translation vector is: (%f, %f)\n", translationVec[0], translationVec[1]);
            if (translationVec[1] > 0) {
                printf("1st if\n");
                player_character.is_grounded = true;
            }
            move_player(&player_character, translationVec);
        }
        if (is_colliding((point*)player_character.quad.corners, 4, player_character.position, (point*)platform1.corners, 4, platform1.location, &translationVec)) {
            printf("polygons collided, translation vector is: (%f, %f)\n", translationVec[0], translationVec[1]);
            if (translationVec[1] > 0) {
                printf("2nd if\n");
                player_character.is_grounded = true;
            }
            
            move_player(&player_character, translationVec);
        }
        */

        glUseProgram(renderer.shader_program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindTextureUnit(0, texture1);
        glBindTextureUnit(1, texture2);
        uint32_t tex = glGetUniformLocation(renderer.shader_program, "textures");
        int samplers[2] = { 0, 1 };
        glUniform1iv(tex, 2, samplers);
        
        //vec2 cam_pos = { 0, 0 };
        //vec2 cam_center = { cam.position[0] + 800, cam.position[1] + 450 };
        //float blend = 1 - pow(0.5f, delta_time * lerp_speed);
        //glm_vec2_lerp(cam_center, player_character.position, blend, cam_pos);
        //printf("camera_pos: %f, %f\n", cam_pos[0], cam_pos[1]);
        center_camera(&cam, player_character.position);
        calculate_view_projection_matrix(&cam);
        uint32_t projection = glGetUniformLocation(renderer.shader_program, "projection");
        glUniformMatrix4fv(projection, 1, GL_FALSE, (float*)cam.view_projection_matrix);
        add_quad(&renderer, ground);
        add_quad(&renderer, platform1);
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
