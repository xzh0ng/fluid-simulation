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

    for i in range(len(q) // 2):
        p = q[i * 2:i * 2 + 2]

        position_u = get_cell_indices(p - np.array([0.5 * h, 0]))
        position_v = get_cell_indices(p - np.array([0, 0.5 * h]))
        position_p = get_cell_indices(p);

        cell_states(position_p[0], position_p[1]) = 1

        wu_i = bilinear_interpolation_weight(nx - 1, ny, h, p)



def external_force():
    pass

def PIC_transfer():
    pass

def pressure_projection():
    pass

def soft_boundary():
    pass

def solid_boundary():
    pass

def bilinear_interpolation_weight(nx, ny, h, p):
    pass
