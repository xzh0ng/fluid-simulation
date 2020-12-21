import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import random

gravity = np.array([0, -0.005])
h = 0.08
k_near = 0.0005
k = k_near / 100
rho_0 = 3.0
dt = 1
random.seed(1)
n = 400
particles = []
canvas_width = 1
canvas_height = 1
damp = 0.5
sigma = 0.1
beta = 1

class particle:
    def __init__(self, x=0, y=0, u=0, v=0) -> None:
        self.prev_pos = np.array([0, 0])
        self.pos = np.array([x, y])
        self.v = np.array([u, v])
        
        self.neighbors = []
        self.rho = 0
        self.rho_near = 0

        self.p = 0
        self.p_near = 0


def calculate_density(particles):
    for i in range(len(particles)):
        for j in range(len(particles)):
            if i >= j:
                continue

            r_ij = particles[j].pos - particles[i].pos
            dist = np.linalg.norm(r_ij)
            q = dist / h
            if q < 1:
                particles[j].rho += (1 - q) ** 2
                particles[j].rho_near += (1 - q) ** 3
                particles[i].neighbors.append(particles[j])
                particles[i].rho += (1 - q) ** 2
                particles[i].rho_near += (1 - q) ** 3

def calculate_pressure(particles):
    for p in particles:
        p.p = k * (p.rho - rho_0)
        p.p_near = k_near * p.rho_near

def apply_displacements(particles):
    for i in particles:
        dx = np.zeros(2)
        for j in i.neighbors:
            r_ij = j.pos - i.pos
            dist = np.linalg.norm(r_ij)
            q = dist / h
            if q < 1:
                D_ij = dt ** 2 * (i.p * (1 - q) + i.p_near * (1 - q) ** 2) * (r_ij / dist)
                j.pos = j.pos + D_ij / 2
                dx = dx - D_ij / 2
        i.pos = i.pos + dx

def apply_gravity(particles):
    for p in particles:
        p.v = p.v + dt * gravity

def move_particles(particles):
    for p in particles:
        p.prev_pos = p.pos
        p.pos = p.pos + dt * p.v
        
        p.rho = 0
        p.rho_near = 0
        p.neighbors = []


def apply_viscosity(particles):
    for i in particles:
        for j in i.neighbors:
            r_ij = j.pos - i.pos
            dist = np.linalg.norm(r_ij)
            q = dist / h
            u = (i.v - j.v).dot(r_ij / dist)
            if u > 0:
                I = dt * (1 - q) * (sigma * u + beta * u ** 2) * (r_ij / dist)
                i.v = i.v - I / 2
                j.v = j.v + I / 2

def prep_for_next_step(particles):
    for p in particles:
        p.v = (p.pos - p.prev_pos) / dt
        if p.pos[0] <= 0:
            p.pos[0] = 0
            if p.v[0] < 0:
                p.v[0] *= -1 * damp
        elif p.pos[0] >= canvas_width:
            p.pos[0] = canvas_width
            if p.v[0] > 0:
                p.v[0] *= -1 * damp
        if p.pos[1] <= 0:
            p.pos[1] = 0
            if p.v[1] < 0:
                p.v[1] *= -1 * damp
        elif p.pos[1] >= canvas_height:
            p.pos[1] = canvas_height
            if p.v[1] > 0:
                p.v[1] *= -1 * damp
        

def double_density_relaxation(particles):
    calculate_density(particles)
    calculate_pressure(particles)


fig = plt.figure()
axes = fig.add_subplot(xlim=(0, canvas_width), ylim=(0, canvas_height))
points, = axes.plot([], [], 'bo', ms=10)


curr_n = 0
def animate(i):
    global curr_n
    if curr_n < n:
        for i in range(10):
            p = particle(x=random.uniform(1 / 3 * canvas_width, 2 / 3 * canvas_width), y=random.uniform(2 / 3 * canvas_width, canvas_width))
            particles.append(p)
        curr_n += 10

    apply_gravity(particles)
    apply_viscosity(particles)
    move_particles(particles)
    double_density_relaxation(particles)
    apply_displacements(particles)
    prep_for_next_step(particles)
    xs = [p.pos[0] for p in particles]
    ys = [p.pos[1] for p in particles]
    points.set_data(xs, ys)
    return points,

ani = animation.FuncAnimation(fig, animate, interval=10, blit=True)
plt.show()