#include "raylib.h"
#include <stdlib.h>

#define MAX_PARTICLES 1000

typedef struct {
    Vector2 position;
    Color color;
    float radius;
    Vector2 speed;
    bool active;
} Particle;

void SetParticleYPos(Particle particles[], float YPos)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        particles[i].position = (Vector2){GetRandomValue(0,GetScreenWidth()), YPos};
    }
    
}

void DrawParticles(Particle particles[])
{
    for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i].active) {
                DrawCircleV(particles[i].position, particles[i].radius, particles[i].color);
            }
        }
}

void SetParticleActive(Particle particles[], bool active)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        particles[i].active = active;
    }
}

void ParticlesUpdate(Particle particles[])
{
    for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i].active) {
                particles[i].position.x += particles[i].speed.x;
                particles[i].position.y += particles[i].speed.y;

                // Se una particella esce dallo schermo, la riportiamo al centro
                if (particles[i].position.x < 0 || particles[i].position.x > GetScreenWidth() ||
                    particles[i].position.y < 0 || particles[i].position.y > GetScreenHeight())
                    {
                        particles[i].position = (Vector2){ GetScreenWidth() / 2, GetScreenHeight() / 2 };
                        particles[i].active = false;
                }
            }
    }
}