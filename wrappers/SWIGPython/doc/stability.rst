******************
Stability Analysis
******************

The stability of a biochemical system is determined by the eigenvalues of the Jacobian matrix.
From roadRunner it is easy to obtain the Jacobian matrix

.. math::

     J=\begin{bmatrix} \dfrac{\partial F_1}{\partial x_1} & \cdots & \dfrac{\partial F_1}{\partial x_n} \\ 
     \vdots & \ddots & \vdots \\ \dfrac{\partial F_m}{\partial x_1} & \cdots & \dfrac{\partial F_m}{\partial x_n}  
     \end{bmatrix}

using the command::

   Jac = rr.getFullJacobian()

returns the Jacobian matrix in the variable Jac.
