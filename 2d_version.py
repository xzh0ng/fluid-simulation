import numpy as np
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import random


def clamp(val, l, h):
    return max(min(val, h), l)

def get_cell_indices(position, h):
    return (position / h).astype(int)

def advection(q, qdot, dt, h, nx, ny):
    q += qdot * dt
    for i in range(len(q) // 2):
        if q[2 * i] < 0:
            q[2 * i] = 0
        if q[2 * i] > h * nx:
            q[2 * i] = h * nx - 1e-5
        if q[2 * i + 1] < 0:
            q[2 * i + 1] = 0
        if q[2 * i + 1] > h * ny:
            q[2 * i + 1] = h * ny - 1e-5


def construct_grid(q, qdot, nx, ny, h):
    u = np.zeros((nx - 1, ny))
    v = np.zeros((nx, ny - 1))
    cell_states = np.zeros((nx, ny))

    Wu = np.zeros(len(q) * 2)
    Wv = np.zeros(len(q) * 2)

    u_corner = np.array([0.5 * h, 0])
    v_corner = np.array([0, 0.5 * h])
    for i in range(len(q) // 2):
        p = q[i * 2:i * 2 + 2]
        velocity = qdot[i * 2:i * 2 + 2]
        position_u = get_cell_indices(p - np.array([0.5 * h, 0]), h)
        position_v = get_cell_indices(p - np.array([0, 0.5 * h]), h)
        position_p = get_cell_indices(p, h);

        cell_states[position_p[0], position_p[1]] = 1


        wu_i = bilinear_interpolation_weight(u_corner, h, p)
        wv_i = bilinear_interpolation_weight(v_corner, h, p)

        Wu[2 * i], Wu[2 * i + 1], Wu[2 * i + 2], Wu[2 * i + 3] = wu_i[0], wu_i[1], wu_i[2], wu_i[3]
        Wv[2 * i], Wv[2 * i + 1], Wv[2 * i + 2], Wv[2 * i + 3] = wv_i[0], wv_i[1], wv_i[2], wv_i[3]

        for a in range(2):
            for b in range(2):
                w_idx = a * 2 + b
                u[clamp(position_u[0] + a, 0, nx - 2), clamp(position_u[1] + a, 0, ny - 1)] += wu_i[w_idx] * velocity[0]
                v[clamp(position_v[0] + a, 0, nx - 1), clamp(position_v[1] + b, 0, ny - 2)] += wu_i[w_idx] * velocity[1]
    return u, v, Wu, Wv, cell_states


def external_force(q, qdot, f, dt):
    for i in range(len(q) // 2):
        qdot[2 * i: 2 * i + 2] += f * dt


def PIC_transfer(q, qdot, u, v, Wu, Wv, nx, ny, h):
    qdot = np.zeros(qdot.shape)
    for i in range(len(q) // 2):
        p = q[i * 2:i * 2 + 2]
        position_u = get_cell_indices(p - np.array([0.5 * h, 0]), h)
        position_v = get_cell_indices(p - np.array([0, 0.5 * h]), h)
        position_p = get_cell_indices(p, h);

        wu_i = Wu[4 * i: 4 * i + 5]
        wv_i = Wv[4 * i: 4 * i + 5]
        for a in range(2):
            for b in range(2):
                w_idx = a * 2 + b
                qdot[2 * i] += u[clamp(position_u[0] + a, 0, nx - 2), clamp(position_u[1] + a, 0, ny - 1)] * wu_i[w_idx]
                qdot[2 * i + 1] += v[clamp(position_v[0] + a, 0, nx - 1), clamp(position_v[1] + b, 0, ny - 2)] * wv_i[w_idx]
    

def pressure_projection(q, qdot, nx, ny, h, dt, density):
    u, v, Wu, Wv, cell_state = construct_grid(q, qdot, nx, ny, h)

    # construct p
    p = np.zeros((nx, ny))
    p[1:nx - 1, :] = u[1:nx - 1, :] - u[0:nx - 2, :]
    p[:, 1:ny - 1] = v[:, 1:ny - 1] - v[:, 0:ny - 2]
    p /= h

    soft_boundary(p, cell_state)

    solid_boundary(u, v)

    # compute gradient
    u[0:nx - 1, :] -= (p[1:nx, :] - p[0:nx - 1, :]) * dt / (h * density)
    v[:, 0:ny - 1] -= (p[:, 1:ny] - p[:, 0:ny - 1]) * dt / (h * density)

    PIC_transfer(q, qdot, u, v, Wu, Wv, nx, ny, h)


def soft_boundary(p, cell_state):
    for i in range(p.shape[0]):
        for j in range(p.shape[1]):
            if cell_state[i, j] == 1:
                continue

            if i >= 1 and cell_state[i - 1, j] == 1:
                p[i - 1, j] -= 0.5 * p[i, j]
            if i + 1 < p.shape[0] and cell_state[i + 1, j] == 1:
                p[i + 1, j] -= 0.5 * p[i, j]
            if j >= 1 and cell_state[i, j - 1] == 1:
                p[i, j - 1] -= 0.5 * p[i, j]
            if j + 1 < p.shape[1] and cell_state[i, j + 1] == 1:
                p[i, j + 1] -= 0.5 * p[i, j]


def solid_boundary(u, v):
    u[0, :][u[0, :] < 0] = 0
    u[-1, :][u[-1, :] > 0] = 0
    v[:, 0][v[:, 0] < 0] = 0
    v[:, -1][v[:, -1] > 0] = 0

def bilinear_interpolation_weight(corner, h, p):
    diffs  = p - corner
    idx = (diffs / h).astype(int)
    ds = diffs / h - idx;

    weights = np.zeros(4)
    # x, y
    weights[0] = (1 - ds[0]) * (1 - ds[1])
    # x, y + 1
    weights[1] = (1 - ds[0]) * ds[1]
    # x + 1, y
    weights[2] = ds[0] * (1 - ds[1])
    # x + 1, y + 1
    weights[3] = ds[0] * ds[1]
    return weights





random.seed(19680801)
nx, ny = 10, 10
h = 0.1
g = np.array([0, -9.8])
n = 200
dt = 0.01
density = 1000
q = np.zeros(n * 2)
qdot = np.zeros(n * 2)
q_prev = None
for i in range(n):
    q[i * 2:2 * i + 2] = np.array([random.uniform(0, nx * h), 1 / 2 * h * ny])
for i in range(n):
    qdot[i * 2:2 * i + 2] = np.array([random.uniform(0, nx * h), random.uniform(0, nx * h)])



# args for animation
fig = plt.figure()
ax = plt.axes(xlim=(0, nx * h), ylim=(0, ny * h))
particles, = ax.plot(q[::2], q[1::2], 'o', ms=5)


def animate(i):
    advection(q, qdot, dt, h, nx, ny)
    external_force(q, qdot, g, dt)
    pressure_projection(q, qdot, nx, ny, h, dt, density)
    particles.set_data(q[::2], q[1::2])
    return particles


plt.xlim(0, nx * h)
plt.ylim(0, ny * h)
anim = animation.FuncAnimation(fig, animate, interval=10)
plt.show()
