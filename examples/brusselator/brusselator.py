#!/usr/bin/env python

import time
import numpy as n
import matplotlib.pyplot as plt
import roadrunner 

r = roadrunner.RoadRunner('brusselator.xml')
max_x = 9
max_y = 10


dx=0.005       # lattice size in x-direction.
dy=0.005      # lattice size in y-direction.
 
nx = int(1/dx)
ny = int(1/dy)
ns = r.model.getStateVector().size

#D = 0.001 * n.array([1.0, 10.0]) # diffusion constant
#D = 0.000001 * n.array([1, 10.0]) # diffusion constant
D = 0.000001 * n.array([0.1, 20.]) # diffusion constant
dt = 0.01

timestep = 0
timeseries = None

dx2=dx**2 # squared lattice spacings
dy2=dy**2 # 

# state of the system, time, and
# two lattices,  need to be copies of each other
time = 0
u0 = n.zeros([nx,ny,ns])
u1 = n.zeros([nx,ny,ns])

# copy of u, this is were rate of change of u will be stored
dudt = n.copy(u0)


ran = n.random.random((nx,ny))
u0[:,:,1] = ran * 3
u0[:,:,0] = ran * 3

#u1[:] = u0


# output image
img = n.zeros([nx, ny, 3], dtype=n.float32) 

# output plot
imgplt = plt.imshow(img, interpolation='nearest')

def rd_step(ui, uo):

    """
    perform a reaction-diffusion step
    
    takes the input lattice, ui, performs the reaction - diffusion step, 
    and writes the output to uo.
    
    ui and uo are (nx,ny,2) 3 dimensional arrays, the last index 
    are the 2 elements of the brusselator state vector.

    This is an example of how to write extremly inefficient Python code. The general 
    rule of writing fast code in Python is to write as little Python as possible. 
    
    """
    global time, nx, ny, r, timeseries, timestep

    for i in range(0,nx):
        for j in range(0,ny):
            # laplacian in x direction   
            uxx = ( ui[i+1 if i < nx-1 else 0, j] - 2*ui[i,j] + ui[i-1, j] )/dx2 
            
            # laplacian in y direction         
            uyy = ( ui[i,j+1 if j < ny-1 else 0] - 2*ui[i,j] + ui[i, j-1] )/dy2
            
            # add rate of change due to reaction part
            uo[i,j] = ui[i,j] + dt*D*(uxx+uyy) + dt * r.model.getStateVectorRate(time, ui[i,j])

            
    time = time + dt
    timeseries[timestep, :] = uo[1,1,:]
    timestep = timestep + 1
           

def rd_step_vec(ui, uo):
    """
    A vectorized version of the reaction diffusion step, code is much smaller 
    and runs about 200 times faster than the non-vectorized version above. 
    
    This function uses a numpy expression to
    evaluate the derivatives in the Laplacian, and
    calculates u[i,j] based on ui[i,j].
    """

    global time, timeseries, timestep, dt, dx2, dy2, dudt
    
    two_ui = 2.0 * ui
    uxx = (n.roll(ui,1,0) + n.roll(ui,-1,0) - two_ui) / dx2
    uyy = (n.roll(ui,1,1) + n.roll(ui,-1,1) - two_ui) / dy2
    uo[:,:,:] = ui + dt * (D * (uxx+uyy) + r.model.getStateVectorRate(time, ui, dudt))

    time = time + dt
    timeseries[timestep, :] = uo[1,1,:]
    timestep = timestep + 1


def update_img(uo):
    """
    update the output image.
    """
    global timestep, max_x, max_y

    if timestep % 100 == 0:

        max_x = n.max(uo[:,:,0])
        max_y = n.max(uo[:,:,1])

        img[:,:,0] = uo[:,:,0] / max_x
        
        img[:,:,1] = uo[:,:,1] / max_y
        
        imgplt.set_array(img)
        
        m = plt.get_current_fig_manager()
        m.canvas.draw()
           
def step(ui, uo):
    global timestep
    rd_step_vec(ui, uo)

    update_img(uo)

    print("{}, {},{}".format(timestep, uo[1,1,0],uo[1,1,0])) 



def run(nsteps):
    global timeseries, timestep
    ui = u0
    uo = u1

    timestep = 0
    timeseries = n.zeros([nsteps, 2])

    plt.show(block=False)

    for i in n.arange(nsteps):
        step(ui, uo)
        ui, uo = uo, ui



