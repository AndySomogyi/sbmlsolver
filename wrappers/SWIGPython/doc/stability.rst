******************
Stability Analysis
******************

The stability of a biochemical system is determined by the eigenvalues of the Jacobian matrix. The
Jacobian matrix is defined as follows:


.. math::

     J=\begin{bmatrix} \dfrac{\partial F_1}{\partial S_1} & \cdots & \dfrac{\partial F_1}{\partial S_n} \\ 
     \vdots & \ddots & \vdots \\ \dfrac{\partial F_m}{\partial S_1} & \cdots & \dfrac{\partial F_m}{\partial S_n}  
     \end{bmatrix}

where :math:`F_i` is the ith differential equation. From roadRunner it is easy to obtain the Jacobian matrix using
the command::

   Jac = rr.getFullJacobian()

which returns the Jacobian matrix in the variable Jac.
