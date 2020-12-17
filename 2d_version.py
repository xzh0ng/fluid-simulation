import numpy as np
import matplotlib.animation as animation
import matplotlib.pyplot as plt


def get_cell_indices(position, h):
    return (position / h).astype(int)

def advection(q, qdot, dt, h, nx, ny):
    q += qdot * dt
    for i in range(len(q) // 2):
        if q[2 * i] < 0:
            q[2 * i] = 0
        if q[2 * i] > h * nx:
            q[2 * i] = 0
        if q[2 * i + 1] < 0:
            q[2 * i + 1] = 0
        if q[2 * i + 1] > h * ny:
            q[2 * i + 1] = 0


def construct_grid(q, qdot, nx, ny, h):
    u = np.zeros((nx - 1, ny))
    v = np.zeros((nx, ny - 1))
    cell_states = np.zeros((nx, ny))

    Wu = np.zeros(q.shape)
    Wv = np.zeros(q.shape)

    for i in range(len(q) // 2):
        p = q[i * 2:i * 2 + 2]
        velocity = qdot[i * 2:i * 2 + 2]
        position_u = get_cell_indices(p - np.array([0.5 * h, 0]))
        position_v = get_cell_indices(p - np.array([0, 0.5 * h]))
        position_p = get_cell_indices(p);

        cell_states[position_p[0], position_p[1]] = 1


        wu_i = bilinear_interpolation_weight(nx - 1, ny, h, p)
        wv_i = bilinear_interpolation_weight(nx, ny - 1, h, p)

        Wu[2 * i], Wu[2 * i + 1], Wu[2 * i + 2], Wu[2 * i + 3] = wu_i[0], wu_i[1], wu_i[2], wu_i[3]
        Wv[2 * i], Wv[2 * i + 1], Wv[2 * i + 2], Wv[2 * i + 3] = wv_i[0], wv_i[1], wv_i[2], wv_i[3]

        for a in range(2):
            for b in range(2):
                w_idx = a * 2 + b
                u[position_u[0] + a, position_u[1] + b] += wu_i[w_idx] * velocity[0]
                v[position_v[0] + a, position_v[1] + b] += wu_i[w_idx] * velocity[1]
    return u, v, Wu, Wv, cell_states


def external_force(q, qdot, f, dt):
    if f.size() == 2:
        for i in range(int(qdot.size() / 2)):
            qdot[2 * i: 2 * i + 2] += f * dt
    else:
        qdot += f * dt

def PIC_transfer(q, qdot, u, v, Wu, Wv, nx, ny, h):
    for i in range(len(q) // 2):
        p = q[i * 2:i * 2 + 2]
        position_u = get_cell_indices(p - np.array([0.5 * h, 0]))
        position_v = get_cell_indices(p - np.array([0, 0.5 * h]))
        position_p = get_cell_indices(p);

        wu_i = Wu[4 * i: 4 * i + 5]
        wv_i = Wv[4 * i: 4 * i + 5]
        for a in range(2):
            for b in range(2):
                w_idx = a * 2 + b
                qdot[2 * i] = u(position_u[0] + a, position_u[1] + b) * wu_i[w_idx]
                qdot[2 * i + 1] = v(position_v[0] + a, position_v[1] + b) * wv_i[w_idx]
    

def pressure_projection():
    pass

def soft_boundary():
    pass

def solid_boundary(u, v):
    u[0, :][u[0, :] < 0] = 0
    u[-1, :][u[-1, :] > 0] = 0
    v[:, 0][v[:, 0] < 0] = 0
    v[:, -1][v[:, -1] > 0] = 0

def bilinear_interpolation_weight(nx, ny, h, p):
    idx = (p / h).astype(int)
    ds = p / h - idx;

    weights = np.zeros(4)
    # x, y
    weights[0] = (1 - ds[0]) * (1 - ds[1])
    # x, y + 1
    weights[1] = (1 - ds[0]) * ds[1]
    # x + 1, y
    weights[2] = ds[0] * (1 - ds[1])
    # x + 1, y + 1
    weights[3] = (1 - ds[0]) * ((1 - ds[1]))

    return weights